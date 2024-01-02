#ifndef _ARTY_UNICODE_H_
#define _ARTY_UNICODE_H_

#include "macros.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  Invalid Unicode codepoint (means that the codepoint couldn't be decoded).
 */
#define INVALID_CODEPOINT -1

/**
 * @brief  No Unicode codepoint (means that there is no more codepoints to be decoded in the UTF8
 * string iterator).
 *
 * @see   arty_utf8_string_iterator_t
 */
#define NO_CODEPOINT -2

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

/**
 * @param  codepoint The codepoint to check.
 * @return `true` if the codepoint is in the XID_Start range, `false` otherwise.
 */
bool arty_is_xid_start(arty_codepoint_t codepoint);

/**
 * @param  codepoint The codepoint to check.
 * @return `true` if the codepoint is in the XID_Continue range, `false` otherwise.
 */
bool arty_is_xid_continue(arty_codepoint_t codepoint);

#ifdef __cplusplus
}
#endif

#endif /* _ARTY_UNICODE_H_ */
