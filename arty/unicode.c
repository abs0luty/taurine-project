#include "unicode.h"

bool arty_is_valid_unicode_codepoint(arty_codepoint_t codepoint)
{
    return codepoint < 0x110000 && (codepoint - 0xd800 > 0x07ff);
}

arty_codepoint_size_t arty_codepoint_size(arty_codepoint_t codepoint)
{
    if (codepoint < 0x80) {
        return 1;
    } else if (codepoint < 0x800) {
        return 2;
    } else if (codepoint < 0x10000) {
        return 3;
    } else if (codepoint < 0x110000) {
        return 4;
    } else {
        return 0;
    }
}
