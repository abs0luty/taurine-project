#ifndef _LUNARITY_AST_H_
#define _LUNARITY_AST_H_

#include "vec/include/vec.h"
#include "lunarity/include/token.h"
#include <stdbool.h>

typedef struct lunarity_prefix_expression {
  enum {
    LUNARITY_PREFIX_EXPRESSION_KIND_VAR,
    LUNARITY_PREFIX_EXPRESSION_KIND_CALL,
    LUNARITY_PREFIX_EXPRESSION_KIND_PARENTHESIZED
  } kind;
  union {
  } data;
} lunarity_prefix_expression_t;

typedef struct lunarity_boolean_literal_expression {
  bool value;
  lunarity_span_t span;
} lunarity_boolean_literal_expression_t;

typedef struct lunarity_string_literal_expression {
  char *value;
  lunarity_span_t span;
} lunarity_string_literal_expression_t;

typedef struct lunarity_binary_expression {
  struct lunarity_expression *left;
  lunarity_token_t op;
  struct lunarity_expression *right;
} lunarity_binary_expression_t;

typedef struct lunarity_unary_expression {
  lunarity_token_t op;
  struct lunarity_expression *right;
} lunarity_unary_expression_t;

typedef struct lunarity_nil_expression {
  lunarity_span_t span;
} lunarity_nil_expression_t;

typedef struct lunarity_expression {
  enum {
    LUNARITY_EXPRESSION_KIND_PREFIX,
    LUNARITY_EXPRESSION_KIND_BOOLEAN,
    LUNARITY_EXPRESSION_KIND_NIL,
    LUNARITY_EXPRESSION_KIND_NUMERAL,
    LUNARITY_EXPRESSION_KIND_STRING_LITERAL,
    LUNARITY_EXPRESSION_KIND_BINARY,
    LUNARITY_EXPRESSION_KIND_UNARY,
    LUNARITY_EXPRESSION_KIND_VARARG,
    LUNARITY_EXPRESSION_KIND_TABLE_CONSTRUCTOR,
    LUNARITY_EXPRESSION_KIND_FUNCTION_DEF
  } kind;
  union {
    lunarity_nil_expression_t nil;
    lunarity_prefix_expression_t prefix;
    lunarity_boolean_literal_expression_t boolean;
    lunarity_string_literal_expression_t string;
    lunarity_binary_expression_t binary;
    lunarity_unary_expression_t unary;
  } data;
} lunarity_expression_t;

typedef struct lunarity_assignment_statement {

} lunarity_assignment_statement_t;

typedef struct lunarity_statement {

} lunarity_statement_t;

typedef struct lunarity_block {
  vec(lunarity_statement_t) statements;
} lunarity_block_t;

/**
 * @returns An empty statement's block.
 * @version 0.1.0
 */
lunarity_block_t lunarity_new_block();

/**
 * @brief Adds a statement into the statement's block.
 * @version 0.1.0
 */
void lunarity_add_statement(lunarity_block_t *block,
                            lunarity_statement_t *statement);

/**
 * @brief   Frees a statement's block
 * @version 0.1.0
 */
void lunarity_free_block(lunarity_block_t *block);

/**
 * @brief   Lua's unit of compilation.
 * @version 0.1.0
 */
typedef lunarity_block_t lunarity_chunk_t;

#endif /* _LUNARITY_AST_H_ */
