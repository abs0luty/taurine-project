#include "lunarity/lexer.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief   Advances the lexer state by one Unicode codepoint.
 * @version 0.1.0
 */
static void lunarity_advance_lexer_state(lunarity_lexer_state_t *state);

/**
 * @brief   Advances the lexer state by two Unicode codepoints.
 * @version 0.1.0
 */
static void lunarity_advance_lexer_state_twice(lunarity_lexer_state_t *state);

/**
 * @brief   Skips all next whitespace characters in the lexer state.
 * @version 0.1.0
 */
static void
lunarity_lexer_state_skip_whitespaces(lunarity_lexer_state_t *state);

/**
 * @return `true` if the given Unicode codepoint is a whitespace character.
 * @version 0.1.0
 */
static bool lunarity_is_whitespace(arty_codepoint_t codepoint);

/**
 * @returns The ASCII lowercase version of the given codepoint.
 * @version 0.1.0
 */
static arty_codepoint_t lunarity_to_ascii_lowercase(arty_codepoint_t codepoint);

/**
 * @brief   Advances the lexer state, consuming XID_Continue characters.
 * @returns The next identifier or keyword token in the lexer state.
 * @version 0.1.0
 */
static lunarity_token_t
lunarity_lexer_state_next_name_token(lunarity_lexer_state_t *state);

/**
 * @brief   Advances the lexer state, consuming numeric characters.
 * @returns The next number token in the lexer state.
 * @version 0.1.0
 */
static lunarity_token_t
lunarity_next_number_token(lunarity_lexer_state_t *state);

/**
 * @brief   Advances the lexer state, consuming string characters.
 * @returns The next string token in the lexer state.
 * @version 0.1.0
 */
static lunarity_token_t
lunarity_lexer_state_next_string_token(lunarity_lexer_state_t *state);

static bool process_escape_sequence(lunarity_lexer_state_t *state,
                                    arty_codepoint_t *codepoint);

/**
 * @brief   Searches for a keyword in the list of keywords.
 * @returns The index of the keyword in the list or `-1` if it was not found.
 * @version 0.1.0
 */
static size_t lunarity_binary_search_kw(const char *name);

lunarity_lexer_state_t
lunarity_new_lexer_state(arty_utf8_string_iterator_t it) {
  lunarity_lexer_state_t state;
  state.it = it;
  state.cursor = lunarity_new_byte_location(1, 0, 0);
  state.current = arty_advance_utf8_string_iterator(&state.it);
  state.next = arty_advance_utf8_string_iterator(&state.it);
  return state;
}

static void lunarity_advance_lexer_state(lunarity_lexer_state_t *state) {
  if (state->current == NO_CODEPOINT) {
    return;
  }

  state->cursor.offset += arty_utf8_bytes_in_codepoint(state->current);
  if (state->current == '\n') {
    state->cursor.line++;
    state->cursor.column = 0;
  } else {
    state->cursor.column++;
  }

  state->current = state->next;
  state->next = arty_advance_utf8_string_iterator(&state->it);
}

static void lunarity_advance_lexer_state_twice(lunarity_lexer_state_t *state) {
  lunarity_advance_lexer_state(state);
  lunarity_advance_lexer_state(state);
}

static void
lunarity_lexer_state_skip_whitespaces(lunarity_lexer_state_t *state) {
  while (lunarity_is_whitespace(state->current)) {
    lunarity_advance_lexer_state(state);
  }
}

static lunarity_token_t
lunarity_lexer_state_next_name_token(lunarity_lexer_state_t *state) {
  lunarity_byte_location_t start_location = state->cursor;
  while (arty_is_xid_continue(state->current) || state->current == '_') {
    lunarity_advance_lexer_state(state);
  }

  size_t length = state->cursor.offset - start_location.offset + 1;
  char *name = malloc(length);
  memcpy(name, state->it.src + start_location.offset, length - 1);
  name[length - 1] = '\0';

  lunarity_span_t span = lunarity_new_span(start_location, state->cursor);
  size_t kw_index = lunarity_binary_search_kw(name);

  if (kw_index != -1) {
    return lunarity_new_token_with_string_data(
        LUNARITY_TOKEN_KIND_KW_AND + kw_index, span, name);
  } else {
    return lunarity_new_token_with_string_data(LUNARITY_TOKEN_KIND_IDENTIFIER,
                                               span, name);
  }
}

// TODO: Implement scanning numbers
static lunarity_token_t
lunarity_next_number_token(lunarity_lexer_state_t *state) {
  lunarity_token_t token;
  return token;
}

static lunarity_token_t
lunarity_lexer_state_next_string_token(lunarity_lexer_state_t *state) {
  lunarity_byte_location_t start_location = state->cursor;
  arty_codepoint_t quote = state->current;
  lunarity_advance_lexer_state(state);

  while (state->current != '\n' && state->current != NO_CODEPOINT) {
    if (state->current == '"' || state->current == '\'') {
      break;
    }

    // TODO: Implement escape sequences
    lunarity_advance_lexer_state(state);
  }

  if (state->current != quote) {
    return lunarity_new_token(LUNARITY_TOKEN_KIND_UNTERMINATED_STRING_LITERAL,
                              lunarity_new_span(state->cursor, state->cursor));
  }

  // The first quote is ignored
  size_t length = state->cursor.offset - start_location.offset;
  char *string = malloc(length);
  memcpy(string, state->it.src + start_location.offset + 1, length - 1);
  string[length - 1] = '\0';

  lunarity_advance_lexer_state(state);
  return lunarity_new_token_with_string_data(
      LUNARITY_TOKEN_KIND_STRING,
      lunarity_new_span(start_location, state->cursor), string);
}

// TODO: Implement escape sequences in strings
static bool process_escape_sequence(lunarity_lexer_state_t *state,
                                    arty_codepoint_t *codepoint) {
  return false;
}

/**
 * @brief   Scans for a single byte punctuation token.
 * @param   byte       The byte to match.
 * @param   name       The name of the punctuation in screaming snake case.
 * @version 0.1.0
 */
#define SINGLE_BYTE_PUNCTUATION(byte, name)                                    \
  do {                                                                         \
    if (state->current == byte) {                                              \
      lunarity_span_t span = lunarity_new_single_byte_span(state->cursor);     \
      lunarity_advance_lexer_state(state);                                     \
      return lunarity_new_token(LUNARITY_TOKEN_KIND_##name, span);             \
    }                                                                          \
  } while (0)

/**
 * @brief   Scans for a double byte punctuation token.
 * @param   first_byte The first byte to match.
 * @param   second_byte The second byte to match.
 * @param   name       The name of the punctuation in screaming snake case.
 * @version 0.1.0
 */
#define DOUBLE_BYTE_PUNCTUATION(first_byte, second_byte, name)                 \
  do {                                                                         \
    if (state->current == first_byte && state->next == second_byte) {          \
      lunarity_span_t span = lunarity_new_double_byte_span(state->cursor);     \
      lunarity_advance_lexer_state_twice(state);                               \
      return lunarity_new_token(LUNARITY_TOKEN_KIND_##name, span);             \
    }                                                                          \
  } while (0)

lunarity_token_t
lunarity_lexer_state_next_token(lunarity_lexer_state_t *state) {
  lunarity_lexer_state_skip_whitespaces(state);

  // Identifier or keyword
  if (arty_is_xid_start(state->current) || state->current == '_') {
    return lunarity_lexer_state_next_name_token(state);
  }

  // String literal
  if (state->current == '\'' || state->current == '"') {
    return lunarity_lexer_state_next_string_token(state);
  }

  // Punctuation
  SINGLE_BYTE_PUNCTUATION('+', PLUS);
  SINGLE_BYTE_PUNCTUATION('-', MINUS);
  SINGLE_BYTE_PUNCTUATION('*', ASTERISK);
  SINGLE_BYTE_PUNCTUATION('%', PERCENT);
  SINGLE_BYTE_PUNCTUATION('^', CARET);
  SINGLE_BYTE_PUNCTUATION('#', HASH);
  SINGLE_BYTE_PUNCTUATION('(', LPAREN);
  SINGLE_BYTE_PUNCTUATION(')', RPAREN);
  SINGLE_BYTE_PUNCTUATION('{', LBRACE);
  SINGLE_BYTE_PUNCTUATION('}', RBRACE);
  SINGLE_BYTE_PUNCTUATION('[', LBRACKET);
  SINGLE_BYTE_PUNCTUATION(']', RBRACKET);
  SINGLE_BYTE_PUNCTUATION(',', COMMA);
  SINGLE_BYTE_PUNCTUATION(';', SEMICOLON);
  SINGLE_BYTE_PUNCTUATION('&', AMPERSAND);
  SINGLE_BYTE_PUNCTUATION('|', BAR);
  DOUBLE_BYTE_PUNCTUATION('>', '>', GTGT);
  DOUBLE_BYTE_PUNCTUATION('>', '=', GE);
  SINGLE_BYTE_PUNCTUATION('>', GT);
  DOUBLE_BYTE_PUNCTUATION('<', '<', LTLT);
  DOUBLE_BYTE_PUNCTUATION('<', '=', LE);
  SINGLE_BYTE_PUNCTUATION('<', LT);
  DOUBLE_BYTE_PUNCTUATION('/', '/', DOUBLE_SLASH);
  SINGLE_BYTE_PUNCTUATION('/', SLASH);
  DOUBLE_BYTE_PUNCTUATION('=', '=', DOUBLE_EQ);
  DOUBLE_BYTE_PUNCTUATION('~', '=', TILDE_EQ);
  SINGLE_BYTE_PUNCTUATION('=', EQ);
  SINGLE_BYTE_PUNCTUATION('~', TILDE);
  DOUBLE_BYTE_PUNCTUATION(':', ':', DOUBLE_COLON);
  SINGLE_BYTE_PUNCTUATION(':', COLON);

  // Punctuation: `.`, `..` or `...`
  if (state->current == '.') {
    if (state->next == '.') {
      lunarity_byte_location_t start_location = state->cursor;
      lunarity_advance_lexer_state(state);

      if (state->next == '.') {
        lunarity_advance_lexer_state_twice(state);
        return lunarity_new_token(
            LUNARITY_TOKEN_KIND_TRIPLE_DOT,
            lunarity_new_triple_byte_span(start_location));
      }

      lunarity_advance_lexer_state(state);
      return lunarity_new_token(LUNARITY_TOKEN_KIND_DOUBLE_DOT,
                                lunarity_new_double_byte_span(start_location));
    }

    lunarity_byte_location_t start_location = state->cursor;
    lunarity_advance_lexer_state(state);
    return lunarity_new_token(LUNARITY_TOKEN_KIND_DOT,
                              lunarity_new_single_byte_span(start_location));
  }

  // End of file
  if (state->current == NO_CODEPOINT) {
    return lunarity_new_token(LUNARITY_TOKEN_KIND_EOF,
                              lunarity_new_single_byte_span(state->cursor));
  }

  // Unexpected character
  lunarity_byte_location_t start_location = state->cursor;
  lunarity_advance_lexer_state(state);
  return lunarity_new_token(LUNARITY_TOKEN_KIND_UNEXPECTED_CHARACTER,
                            lunarity_new_span(start_location, state->cursor));
}

// List of sorted Lunarity's keywords.
const char *KEYWORDS[] = {
    "and",      "break",  "do",   "else", "elseif", "end",  "false", "for",
    "function", "goto",   "if",   "in",   "local",  "nil",  "not",   "or",
    "repeat",   "return", "then", "true", "until",  "while"};

static size_t lunarity_binary_search_kw(const char *name) {
  size_t start = 0, end = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]) - 1;

  while (start < end) {
    size_t mid = start + (end - start) / 2;
    int cmp = strcmp(KEYWORDS[mid], name);

    if (cmp < 0) {
      start = mid + 1;
    } else if (cmp > 0) {
      end = mid;
    } else {
      return mid;
    }
  }

  return -1;
}

static bool lunarity_is_whitespace(arty_codepoint_t codepoint) {
  return codepoint == 0x0009 || codepoint == 0x000a || codepoint == 0x000b ||
         codepoint == 0x000c || codepoint == 0x000d || codepoint == 0x0020 ||
         codepoint == 0x0085 || codepoint == 0x200E || codepoint == 0x200F ||
         codepoint == 0x2028 || codepoint == 0x2029;
}

static arty_codepoint_t
lunarity_to_ascii_lowercase(arty_codepoint_t codepoint) {
  if (codepoint >= 'A' && codepoint <= 'Z') {
    return codepoint - 'A' + 'a';
  } else {
    return codepoint;
  }
}
