#include "lexer.h"
#include <string.h>

static void lunarity_advance_lexer_state(lunarity_lexer_state_t *state);
static void lunarity_lexer_state_skip_whitespaces(lunarity_lexer_state_t *state);
static bool lunarity_is_whitespace(arty_codepoint_t codepoint);
static lunarity_token_t lunarity_next_name_token(lunarity_lexer_state_t *state);
static bool lunarity_check_for_kw(const char* name, lunarity_span_t span, lunarity_token_t* token);
static size_t lunarity_binary_search_kw(const char* name);

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
}

static void lunarity_lexer_state_skip_whitespaces(lunarity_lexer_state_t *state)
{
  while (lunarity_is_whitespace(state->current)) {
    lunarity_advance_lexer_state(state);
  }
}

static lunarity_token_t lunarity_next_name_token(lunarity_lexer_state_t *state)
{
  lunarity_byte_location_t start_location = state->cursor;
  while (arty_is_xid_continue(state->current)) {
    lunarity_advance_lexer_state(state);
  }

  char* name = (char *)malloc(state->cursor.offset - start_location.offset + 1);
  strcpy(name, state->it.src + start_location.offset);

  lunarity_span_t span = lunarity_new_span(start_location, state->cursor);

  lunarity_token_t token;

  if (lunarity_check_for_kw(name, span, &token)) {
    return token;
  } else {
    return lunarity_new_token_with_data(
        LUNARITY_TOKEN_KIND_IDENTIFIER,
        span,
        name);
  }
}

lunarity_token_t lunarity_next_token(lunarity_lexer_state_t *state)
{
  lunarity_token_t token;

  lunarity_lexer_state_skip_whitespaces(state);

  if (arty_is_xid_start(state->current)) {
    return lunarity_next_name_token(state);
  }

  if (state->current == NO_CODEPOINT) {
    return lunarity_new_token(
      LUNARITY_TOKEN_KIND_EOF,
      lunarity_new_single_byte_span(state->cursor));
  }

  return token;
}

const char* KEYWORDS[] = {
  "and", "break", "do", "else", "elseif", "end", "false",
  "for", "function", "goto", "if", "in", "local", "nil",
  "not", "or", "repeat", "return", "then", "true", "until",
  "while"
};

static bool lunarity_check_for_kw(const char *name, lunarity_span_t span, lunarity_token_t* token)
{
  size_t kw_index = lunarity_binary_search_kw(name);

  if (kw_index != -1) {
    *token = lunarity_new_token_with_data(
        LUNARITY_TOKEN_KIND_KW_AND + kw_index,
        span,
        name
    );
    return true;
  }

  return false;
}

static size_t lunarity_binary_search_kw(const char* name)
{
  size_t low = 0, high = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]) - 1;

  while (low <= high) {
    size_t mid = low + (high - low) / 2;
    int cmp = strcmp(KEYWORDS[mid], name);

    if (cmp == 0) {
      return mid;
    } else if (cmp < 0) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return -1;
}

static bool lunarity_is_whitespace(arty_codepoint_t codepoint)
{
  return codepoint == 0x009
    || codepoint == 0x000a
    || codepoint == 0x000b
    || codepoint == 0x000c
    || codepoint == 0x000d
    || codepoint == 0x0020
    || codepoint == 0x0085
    || codepoint == 0x200E
    || codepoint == 0x200F
    || codepoint == 0x2028
    || codepoint == 0x2029;
}
