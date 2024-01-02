#include "unicode.h"

/**
 * @brief  The replacement character (U+FFFD).
 */
#define REPLACEMENT_CHARACTER 0xFFFD

/**
 * @brief  Returns the size of the given codepoint in UTF8 bytes.
 * @param  codepoint The codepoint to get the size of.
 * @return The size of the codepoint in bytes if codepoint is valid
 *         and `0` otherwise.
 */
arty_codepoint_size_t arty_utf8_bytes_in_codepoint(arty_codepoint_t codepoint);

/**
 * @brief  Returns the size of the given codepoint in UTF8 bytes.
 * @param  lead The leading byte of the codepoint in UTF8.
 * @return The potential size of the codepoint in bytes.
 */
arty_codepoint_size_t arty_utf8_bytes_in_codepoint_by_leading_byte(unsigned char lead);

/**
 * @brief      Encodes the given codepoint in UTF8 bytes.
 *             Buffer isn't touched if the codepoint isn't valid.
 *             Buffer is maximally 4 bytes long and isn't null-terminated.
 *             Size of the buffer can be calculated with #arty_codepoint_size_in_utf8.
 * @param      codepoint The codepoint to encode.
 * @param[out] dst The buffer where the encoded codepoint will be written.
 */
void arty_encode_codepoint_in_utf8(arty_codepoint_t codepoint, char *dst);

/**
 * @brief      Encodes the given codepoint in UTF8 bytes.
 *             Buffer isn't touched if the codepoint isn't valid.
 *             Buffer is maximally 4 bytes long and is null-terminated.
 *             Size of the buffer can be calculated with #arty_codepoint_size_in_utf8.
 * @param      codepoint The codepoint to encode.
 * @param[out] dst The buffer where the encoded codepoint will be written.
 */
void arty_encode_codepoint_in_utf8_to_null_terminated_string(arty_codepoint_t codepoint, char *dst);

/**
 * @brief      Decodes the given UTF8 bytes into the codepoint.
 * @param      src The UTF8 bytes to decode.
 * @return     The codepoint decoded from the UTF8 bytes.
 *             `0` if the codepoint isn't valid.
 */
arty_codepoint_t arty_decode_codepoint_from_utf8(const char *src);
