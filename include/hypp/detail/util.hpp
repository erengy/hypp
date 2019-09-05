#pragma once

#include <array>
#include <charconv>
#include <string>
#include <string_view>

namespace hypp::detail {

inline std::string operator+(std::string lhs, const std::string_view& rhs) {
  return lhs.append(rhs);
}
template <typename... Ts>
std::string concat(const Ts&... args) {
  return (std::string{} + ... + args);
}

template <typename T>
T from_chars(const std::string_view str) {
  T value{0};
  std::from_chars(str.data(), str.data() + str.size(), value, 10);
  return value;
}

template <typename T>
std::string to_chars(const T value) {
  std::array<char, 16> str;
  const auto result = std::to_chars(str.data(), str.data() + str.size(), value);
  return result.ec == std::errc() ?
      std::string(str.data(), result.ptr - str.data()) : std::string{};
}

}  // namespace hypp::detail
