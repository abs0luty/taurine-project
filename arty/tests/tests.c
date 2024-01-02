#include "utf8.h"
#include "unicode.h"
#include "tau.h"

TEST(arty_tests, arty_utf8_bytes_in_codepoint)
{
  CHECK_EQ(arty_utf8_bytes_in_codepoint(0x1D11E), 4);
  CHECK_EQ(arty_utf8_bytes_in_codepoint(0x110000), 0);
  CHECK_EQ(arty_utf8_bytes_in_codepoint(0x39), 1);
  CHECK_EQ(arty_utf8_bytes_in_codepoint(0x041F), 2);
}

TEST(arty_tests, arty_is_valid_unicode_codepoint) {
  CHECK(!arty_is_valid_unicode_codepoint(0x110000));
  CHECK(arty_is_valid_unicode_codepoint(0x041F));
}

TEST(arty_tests, arty_decode_codepoint_from_utf8) {
  arty_codepoint_t codepoint = arty_decode_codepoint_from_utf8("П");
  CHECK_EQ(codepoint, 0x041F);

  codepoint = arty_decode_codepoint_from_utf8("p");
  CHECK_EQ(codepoint, 0x0070);
}

TEST(arty_tests, arty_utf8_bytes_in_codepoint_by_leading_byte) {
  const char* cyrillic_p = "П";
  const char* p = "p";

  CHECK_EQ(arty_utf8_bytes_in_codepoint_by_leading_byte(cyrillic_p[0]), 2);
  CHECK_EQ(arty_utf8_bytes_in_codepoint_by_leading_byte(p[0]), 1);
}

TEST(arty_tests, arty_encode_codepoint_in_utf8_to_null_terminated_string) {
  char dst[3];
  arty_codepoint_t codepoint = 0x041F;
  arty_encode_codepoint_in_utf8_to_null_terminated_string(codepoint, dst);
  CHECK_STREQ(dst, "П");
}

TEST(arty_tests, arty_encode_codepoint_in_utf8) {
  char dst[2];
  arty_codepoint_t codepoint = 0x041F;
  arty_encode_codepoint_in_utf8(codepoint, dst);
  CHECK_STREQ(dst, "П");
}

TAU_MAIN()
