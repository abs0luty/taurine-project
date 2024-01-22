#ifndef _LUNARITY_LEXER_H_
#define _LUNARITY_LEXER_H_

#include "arty/unicode.h"
#include "arty/utf8.h"
#include "location.h"
#include "token.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lunarity_lexer_state {
  arty_utf8_string_iterator_t it;
  arty_codepoint_t current, next;
  lunarity_byte_location_t cursor;
} lunarity_lexer_state_t;

lunarity_lexer_state_t lunarity_new_lexer_state(arty_utf8_string_iterator_t it);

lunarity_token_t lunarity_lexer_state_next_token(lunarity_lexer_state_t *state);

#ifdef __cplusplus
}
#endif

#endif /* _LUNARITY_LEXER_H_ */
