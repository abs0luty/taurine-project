#ifndef _ARTY_UNICODE_H_
#define _ARTY_UNICODE_H_

#include <stdbool.h>

typedef unsigned int arty_codepoint_t;
typedef unsigned long long arty_codepoint_size_t;

/**
 * @param  codepoint The codepoint to check.
 * @return `true` if the codepoint is valid, `false` otherwise.
 */
bool arty_is_valid_unicode_codepoint(arty_codepoint_t codepoint);

#endif /* _ARTY_UNICODE_H_ */
