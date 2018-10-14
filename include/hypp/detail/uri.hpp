#pragma once

#include <string>
#include <string_view>

namespace hypp::detail::uri {

std::string decode(const std::string_view str) {
  std::string output;

  constexpr auto digits = "0123456789ABCDEF";
  constexpr auto is_xdigit = [](const char c) {
    return ('0' <= c && c <= '9') ||
           ('A' <= c && c <= 'F') ||
           ('a' <= c && c <= 'f');
  };
  constexpr auto is_encoded = [&](const size_t i) {
    return str[i] == '%' &&
           i + 2 < str.size() &&
           is_xdigit(str[i + 1]) &&
           is_xdigit(str[i + 2]);
  };

  for (size_t i = 0; i < str.size(); ++i) {
    if (!is_encoded(i)) {
      output.push_back(str[i]);
    } else {
      char c = 0;
      for (char j = 0; j < 16; ++j) {
        if (str[i + 1] == digits[j]) c += j << 4;
        if (str[i + 2] == digits[j]) c += j;
      }
      output.push_back(c);
      i += 2;
    }
  }

  return output;
}

std::string encode(const std::string_view str) {
  std::string output;

  constexpr auto digits = "0123456789ABCDEF";
  constexpr auto safe = [](const char c) {
    return ('0' <= c && c <= '9') ||
           ('A' <= c && c <= 'Z') ||
           ('a' <= c && c <= 'z') ||
           c == '-' || c == '.' || c == '_' || c == '~';
  };

  for (const auto c : str) {
    if (safe(c)) {
      output.push_back(c);
    } else {
      output.push_back('%');
      output.push_back(digits[(c >> 4) & 0x0F]);
      output.push_back(digits[c & 0x0F]);
    }
  }

  return output;
}

}  // namespace hypp::detail::uri
