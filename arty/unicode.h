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

/**
 * @brief  Returns the size of the given codepoint in bytes.
 * @param  codepoint The codepoint to get the size of.
 * @return The size of the codepoint in bytes if codepoint is valid
 *         and `0` otherwise.
 */
arty_codepoint_size_t arty_codepoint_size(arty_codepoint_t codepoint);

#endif /* _ARTY_UNICODE_H_ */
