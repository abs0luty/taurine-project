#include "unicode.h"

bool arty_is_valid_unicode_codepoint(arty_codepoint_t codepoint)
{
    return codepoint < 0x110000 && (codepoint - 0xd800 > 0x07ff);
}
