#include "utf8.h"

arty_codepoint_size_t arty_utf8_bytes_in_codepoint(arty_codepoint_t codepoint)
{
    if (codepoint < 0x00)
    {
        return 0;
    }
    else if (codepoint < 0x80)
    {
        return 1;
    }
    else if (codepoint < 0x800)
    {
        return 2;
    }
    else if (codepoint < 0x10000)
    {
        return 3;
    }
    else if (codepoint < 0x110000)
    {
        return 4;
    }
    else
    {
        return 0;
    }
}

arty_codepoint_size_t arty_utf8_bytes_in_codepoint_by_leading_byte(unsigned char lead)
{
    if (lead < 0x80)
    {
        return 1;
    }
    else if ((lead & 0xE0) == 0xC0)
    {
        return 2;
    }
    else if ((lead & 0xF0) == 0xE0)
    {
        return 3;
    }
    else if ((lead & 0xF8) == 0xF0)
    {
        return 4;
    }
    else
    {
        return 0;
    }
}

bool arty_encode_codepoint_in_utf8(arty_codepoint_t codepoint, char *dst)
{
    if (dst == _ARTY_NULLPTR || codepoint < 0x00)
    {
        return false;
    }

    if (codepoint < 0x80)
    {
        dst[0] = codepoint;
    }
    else if (codepoint < 0x800)
    {
        dst[0] = (codepoint >> 6) + 0xc0;
        dst[1] = (codepoint & 0x3f) + 0x80;
    }
    else if (codepoint < 0x10000)
    {
        dst[0] = (codepoint >> 12) + 0xe0;
        dst[1] = ((codepoint >> 6) & 0x3f) + 0x80;
        dst[2] = (codepoint & 0x3f) + 0x80;
    }
    else if (codepoint < 0x110000)
    {
        dst[0] = (codepoint >> 18) + 0xf0;
        dst[1] = ((codepoint >> 12) & 0x3f) + 0x80;
        dst[2] = ((codepoint >> 6) & 0x3f) + 0x80;
        dst[3] = (codepoint & 0x3f) + 0x80;
    }
    else
    {
        return false;
    }

    return true;
}

bool arty_encode_codepoint_in_utf8_to_null_terminated_string(arty_codepoint_t codepoint, char *dst)
{
    if (!arty_encode_codepoint_in_utf8(codepoint, dst)) {
        return false;
    }

    dst[arty_utf8_bytes_in_codepoint(codepoint)] = 0;
    return true;
}

arty_codepoint_t arty_decode_codepoint_from_utf8(const char *src)
{
    if (src == _ARTY_NULLPTR || *src == '\0')
    {
        return 0;
    }

    // The leading byte.
    unsigned char lead = *src;

    // The number of bytes in the codepoint.
    arty_codepoint_size_t size = arty_utf8_bytes_in_codepoint_by_leading_byte(lead);

    if (size == 0)
    {
        return INVALID_CODEPOINT;
    }

    // The codepoint.
    arty_codepoint_t codepoint = lead & (0xFF >> size);

    // Extract continuation bytes and form the codepoint.
    for (arty_codepoint_size_t i = 1; i < size; i++)
    {
        if ((src[i] & 0xC0) != 0x80)
        {
            return INVALID_CODEPOINT;
        }

        codepoint = (codepoint << 6) | (src[i] & 0x3F);
    }

    return codepoint;
}
