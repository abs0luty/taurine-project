#include "lunarity/include/lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void lunarity_advance_lexer_state(lunarity_lexer_state_t *state);

static void lunarity_advance_lexer_state_twice(lunarity_lexer_state_t *state);

static void lunarity_lexer_state_skip_whitespaces(lunarity_lexer_state_t *state);

static bool lunarity_is_whitespace(arty_codepoint_t codepoint);

static lunarity_token_t lunarity_next_name_token(lunarity_lexer_state_t *state);

static size_t lunarity_binary_search_kw(const char *name);

lunarity_lexer_state_t lunarity_new_lexer_state(arty_utf8_string_iterator_t it)
{
  lunarity_lexer_state_t state;
  state.it = it;
  state.cursor = lunarity_new_byte_location(1, 0, 0);
  state.current = arty_advance_utf8_string_iterator(&state.it);
  state.next = arty_advance_utf8_string_iterator(&state.it);
  return state;
}

static void lunarity_advance_lexer_state(lunarity_lexer_state_t *state)
{
  if (state->current == NO_CODEPOINT)
  {
    return;
  }

  state->cursor.offset += arty_utf8_bytes_in_codepoint(state->current);
  if (state->current == '\n')
  {
    state->cursor.line++;
    state->cursor.column = 0;
  }
  else
  {
    state->cursor.column++;
  }

  state->current = state->next;
  state->next = arty_advance_utf8_string_iterator(&state->it);
}

static void lunarity_advance_lexer_state_twice(lunarity_lexer_state_t *state) {
  lunarity_advance_lexer_state(state);
  lunarity_advance_lexer_state(state);
}

static void lunarity_lexer_state_skip_whitespaces(lunarity_lexer_state_t *state)
{
  while (lunarity_is_whitespace(state->current))
  {
    lunarity_advance_lexer_state(state);
  }
}

static lunarity_token_t lunarity_next_name_token(lunarity_lexer_state_t *state)
{
  lunarity_byte_location_t start_location = state->cursor;
  while (arty_is_xid_continue(state->current) || state->current == '_')
  {
    lunarity_advance_lexer_state(state);
  }

  char *name = malloc(state->cursor.offset - start_location.offset + 1);
  strcpy(name, state->it.src + start_location.offset);
  name[state->cursor.offset - start_location.offset] = '\0';

  lunarity_advance_lexer_state(state);

  lunarity_span_t span = lunarity_new_span(start_location, state->cursor);

  size_t kw_index = lunarity_binary_search_kw(name);

  if (kw_index != -1)
  {
    return lunarity_new_token_with_data(
        LUNARITY_TOKEN_KIND_KW_AND + kw_index,
        span,
        name);
  }
  else
  {
    return lunarity_new_token_with_data(
        LUNARITY_TOKEN_KIND_IDENTIFIER,
        span,
        name);
  }
}

#define SINGLE_BYTE_PUNCTUATION(_char, _kind)                                  \
  if (state->current == _char) {                                               \
    lunarity_span_t span = lunarity_new_single_byte_span(state->cursor);       \
    lunarity_advance_lexer_state(state);                                       \
    return lunarity_new_token(_kind, span);                                    \
  }

#define DOUBLE_BYTE_PUNCTUATION(_char1, _char2, _kind)                         \
  if (state->current == _char1 && state->next == _char2) {                     \
    lunarity_span_t span = lunarity_new_double_byte_span(state->cursor);       \
    lunarity_advance_lexer_state_twice(state);                                 \
    return lunarity_new_token(_kind, span);                                    \
  }

lunarity_token_t lunarity_next_token(lunarity_lexer_state_t *state)
{
  lunarity_token_t token;

  lunarity_lexer_state_skip_whitespaces(state);

  if (arty_is_xid_start(state->current) || state->current == '_')
  {
    return lunarity_next_name_token(state);
  }

  SINGLE_BYTE_PUNCTUATION('+', LUNARITY_TOKEN_KIND_PLUS)
  SINGLE_BYTE_PUNCTUATION('-', LUNARITY_TOKEN_KIND_MINUS)
  SINGLE_BYTE_PUNCTUATION('*', LUNARITY_TOKEN_KIND_ASTERISK)
  SINGLE_BYTE_PUNCTUATION('%', LUNARITY_TOKEN_KIND_PERCENT)
  SINGLE_BYTE_PUNCTUATION('^', LUNARITY_TOKEN_KIND_CARET)
  SINGLE_BYTE_PUNCTUATION('#', LUNARITY_TOKEN_KIND_HASH)
  SINGLE_BYTE_PUNCTUATION('(', LUNARITY_TOKEN_KIND_LPAREN)
  SINGLE_BYTE_PUNCTUATION(')', LUNARITY_TOKEN_KIND_RPAREN)
  SINGLE_BYTE_PUNCTUATION('{', LUNARITY_TOKEN_KIND_LBRACE)
  SINGLE_BYTE_PUNCTUATION('}', LUNARITY_TOKEN_KIND_RBRACE)
  SINGLE_BYTE_PUNCTUATION('[', LUNARITY_TOKEN_KIND_LBRACKET)
  SINGLE_BYTE_PUNCTUATION(']', LUNARITY_TOKEN_KIND_RBRACKET)
  SINGLE_BYTE_PUNCTUATION(',', LUNARITY_TOKEN_KIND_COMMA)
  SINGLE_BYTE_PUNCTUATION(';', LUNARITY_TOKEN_KIND_SEMICOLON)
  SINGLE_BYTE_PUNCTUATION('&', LUNARITY_TOKEN_KIND_AMPERSAND)
  SINGLE_BYTE_PUNCTUATION('|', LUNARITY_TOKEN_KIND_BAR)

  DOUBLE_BYTE_PUNCTUATION('>', '>', LUNARITY_TOKEN_KIND_GTGT)
  DOUBLE_BYTE_PUNCTUATION('>', '=', LUNARITY_TOKEN_KIND_GE)
  SINGLE_BYTE_PUNCTUATION('>', LUNARITY_TOKEN_KIND_GT)

  DOUBLE_BYTE_PUNCTUATION('<', '<', LUNARITY_TOKEN_KIND_LTLT)
  DOUBLE_BYTE_PUNCTUATION('<', '=', LUNARITY_TOKEN_KIND_LE)
  SINGLE_BYTE_PUNCTUATION('<', LUNARITY_TOKEN_KIND_LT)

  DOUBLE_BYTE_PUNCTUATION('/', '/', LUNARITY_TOKEN_KIND_DOUBLE_SLASH)
  SINGLE_BYTE_PUNCTUATION('/', LUNARITY_TOKEN_KIND_SLASH)

  DOUBLE_BYTE_PUNCTUATION('=', '=', LUNARITY_TOKEN_KIND_DOUBLE_EQ)
  DOUBLE_BYTE_PUNCTUATION('~', '=', LUNARITY_TOKEN_KIND_TILDE_EQ)
  SINGLE_BYTE_PUNCTUATION('=', LUNARITY_TOKEN_KIND_EQ)
  SINGLE_BYTE_PUNCTUATION('~', LUNARITY_TOKEN_KIND_TILDE)

  DOUBLE_BYTE_PUNCTUATION(':', ':', LUNARITY_TOKEN_KIND_DOUBLE_COLON)
  SINGLE_BYTE_PUNCTUATION(':', LUNARITY_TOKEN_KIND_COLON)

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

  if (state->current == NO_CODEPOINT)
  {
    return lunarity_new_token(
        LUNARITY_TOKEN_KIND_EOF,
        lunarity_new_single_byte_span(state->cursor));
  }

  return token;
}

const char *KEYWORDS[] = {
    "and", "break", "do", "else", "elseif", "end", "false",
    "for", "function", "goto", "if", "in", "local", "nil",
    "not", "or", "repeat", "return", "then", "true", "until",
    "while"};

static size_t lunarity_binary_search_kw(const char *name)
{
  size_t low = 0, high = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]) - 1;

  while (low < high)
  {
    size_t mid = low + (high - low) / 2;

    int cmp = strcmp(KEYWORDS[mid], name);

    if (cmp < 0)
    {
      low = mid + 1;
    }
    else if (cmp > 0)
    {
      high = mid;
    }
    else
    {
      return mid;
    }
  }

  return -1;
}

static bool lunarity_is_whitespace(arty_codepoint_t codepoint)
{
  return codepoint == 0x009 || codepoint == 0x000a || codepoint == 0x000b || codepoint == 0x000c || codepoint == 0x000d || codepoint == 0x0020 || codepoint == 0x0085 || codepoint == 0x200E || codepoint == 0x200F || codepoint == 0x2028 || codepoint == 0x2029;
}
