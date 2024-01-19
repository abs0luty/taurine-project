#ifndef _ARTY_UNICODE_H_
#define _ARTY_UNICODE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Invalid Unicode codepoint (means that the codepoint couldn't be
 * decoded).
 * @version 0.1.0
 */
#define INVALID_CODEPOINT -1

/**
 * @brief   No Unicode codepoint (means that there is no more codepoints to be
 * decoded in the UTF8 string iterator).
 * @version 0.1.0
 * @see     arty_utf8_string_iterator_t
 */
#define NO_CODEPOINT -2

/**
 * @brief   The Unicode codepoint type.
 * @version 0.1.0
 */
typedef int32_t arty_codepoint_t;

/**
 * @brief   The type to denote the size of the codepoint in UTF8 bytes.
 * @version 0.1.0
 */
typedef size_t arty_codepoint_size_t;

/**
 * @param   codepoint The codepoint to check.
 * @return  `true` if the codepoint is valid, `false` otherwise.
 * @version 0.1.0
 */
bool arty_is_valid_unicode_codepoint(arty_codepoint_t codepoint);

/**
 * @param   codepoint The codepoint to check.
 * @return  `true` if the codepoint is in the XID_Start range, `false`
 * otherwise.
 * @version 0.1.0
 */
bool arty_is_xid_start(arty_codepoint_t codepoint);

/**
 * @param   codepoint The codepoint to check.
 * @return  `true` if the codepoint is in the XID_Continue range, `false`
 * otherwise.
 * @version 0.1.0
 */
bool arty_is_xid_continue(arty_codepoint_t codepoint);

#ifdef __cplusplus
}
#endif

#endif /* _ARTY_UNICODE_H_ */
