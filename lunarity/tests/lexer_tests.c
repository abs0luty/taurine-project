#include "lunarity/include/lexer.h"
#include "tau/tau.h"

/**
 * @brief   Initializes a lexer state from a given Lunarity source code.
 * @param   source The source code.
 * @param   length The length of the source code in bytes.
 * @version 0.1.0
 */
#define START_LEXER_TEST(source, length)                                       \
  arty_utf8_string_iterator_t it;                                              \
  lunarity_lexer_state_t state;                                                \
  do {                                                                         \
    it = arty_new_utf8_string_iterator(source, length);                        \
    state = lunarity_new_lexer_state(it);                                      \
  } while (0)

/**
 * @brief   Advances the lexer state by one token and compares this token data
 *          with an expected values.
 * @param   token_kind Expected token kind.
 * @param   start_line Expected token span's first byte line number.
 * @param   start_column Expected token span's first byte column number.
 * @param   start_offset Expected token span's first byte offset.
 * @param   end_line Expected token span's last byte line number.
 * @param   end_column Expected token span's last byte column number.
 * @param   end_offset Expected token span's last byte offset.
 * @version 0.1.0
 */
#define CHECK_NEXT_TOKEN(token_kind, start_line, start_column, start_offset,   \
                         end_line, end_column, end_offset)                     \
  do {                                                                         \
    lunarity_token_t token = lunarity_lexer_state_next_token(&state);          \
    CHECK_EQ(token.kind, token_kind);                                          \
    CHECK_EQ(token.span.start.line, start_line);                               \
    CHECK_EQ(token.span.start.column, start_column);                           \
    CHECK_EQ(token.span.start.offset, start_offset);                           \
    CHECK_EQ(token.span.end.line, end_line);                                   \
    CHECK_EQ(token.span.end.column, end_column);                               \
    CHECK_EQ(token.span.end.offset, end_offset);                               \
  } while (0)

/**
 * @brief   Advances the lexer state by one token and compares this token data
 *          with an expected values.
 * @param   token_kind   Expected token kind.
 * @param   start_line   Expected token span's first byte line number.
 * @param   start_column Expected token span's first byte column number.
 * @param   start_offset Expected token span's first byte offset.
 * @param   end_line     Expected token span's last byte line number.
 * @param   end_column   Expected token span's last byte column number.
 * @param   end_offset   Expected token span's last byte offset.
 * @param   string_data  Expected token string data.
 * @version 0.1.0
 */
#define CHECK_NEXT_STRING_TOKEN(token_kind, start_line, start_column,          \
                                start_offset, end_line, end_column,            \
                                end_offset, string_data)                       \
  do {                                                                         \
    lunarity_token_t token = lunarity_lexer_state_next_token(&state);          \
    CHECK_EQ(token.kind, token_kind);                                          \
    CHECK_EQ(token.span.start.line, start_line);                               \
    CHECK_EQ(token.span.start.column, start_column);                           \
    CHECK_EQ(token.span.start.offset, start_offset);                           \
    CHECK_EQ(token.span.end.line, end_line);                                   \
    CHECK_EQ(token.span.end.column, end_column);                               \
    CHECK_EQ(token.span.end.offset, end_offset);                               \
    CHECK_STREQ(token.string, string_data);                                    \
  } while (0)

TEST(lexer_tests, eof) {
  START_LEXER_TEST("", 0);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_EOF, 1, 0, 0, 1, 1, 1);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_EOF, 1, 0, 0, 1, 1, 1);
}

TEST(lexer_tests, whitespaces) {
  START_LEXER_TEST("\n\t \r\u2029", 7);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_EOF, 2, 4, 7, 2, 5, 8);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_EOF, 2, 4, 7, 2, 5, 8);
}

TEST(lexer_tests, ascii_identifier) {
  START_LEXER_TEST("_test_29", 8);
  CHECK_NEXT_STRING_TOKEN(LUNARITY_TOKEN_KIND_IDENTIFIER, 1, 0, 0, 1, 8, 8,
                          "_test_29");
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_EOF, 1, 8, 8, 1, 9, 9);
}

TEST(lexer_tests, unicode_identifier) {
  START_LEXER_TEST("_тест_29", 12);
  CHECK_NEXT_STRING_TOKEN(LUNARITY_TOKEN_KIND_IDENTIFIER, 1, 0, 0, 1, 8, 12,
                          "_тест_29");
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_EOF, 1, 8, 12, 1, 9, 13);
}

TEST(lexer_tests, keyword) {
  START_LEXER_TEST("local", 5);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_KW_LOCAL, 1, 0, 0, 1, 5, 5);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_EOF, 1, 5, 5, 1, 6, 6);
}

TEST(lexer_tests, single_byte_punctuation) {
  START_LEXER_TEST("+-(){}[]", 8);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_PLUS, 1, 0, 0, 1, 1, 1);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_MINUS, 1, 1, 1, 1, 2, 2);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_LPAREN, 1, 2, 2, 1, 3, 3);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_RPAREN, 1, 3, 3, 1, 4, 4);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_LBRACE, 1, 4, 4, 1, 5, 5);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_RBRACE, 1, 5, 5, 1, 6, 6);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_LBRACKET, 1, 6, 6, 1, 7, 7);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_RBRACKET, 1, 7, 7, 1, 8, 8);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_EOF, 1, 8, 8, 1, 9, 9);
}

TEST(lexer_tests, double_byte_punctuation) {
  START_LEXER_TEST("== ~= ::", 8);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_DOUBLE_EQ, 1, 0, 0, 1, 2, 2);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_TILDE_EQ, 1, 3, 3, 1, 5, 5);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_DOUBLE_COLON, 1, 6, 6, 1, 8, 8);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_EOF, 1, 8, 8, 1, 9, 9);
}

TEST(lexer_tests, dots) {
  START_LEXER_TEST(". .. ...", 8);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_DOT, 1, 0, 0, 1, 1, 1);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_DOUBLE_DOT, 1, 2, 2, 1, 4, 4);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_TRIPLE_DOT, 1, 5, 5, 1, 8, 8);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_EOF, 1, 8, 8, 1, 9, 9);
}

TEST(lexer_tests, string) {
  START_LEXER_TEST("\"test\"", 6);
  CHECK_NEXT_STRING_TOKEN(LUNARITY_TOKEN_KIND_STRING, 1, 0, 0, 1, 6, 6, "test");
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_EOF, 1, 6, 6, 1, 7, 7);
}

TEST(lexer_tests, unexpected_character) {
  START_LEXER_TEST("`", 1);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_UNEXPECTED_CHARACTER, 1, 0, 0, 1, 1, 1);
  CHECK_NEXT_TOKEN(LUNARITY_TOKEN_KIND_EOF, 1, 1, 1, 1, 2, 2);
}

TAU_MAIN()
