#ifndef _ARTY_UNICODE_H_
#define _ARTY_UNICODE_H_

#include <stdbool.h>

/**
 * @brief  Invalid Unicode codepoint (means that the codepoint couldn't be decoded).
 */
#define INVALID_CODEPOINT -1

/**
 * The Unicode codepoint type.
 */
typedef int arty_codepoint_t;

/**
 * The type to denote the size of the codepoint in UTF8 bytes.
 */
typedef unsigned long long arty_codepoint_size_t;

/**
 * @param  codepoint The codepoint to check.
 * @return `true` if the codepoint is valid, `false` otherwise.
 */
bool arty_is_valid_unicode_codepoint(arty_codepoint_t codepoint);

#endif /* _ARTY_UNICODE_H_ */
