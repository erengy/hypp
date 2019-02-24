#pragma once

#include <array>
#include <string_view>

#include <hypp/detail/util.hpp>

namespace hypp::detail {

namespace syntax {

// Reference: https://tools.ietf.org/html/rfc3986#appendix-A
// Reference: https://tools.ietf.org/html/rfc5234#appendix-B.1
// Reference: https://tools.ietf.org/html/rfc7230#appendix-B

constexpr auto kCRLF = "\r\n";       // Carriage Return + Line Feed
constexpr auto kHTAB = '\t';         // Horizontal Tab
constexpr auto kHttpName = "HTTP";   // HTTP name (case-sensitive)
constexpr auto kSP = ' ';            // Space
constexpr auto kWhitespace = " \t";  // Whitespace (SP / HTAB)

}  // namespace syntax

constexpr bool is_alpha(const char c) {
  // ALPHA = %x41-5A / %x61-7A  ; A-Z / a-z
  return ('A' <= c && c <= 'Z') ||
         ('a' <= c && c <= 'z');
}

constexpr bool is_digit(const char c) {
  // DIGIT = %x30-39  ; 0-9
  return '0' <= c && c <= '9';
}

constexpr bool is_hex_digit(const char c) {
  // HEXDIG = DIGIT / "A" / "B" / "C" / "D" / "E" / "F"
  return ('0' <= c && c <= '9') ||
         ('A' <= c && c <= 'F') ||
         ('a' <= c && c <= 'f');
}

constexpr bool is_obs_text(const char c) {
  // obs-text = %x80-FF
  return 0x80 <= c && c <= 0xFF;
}

constexpr bool is_tchar(const char c) {
  // tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*"
  //       / "+" / "-" / "." / "^" / "_" / "`" / "|" / "~"
  //       / DIGIT / ALPHA
  //       ; any VCHAR, except delimiters
  constexpr std::array<char, 256> tchar{
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,  '!',    0,  '#',  '$',  '%',  '&', '\'',
      0,    0,  '*',  '+',    0,  '-',  '.',    0,
    '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
    '8',  '9',    0,    0,    0,    0,    0,    0,
      0,  'A',  'B',  'C',  'D',  'E',  'F',  'G',
    'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
    'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
    'X',  'Y',  'Z',    0,    0,    0,  '^',  '_',
    '`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',
    'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
    'p',  'q',  'r',  's',  't',  'u',  'v',  'w',
    'x',  'y',  'z',    0,  '|',    0,  '~',    0,
  };
  return tchar[c];
}

constexpr bool is_vchar(const char c) {
  // VCHAR = %x21-7E  ; visible (printing) characters
  return 0x21 <= c && c <= 0x7E;
}

constexpr bool is_gen_delim(const char c) {
  // gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
  //            ; delimiters of the generic URI components
  switch (c) {
    case ':': case '/': case '?': case '#': case '[': case ']': case '@':
      return true;
    default:
      return false;
  }
}

constexpr bool is_sub_delim(const char c) {
  // sub-delims = "!" / "$" / "&" / "'" / "(" / ")"
  //            / "*" / "+" / "," / ";" / "="
  switch (c) {
    case '!': case '$': case '&': case '\'': case '(': case ')':
    case '*': case '+': case ',': case  ';': case '=':
      return true;
    default:
      return false;
  }
}

constexpr bool is_reserved(const char c) {
  // reserved = gen-delims / sub-delims
  return is_gen_delim(c) || is_sub_delim(c);
}

constexpr bool is_unreserved(const char c) {
  // unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
  switch (c) {
    default:
      return is_alpha(c) || is_digit(c);
    case '-': case '.': case '_': case '~':
      return true;
  }
}

size_t is_dec_octet(std::string_view view) {
  // dec-octet = DIGIT              ; 0-9
  //           / %x31-39 DIGIT      ; 10-99
  //           / "1" 2DIGIT         ; 100-199
  //           / "2" %x30-34 DIGIT  ; 200-249
  //           / "25" %x30-35       ; 250-255
  view = view.substr(0, 3);
  const int value = hypp::detail::from_chars<int>(view);
  const auto it = std::find_if_not(view.begin(), view.end(), is_digit);
  switch (std::distance(view.begin(), it)) {
    case 1: return 1;
    case 2: return 10 <= value ? 2 : 0;
    case 3: return 100 <= value && value <= 255 ? 3 : 0;
  }
  return 0;
}

constexpr size_t is_pct_encoded(const std::string_view view) {
  // pct-encoded = "%" HEXDIG HEXDIG
  return view.size() > 2 && view[0] == '%' &&
         is_hex_digit(view[1]) && is_hex_digit(view[2]) ? 3 : 0;
}

constexpr size_t is_pchar(const std::string_view view) {
  // pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
  constexpr auto p = [](const char c) {
    switch (c) {
      default:
        return is_unreserved(c) || is_sub_delim(c);
      case ':': case '@':
        return true;
    }
  };
  std::string_view v{view};
  for (size_t count = std::string_view::npos; count && !v.empty(); ) {
    count = p(v.front()) ? size_t{1} : is_pct_encoded(v);
    v.remove_prefix(count);
  }
  return view.size() - v.size();
}

}  // namespace hypp::detail
