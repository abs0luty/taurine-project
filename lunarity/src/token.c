#include "lunarity/token.h"

lunarity_token_t lunarity_new_token(lunarity_token_kind_t kind,
                                    lunarity_span_t span) {
  lunarity_token_t token;
  token.kind = kind;
  token.span = span;
  return token;
}

lunarity_token_t lunarity_new_token_with_string_data(lunarity_token_kind_t kind,
                                                     lunarity_span_t span,
                                                     const char *data) {
  lunarity_token_t token;
  token.kind = kind;
  token.span = span;
  token.string = data;
  return token;
}
