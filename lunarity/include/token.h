#ifndef _LUNARITY_TOKEN_H_
#define _LUNARITY_TOKEN_H_

#include "location.h"

typedef enum lunarity_token_kind
{
  LUNARITY_TOKEN_KIND_EOF,
  LUNARITY_TOKEN_KIND_EQUAL,
  LUNARITY_TOKEN_KIND_LESS_EQUAL,
  LUNARITY_TOKEN_KIND_GREATER_EQUAL,
  LUNARITY_TOKEN_KIND_DOUBLE_DOT,
  LUNARITY_TOKEN_KIND_TRIPLE_DOT,
  LUNARITY_TOKEN_KIND_SKINNY_ARROW,
  LUNARITY_TOKEN_KIND_DOUBLE_ARROW,
  LUNARITY_TOKEN_KIND_FLOOR_DIV,

  LUNARITY_TOKEN_KIND_ADD_ASSIGN,
  LUNARITY_TOKEN_KIND_SUB_ASSIGN,
  LUNARITY_TOKEN_KIND_MUL_ASSIGN,
  LUNARITY_TOKEN_KIND_DIV_ASSIGN,
  LUNARITY_TOKEN_KIND_FLOOR_DIV_ASSIGN,
  LUNARITY_TOKEN_MOD_ASSIGN,
  LUNARITY_TOKEN_POW_ASSIGN,
  LUNARITY_TOKEN_CONCAT_ASSIGN
} lunarity_token_kind_t;

typedef struct lunarity_token
{
  lunarity_token_kind_t kind;
  lunarity_span_t span;
  const char *data;
} lunarity_token_t;

/**
 * Create a new token
 * @param   kind The kind of the token
 * @param   span The location of the token
 * @return  The new token
 * @version 0.1.0
 */
lunarity_token_t lunarity_new_token(
    lunarity_token_kind_t kind,
    lunarity_span_t span);

/**
 * Create a new token with data
 * @param   kind The kind of the token
 * @param   span The location of the token
 * @param   data The string data that the token contains
 * @return  The new token
 * @version 0.1.0
 */
lunarity_token_t lunarity_new_token_with_data(
    lunarity_token_kind_t kind,
    lunarity_span_t span,
    const char *data);

#endif /* _LUNARITY_TOKEN_H_ */
