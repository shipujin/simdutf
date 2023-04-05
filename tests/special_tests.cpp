/**
 * Special tests. Specific cases.
 */
#include "simdutf.h"

#include <memory>
#include <tests/helpers/test.h>

TEST(special_cases_utf8_utf16le_roundtrip) {
  std::string cases[] = {
      "\x05\x0A\x0A\x01\x0C\x01\x01\x0A\x0C\x01\x01\x01\x01\x01\x0A\x0A\x0A\xF0"
      "\x93\x93\x93\x01\x01\x01\x01\xE2\xBB\x9A\xEF\x9B\xBB\xEE\x81\x81\x05\x2D"
      "\x01\x7B\x01\x01\xE2\xBB\x9A\xEF\x9B\xBB\xEE\x81\x81\x05\x2D\x01\x7B\x01"
      "\x01\x01\x01\x0A\x01\x2A\x0A\x7E\x0A\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01",
      "\x20\x20\xEF\xBB\x8A\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
      "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20",
      "hello\xe4\xbd\xa0\xe5\xa5\xbd"};
  for (const std::string& source : cases) {
    bool validutf8 = simdutf::validate_utf8(source.c_str(), source.size());
    ASSERT_TRUE(validutf8);
    // We need a buffer of size where to write the UTF-16LE words.
    size_t expected_utf16words =
        simdutf::utf16_length_from_utf8(source.c_str(), source.size());
    std::unique_ptr<char16_t[]> utf16_output{new char16_t[expected_utf16words]};
    // convert to UTF-16LE
    size_t utf16words = simdutf::convert_utf8_to_utf16le(
        source.c_str(), source.size(), utf16_output.get());
    // It wrote utf16words * sizeof(char16_t) bytes.
    bool validutf16 = simdutf::validate_utf16le(utf16_output.get(), utf16words);
    ASSERT_TRUE(validutf16);

    std::unique_ptr<char16_t[]> utf16_valid_output{new char16_t[expected_utf16words]};
    // convert to UTF-16LE
    size_t utf16words_valid = simdutf::convert_valid_utf8_to_utf16le(
        source.c_str(), source.size(), utf16_valid_output.get());
    ASSERT_TRUE(utf16words_valid == utf16words);
    for(size_t z = 0; z < utf16words_valid; z++) {
      ASSERT_TRUE(utf16_valid_output.get()[z] ==  utf16_output.get()[z]);
    }

    // convert it back:
    // We need a buffer of size where to write the UTF-8 words.
    size_t expected_utf8words =
        simdutf::utf8_length_from_utf16le(utf16_output.get(), utf16words);
    ASSERT_TRUE(expected_utf8words == source.size());
    std::unique_ptr<char[]> utf8_output{new char[expected_utf8words]};
    // convert to UTF-8
    size_t utf8words = simdutf::convert_utf16le_to_utf8(
        utf16_output.get(), utf16words, utf8_output.get());
    ASSERT_TRUE(expected_utf8words == utf8words);
    std::string final_string(utf8_output.get(), utf8words);
    ASSERT_TRUE(final_string == source);

    size_t utf8words_valid = simdutf::convert_valid_utf16le_to_utf8(
        utf16_output.get(), utf16words, utf8_output.get());
    ASSERT_TRUE(expected_utf8words == utf8words_valid);
    std::string final_string_valid(utf8_output.get(), utf8words_valid);
    ASSERT_TRUE(final_string_valid == source);

  }
}


TEST(special_cases_utf8_utf16be_roundtrip) {
  std::string cases[] = {
      "\x05\x0A\x0A\x01\x0C\x01\x01\x0A\x0C\x01\x01\x01\x01\x01\x0A\x0A\x0A\xF0"
      "\x93\x93\x93\x01\x01\x01\x01\xE2\xBB\x9A\xEF\x9B\xBB\xEE\x81\x81\x05\x2D"
      "\x01\x7B\x01\x01\xE2\xBB\x9A\xEF\x9B\xBB\xEE\x81\x81\x05\x2D\x01\x7B\x01"
      "\x01\x01\x01\x0A\x01\x2A\x0A\x7E\x0A\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01",
      "\x20\x20\xEF\xBB\x8A\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
      "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20",
      "hello\xe4\xbd\xa0\xe5\xa5\xbd"};
  for (const std::string& source : cases) {
    bool validutf8 = simdutf::validate_utf8(source.c_str(), source.size());
    ASSERT_TRUE(validutf8);
    // We need a buffer of size where to write the UTF-16LE words.
    size_t expected_utf16words =
        simdutf::utf16_length_from_utf8(source.c_str(), source.size());
    std::unique_ptr<char16_t[]> utf16_output{new char16_t[expected_utf16words]};
    // convert to UTF-16BE
    size_t utf16words = simdutf::convert_utf8_to_utf16be(
        source.c_str(), source.size(), utf16_output.get());
    // It wrote utf16words * sizeof(char16_t) bytes.
    bool validutf16 = simdutf::validate_utf16be(utf16_output.get(), utf16words);
    ASSERT_TRUE(validutf16);

    std::unique_ptr<char16_t[]> utf16_valid_output{new char16_t[expected_utf16words]};
    // convert to UTF-16BE
    size_t utf16words_valid = simdutf::convert_valid_utf8_to_utf16be(
        source.c_str(), source.size(), utf16_valid_output.get());
    ASSERT_TRUE(utf16words_valid == utf16words);
    for(size_t z = 0; z < utf16words_valid; z++) {
      ASSERT_TRUE(utf16_valid_output.get()[z] ==  utf16_output.get()[z]);
    }

    // convert it back:
    // We need a buffer of size where to write the UTF-8 words.
    size_t expected_utf8words =
        simdutf::utf8_length_from_utf16be(utf16_output.get(), utf16words);
    ASSERT_TRUE(expected_utf8words == source.size());
    std::unique_ptr<char[]> utf8_output{new char[expected_utf8words]};
    // convert to UTF-8
    size_t utf8words = simdutf::convert_utf16be_to_utf8(
        utf16_output.get(), utf16words, utf8_output.get());
    ASSERT_TRUE(expected_utf8words == utf8words);
    std::string final_string(utf8_output.get(), utf8words);
    ASSERT_TRUE(final_string == source);

    size_t utf8words_valid = simdutf::convert_valid_utf16be_to_utf8(
        utf16_output.get(), utf16words, utf8_output.get());
    ASSERT_TRUE(expected_utf8words == utf8words_valid);
    std::string final_string_valid(utf8_output.get(), utf8words_valid);
    ASSERT_TRUE(final_string_valid == source);

  }
}


TEST(special_cases_utf8_utf16_roundtrip) {
  std::string cases[] = {
      "\x05\x0A\x0A\x01\x0C\x01\x01\x0A\x0C\x01\x01\x01\x01\x01\x0A\x0A\x0A\xF0"
      "\x93\x93\x93\x01\x01\x01\x01\xE2\xBB\x9A\xEF\x9B\xBB\xEE\x81\x81\x05\x2D"
      "\x01\x7B\x01\x01\xE2\xBB\x9A\xEF\x9B\xBB\xEE\x81\x81\x05\x2D\x01\x7B\x01"
      "\x01\x01\x01\x0A\x01\x2A\x0A\x7E\x0A\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01",
      "\x20\x20\xEF\xBB\x8A\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
      "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20",
      "hello\xe4\xbd\xa0\xe5\xa5\xbd"};
  for (const std::string& source : cases) {
    bool validutf8 = simdutf::validate_utf8(source.c_str(), source.size());
    ASSERT_TRUE(validutf8);
    // We need a buffer of size where to write the UTF-16LE words.
    size_t expected_utf16words =
        simdutf::utf16_length_from_utf8(source.c_str(), source.size());
    std::unique_ptr<char16_t[]> utf16_output{new char16_t[expected_utf16words]};
    // convert to UTF-16
    size_t utf16words = simdutf::convert_utf8_to_utf16(
        source.c_str(), source.size(), utf16_output.get());
    // It wrote utf16words * sizeof(char16_t) bytes.
    bool validutf16 = simdutf::validate_utf16(utf16_output.get(), utf16words);
    ASSERT_TRUE(validutf16);

    std::unique_ptr<char16_t[]> utf16_valid_output{new char16_t[expected_utf16words]};
    // convert to UTF-16
    size_t utf16words_valid = simdutf::convert_valid_utf8_to_utf16(
        source.c_str(), source.size(), utf16_valid_output.get());
    ASSERT_TRUE(utf16words_valid == utf16words);
    for(size_t z = 0; z < utf16words_valid; z++) {
      ASSERT_TRUE(utf16_valid_output.get()[z] ==  utf16_output.get()[z]);
    }

    // convert it back:
    // We need a buffer of size where to write the UTF-8 words.
    size_t expected_utf8words =
        simdutf::utf8_length_from_utf16(utf16_output.get(), utf16words);
    ASSERT_TRUE(expected_utf8words == source.size());
    std::unique_ptr<char[]> utf8_output{new char[expected_utf8words]};
    // convert to UTF-8
    size_t utf8words = simdutf::convert_utf16_to_utf8(
        utf16_output.get(), utf16words, utf8_output.get());
    ASSERT_TRUE(expected_utf8words == utf8words);
    std::string final_string(utf8_output.get(), utf8words);
    ASSERT_TRUE(final_string == source);
    size_t utf8words_valid = simdutf::convert_valid_utf16_to_utf8(
        utf16_output.get(), utf16words, utf8_output.get());
    ASSERT_TRUE(expected_utf8words == utf8words_valid);
    std::string final_string_valid(utf8_output.get(), utf8words_valid);
    ASSERT_TRUE(final_string_valid == source);
  }
}

TEST(special_cases_utf8_utf16_invalid) {
  std::string cases[] = {
      "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01"
      "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05"
      "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x05\x01\x01\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe"
      "\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe"
      "\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe"
      "\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x05\x01\x01\x01\x01\x01\x01\x05\xe2\x81\x9f\xf0\x93\x93\x93\xf0\x9d\x93"
      "\x97\xf0\x93\x93\x93\xf0\x93\x91\x83\x6c",
      "\x05\x01\x0a\xf0\x91\x81\x80\x01\x26\x0a\x0a\x0a\x26\x0a\x0a\x0a\xf0\x91"
      "\x81\x80\x01\x05\x7b\x01\x17\x01\x01\x01\x01\x01\x01\x01\x26\x01\x0a\x0a"
      "\x01\x01\x0a\x11\x2a\x01\x01\x26\x0d\xad\xad\xad\xad\xad\xad\xad\xad\xad"
      "\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad"
      "\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad"
      "\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad"
      "\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad"
      "\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad"
      "\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xd3\x96\x2c\x2d\x2c\x01"
      "\x5b\x2d\x26\xc3\x8a\xf2\x93\x91\x91\xf3\x93\x93\x93\x26\x0d\xd3\x96\x2c"
      "\x2d\x2c\x01\x0a\x01\x01\x01\x24\xf0\x93\x97\x93\xf0\x93\x9d\x89\x0a\x01"
      "\xd3\x8a\x01\x01\x01\x01\x01\xf0\x93\x97\x93\xf0\x93\x9d\x89\x0a\x01\x01"
      "\x01\x24\x01\x01\x01\x01\xed\x9f\xb9\x0a\x0a\x01\x34\x01\x01\x01\x01\x01"
      "\x09\x01\x01\x01\x01\x01\x01\x01\x0f\x1f\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\xed\x9f\xb9\x25\xef\x93\x9f\xe8\x9f\xbf\x01\x0a\x41\xf3\x93"
      "\x93\x93\xf0\x93\x8f\x8f\x74\x01\x01\xf0\x93\x8f\xaf\xf3\x97\x91\x91\xf3"
      "\x93\x93\x93\xf0\x93\x8f\x8f\xf3\x93\x91\x91\x24\x01\x01\x6d\x01\x01\x01"
      "\x57\x57\x57\x57\x32\x37\x37\x37\x37\x37\x37\x37\x37\x37\x37\x37\x37\x37"
      "\x37\x37\x37\x37\x57\x57\x57\x57\x57\x57\x57\x57\x57\x57\x57\x57\x57\x57"
      "\x57\x57\x57\x57\x57\x57\x56\x57\x57\xb0\xa8\xa8\xa8\x57\x57\x57\x57\x57"
      "\x57\x57\x2c\x57\x57\x57\x57\x57\x57\x57\xff\xff\xff",
      "\x20\x20\x20\x20\x20\x81\x20\xbf\xbf\x20\x20\x20\x20\xbf\x20\xb9\x83\x20"
      "\x20\x20\x20\x20\x20\x20\x20\xa6\xa6\xa6\x20\x20\x20\x20\x20\x20\x20\x20"
      "\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6"
      "\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6"
      "\xa6\xa6\xa6\xa6\xa6\xa6\x20\x20",
      "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xff\x20\x20\x20\x93\x20"
      "\x20\x81\x83\x20\x20\x20\x20\x20\x20\x20\x20\x20\xba\xba\xba\xa2\xa2\xa2"
      "\xa2\xa2\xba\xba\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xba"
      "\xba\xba\xba\xa2\xa2\xba\xba\xba\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2"
      "\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2"};
  for (const std::string& source : cases) {
    bool validutf8 = simdutf::validate_utf8(source.c_str(), source.size());
    ASSERT_TRUE(!validutf8);

    size_t expected_utf16words =
        simdutf::utf16_length_from_utf8(source.c_str(), source.size());
    std::unique_ptr<char16_t[]> utf16_output{new char16_t[expected_utf16words]};
    // convert to UTF-16LE, this will fail!!! (on purpose)
    size_t utf16words = simdutf::convert_utf8_to_utf16le(
        source.c_str(), source.size(), utf16_output.get());
    ASSERT_TRUE(utf16words == 0);
  }
}

TEST(special_cases_utf8_utf32_invalid) {
  std::string cases[] = {
      "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01"
      "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05"
      "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x01\x05\x01\x01\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe"
      "\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe"
      "\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe"
      "\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\xbe\x01\x01\x01\x01\x01"
      "\x01\x01\x01\x01\x01\x05\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x05\x01\x01\x01\x01\x01\x01\x05\xe2\x81\x9f\xf0\x93\x93\x93\xf0\x9d\x93"
      "\x97\xf0\x93\x93\x93\xf0\x93\x91\x83\x6c",
      "\x05\x01\x0a\xf0\x91\x81\x80\x01\x26\x0a\x0a\x0a\x26\x0a\x0a\x0a\xf0\x91"
      "\x81\x80\x01\x05\x7b\x01\x17\x01\x01\x01\x01\x01\x01\x01\x26\x01\x0a\x0a"
      "\x01\x01\x0a\x11\x2a\x01\x01\x26\x0d\xad\xad\xad\xad\xad\xad\xad\xad\xad"
      "\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad"
      "\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad"
      "\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad"
      "\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad"
      "\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad"
      "\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xad\xd3\x96\x2c\x2d\x2c\x01"
      "\x5b\x2d\x26\xc3\x8a\xf2\x93\x91\x91\xf3\x93\x93\x93\x26\x0d\xd3\x96\x2c"
      "\x2d\x2c\x01\x0a\x01\x01\x01\x24\xf0\x93\x97\x93\xf0\x93\x9d\x89\x0a\x01"
      "\xd3\x8a\x01\x01\x01\x01\x01\xf0\x93\x97\x93\xf0\x93\x9d\x89\x0a\x01\x01"
      "\x01\x24\x01\x01\x01\x01\xed\x9f\xb9\x0a\x0a\x01\x34\x01\x01\x01\x01\x01"
      "\x09\x01\x01\x01\x01\x01\x01\x01\x0f\x1f\x01\x01\x01\x01\x01\x01\x01\x01"
      "\x01\x01\x01\xed\x9f\xb9\x25\xef\x93\x9f\xe8\x9f\xbf\x01\x0a\x41\xf3\x93"
      "\x93\x93\xf0\x93\x8f\x8f\x74\x01\x01\xf0\x93\x8f\xaf\xf3\x97\x91\x91\xf3"
      "\x93\x93\x93\xf0\x93\x8f\x8f\xf3\x93\x91\x91\x24\x01\x01\x6d\x01\x01\x01"
      "\x57\x57\x57\x57\x32\x37\x37\x37\x37\x37\x37\x37\x37\x37\x37\x37\x37\x37"
      "\x37\x37\x37\x37\x57\x57\x57\x57\x57\x57\x57\x57\x57\x57\x57\x57\x57\x57"
      "\x57\x57\x57\x57\x57\x57\x56\x57\x57\xb0\xa8\xa8\xa8\x57\x57\x57\x57\x57"
      "\x57\x57\x2c\x57\x57\x57\x57\x57\x57\x57\xff\xff\xff",
      "\x20\x20\x20\x20\x20\x81\x20\xbf\xbf\x20\x20\x20\x20\xbf\x20\xb9\x83\x20"
      "\x20\x20\x20\x20\x20\x20\x20\xa6\xa6\xa6\x20\x20\x20\x20\x20\x20\x20\x20"
      "\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6"
      "\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6\xa6"
      "\xa6\xa6\xa6\xa6\xa6\xa6\x20\x20",
      "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\xff\x20\x20\x20\x93\x20"
      "\x20\x81\x83\x20\x20\x20\x20\x20\x20\x20\x20\x20\xba\xba\xba\xa2\xa2\xa2"
      "\xa2\xa2\xba\xba\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xba"
      "\xba\xba\xba\xa2\xa2\xba\xba\xba\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2"
      "\xa2\xa2\xa2\xa2\xa2\xa2\xa2\xa2"};
  for (const std::string& source : cases) {
    bool validutf8 = simdutf::validate_utf8(source.c_str(), source.size());
    ASSERT_TRUE(!validutf8);

    size_t expected_utf32words =
        simdutf::utf32_length_from_utf8(source.c_str(), source.size());
    std::unique_ptr<char32_t[]> utf32_output{new char32_t[expected_utf32words]};
    // convert to UTF-16LE, this will fail!!! (on purpose)
    size_t utf32words = simdutf::convert_utf8_to_utf32(
        source.c_str(), source.size(), utf32_output.get());
    ASSERT_TRUE(utf32words == 0);
  }
}
int main(int argc, char *argv[]) { return simdutf::test::main(argc, argv); }