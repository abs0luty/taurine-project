#include "../include/unicode.h"

bool arty_is_valid_unicode_codepoint(arty_codepoint_t codepoint)
{
    return (unsigned int) codepoint < 0x110000
        && (((unsigned int) codepoint) - 0xd800) > 0x07ff;
}
