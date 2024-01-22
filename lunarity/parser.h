#ifndef _LUNARITY_PARSER_H_
#define _LUNARITY_PARSER_H_

#include "lunarity/lexer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lunarity_parser {
  lunarity_lexer_state_t lexer_state;
  lunarity_token_t current, next;
} lunarity_parser_t;

lunarity_parser_t lunarity_new_parser(lunarity_lexer_state_t lexer_state);

#ifdef __cplusplus
}
#endif

#endif /* _LUNARITY_PARSER_H_ */
