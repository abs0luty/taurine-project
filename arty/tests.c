#include "utf8.h"
#include "unicode.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

int main()
{
    assert(arty_utf8_bytes_in_codepoint(0x1D11E) == 4);
    assert(!arty_is_valid_unicode_codepoint(0x110000));

    arty_codepoint_t codepoint = arty_decode_codepoint_from_utf8("П");
    arty_codepoint_size_t size = arty_utf8_bytes_in_codepoint(codepoint);
    assert(codepoint == 0x041F);
    assert(size == 2);

    char dst[3];
    arty_encode_codepoint_in_utf8_to_null_terminated_string(codepoint, dst);
    assert(strcmp(dst, "П") == 0);

    return 0;
}
