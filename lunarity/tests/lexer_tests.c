#include "lexer.h"
#include "tau.h"

TEST(lexer_tests, eof) {
  arty_utf8_string_iterator_t it = arty_new_utf8_string_iterator("", 0);
  lunarity_lexer_state_t state = lunarity_new_lexer_state(it);

  lunarity_token_t token = lunarity_next_token(&state);
  CHECK_EQ(token.kind, LUNARITY_TOKEN_KIND_EOF);
  CHECK_EQ(token.span.start.line, 1);
  CHECK_EQ(token.span.start.column, 0);
  CHECK_EQ(token.span.start.offset, 0);
  CHECK_EQ(token.span.end.line, 1);
  CHECK_EQ(token.span.end.column, 1);
  CHECK_EQ(token.span.end.offset, 1);

  token = lunarity_next_token(&state);
  CHECK_EQ(token.kind, LUNARITY_TOKEN_KIND_EOF);
  CHECK_EQ(token.span.start.line, 1);
  CHECK_EQ(token.span.start.column, 0);
  CHECK_EQ(token.span.start.offset, 0);
  CHECK_EQ(token.span.end.line, 1);
  CHECK_EQ(token.span.end.column, 1);
  CHECK_EQ(token.span.end.offset, 1);
}

TAU_MAIN()
