#pragma once

#include <array>
#include <string_view>

namespace hypp::parser::syntax {

// Reference: https://tools.ietf.org/html/rfc5234#appendix-B.1
// Reference: https://tools.ietf.org/html/rfc7230#appendix-B

constexpr auto kCRLF = "\r\n";       // Carriage Return + Line Feed
constexpr auto kHTAB = '\t';         // Horizontal Tab
constexpr auto kHttpName = "HTTP";   // HTTP name (case-sensitive)
constexpr auto kSP = ' ';            // Space
constexpr auto kWhitespace = " \t";  // Whitespace (SP / HTAB)

constexpr bool IsAlpha(const char c) {
  // ALPHA = %x41-5A / %x61-7A ; A-Z / a-z
  return ('A' <= c && c <= 'Z') ||
         ('a' <= c && c <= 'z');
}

constexpr bool IsDigit(const char c) {
  // DIGIT = %x30-39 ; 0-9
  return '0' <= c && c <= '9';
}

constexpr bool IsHexDigit(const char c) {
  // HEXDIG = DIGIT / "A" / "B" / "C" / "D" / "E" / "F"
  return ('0' <= c && c <= '9') ||
         ('A' <= c && c <= 'F') ||
         ('a' <= c && c <= 'f');
}

constexpr bool IsObsText(const char c) {
  // obs-text = %x80-FF
  return 0x80 <= c && c <= 0xFF;
}

constexpr bool IsTchar(const char c) {
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

constexpr bool IsVchar(const char c) {
  // VCHAR = %x21-7E ; visible (printing) characters
  return 0x21 <= c && c <= 0x7E;
}

constexpr bool IsGenDelim(const char c) {
  // gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
  //            ; delimiters of the generic URI components
  switch (c) {
    case ':': case '/': case '?': case '#': case '[': case ']': case '@':
      return true;
    default:
      return false;
  }
}

constexpr bool IsSubDelim(const char c) {
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

constexpr bool IsPctEncoded(const std::string_view view) {
  // pct-encoded = "%" HEXDIG HEXDIG
  return view.size() > 2 && view[0] == '%' &&
         IsHexDigit(view[1]) && IsHexDigit(view[2]);
}

constexpr bool IsReserved(const char c) {
  // reserved = gen-delims / sub-delims
  return IsGenDelim(c) || IsSubDelim(c);
}

constexpr bool IsUnreserved(const char c) {
  // unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
  switch (c) {
    default:
      return IsAlpha(c) || IsDigit(c);
    case '-': case '.': case '_': case '~':
      return true;
  }
}

constexpr bool IsPchar(const char c) {
  // @TODO: pct-encoded
  // pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
  switch (c) {
    default:
      return IsUnreserved(c) || IsSubDelim(c);
    case ':': case '@':
      return true;
  }
}

}  // namespace hypp::parser::syntax
