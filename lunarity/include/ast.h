#ifndef _LUNARITY_AST_H_
#define _LUNARITY_AST_H_

typedef struct lunarity_assignment_statement {
  
} lunarity_assignment_statement_t;

typedef struct lunarity_statement {

} lunarity_statement_t;

typedef struct lunarity_block {
  lunarity_statement_t *statements;
} lunarity_block_t;

lunarity_block_t lunarity_new_block();

void lunarity_add_statement(
  lunarity_block_t *block,
  lunarity_statement_t *statement);

/**
 * Frees a statement's block
 */
void lunarity_free_block(lunarity_block_t *block);

/**
 * A chunk is a Lua's unit of compilation.
 */
typedef lunarity_block_t lunarity_chunk_t;

#endif /* _LUNARITY_AST_H_ */
