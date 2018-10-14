#pragma once

#include <algorithm>
#include <charconv>
#include <string>
#include <string_view>

namespace hypp::detail::util {

std::string operator+(std::string lhs, const std::string_view& rhs) {
  return lhs.append(rhs);
}
template <typename... Ts>
std::string concat(const Ts&... args) {
  return (std::string{} + ... + args);
}

constexpr bool equals(const std::string_view a, const std::string_view b) {
  constexpr auto to_lower = [](const char c) {
    return ('A' <= c && c <= 'Z') ? c + ('a' - 'A') : c;
  };

  constexpr auto equal_chars = [&to_lower](const char a, const char b) {
    return to_lower(a) == to_lower(b);
  };

  return a.size() == b.size() &&
         std::equal(a.begin(), a.end(), b.begin(), equal_chars);
}

int to_int(const std::string_view view) {
  int value{0};
  std::from_chars(view.data(), view.data() + view.size(), value, 10);
  return value;
}

}  // namespace hypp::detail::util
