#pragma once

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

int to_int(const std::string_view view) {
  int value{0};
  std::from_chars(view.data(), view.data() + view.size(), value, 10);
  return value;
}

}  // namespace hypp::detail::util
