#ifndef _LUNARITY_TOKEN_H_
#define _LUNARITY_TOKEN_H_

#include "location.h"

typedef enum lunarity_token_kind {
  LUNARITY_TOKEN_KIND_EOF,

  /* Keywords */
  LUNARITY_TOKEN_KIND_KW_AND,
  LUNARITY_TOKEN_KIND_KW_BREAK,
  LUNARITY_TOKEN_KIND_KW_DO,
  LUNARITY_TOKEN_KIND_KW_ELSE,
  LUNARITY_TOKEN_KIND_KW_ELSEIF,
  LUNARITY_TOKEN_KIND_KW_END,
  LUNARITY_TOKEN_KIND_KW_FALSE,
  LUNARITY_TOKEN_KIND_KW_FOR,
  LUNARITY_TOKEN_KIND_KW_FUNCTION,
  LUNARITY_TOKEN_KIND_KW_GOTO,
  LUNARITY_TOKEN_KIND_KW_IF,
  LUNARITY_TOKEN_KIND_KW_IN,
  LUNARITY_TOKEN_KIND_KW_LOCAL,
  LUNARITY_TOKEN_KIND_KW_NIL,
  LUNARITY_TOKEN_KIND_KW_NOT,
  LUNARITY_TOKEN_KIND_KW_OR,
  LUNARITY_TOKEN_KIND_KW_REPEAT,
  LUNARITY_TOKEN_KIND_KW_RETURN,
  LUNARITY_TOKEN_KIND_KW_THEN,
  LUNARITY_TOKEN_KIND_KW_TRUE,
  LUNARITY_TOKEN_KIND_KW_UNTIL,
  LUNARITY_TOKEN_KIND_KW_WHILE,

  /* Punctuation */
  LUNARITY_TOKEN_KIND_PLUS,
  LUNARITY_TOKEN_KIND_MINUS,
  LUNARITY_TOKEN_KIND_ASTERISK,
  LUNARITY_TOKEN_KIND_SLASH,
  LUNARITY_TOKEN_KIND_PERCENT,
  LUNARITY_TOKEN_KIND_CARET,
  LUNARITY_TOKEN_KIND_HASH,
  LUNARITY_TOKEN_KIND_AMPERSAND,
  LUNARITY_TOKEN_KIND_TILDE,
  LUNARITY_TOKEN_KIND_BAR,
  LUNARITY_TOKEN_KIND_LTLT,
  LUNARITY_TOKEN_KIND_GTGT,
  LUNARITY_TOKEN_KIND_DOUBLE_SLASH,
  LUNARITY_TOKEN_KIND_DOUBLE_EQ,
  LUNARITY_TOKEN_KIND_TILDE_EQ,
  LUNARITY_TOKEN_KIND_LE,
  LUNARITY_TOKEN_KIND_GE,
  LUNARITY_TOKEN_KIND_LT,
  LUNARITY_TOKEN_KIND_GT,
  LUNARITY_TOKEN_KIND_EQ,
  LUNARITY_TOKEN_KIND_LPAREN,
  LUNARITY_TOKEN_KIND_RPAREN,
  LUNARITY_TOKEN_KIND_LBRACE,
  LUNARITY_TOKEN_KIND_RBRACE,
  LUNARITY_TOKEN_KIND_LBRACKET,
  LUNARITY_TOKEN_KIND_RBRACKET,
  LUNARITY_TOKEN_KIND_DOUBLE_COLON,
  LUNARITY_TOKEN_KIND_COLON,
  LUNARITY_TOKEN_KIND_SEMICOLON,
  LUNARITY_TOKEN_KIND_COMMA,
  LUNARITY_TOKEN_KIND_DOT,
  LUNARITY_TOKEN_KIND_DOUBLE_DOT,
  LUNARITY_TOKEN_KIND_TRIPLE_DOT,

  LUNARITY_TOKEN_KIND_IDENTIFIER,
  LUNARITY_TOKEN_KIND_STRING,

  LUNARITY_TOKEN_KIND_UNTERMINATED_STRING_LITERAL,
} lunarity_token_kind_t;

typedef struct lunarity_token {
  lunarity_token_kind_t kind;
  lunarity_span_t span;
  union {
    double number;
    const char *string;
  };
} lunarity_token_t;

/**
 * Create a new token
 * @param   kind The kind of the token
 * @param   span The location of the token
 * @return  The new token
 * @version 0.1.0
 */
lunarity_token_t lunarity_new_token(lunarity_token_kind_t kind,
                                    lunarity_span_t span);

/**
 * Create a new token with data
 * @param   kind The kind of the token
 * @param   span The location of the token
 * @param   data The string data that the token contains
 * @return  The new token
 * @version 0.1.0
 */
lunarity_token_t lunarity_new_token_with_string_data(lunarity_token_kind_t kind,
                                                     lunarity_span_t span,
                                                     const char *data);

#endif /* _LUNARITY_TOKEN_H_ */
