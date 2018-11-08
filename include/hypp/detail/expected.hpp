#pragma once

#include <utility>
#include <variant>

namespace hypp::detail {

// A naive implementation of the std::expected proposal
// Reference: https://wg21.link/P0323r6

template <typename E>
class Unexpected {
public:
  constexpr Unexpected() = delete;
  constexpr explicit Unexpected(const E& value) : value_{value} {}
  constexpr explicit Unexpected(E&& value) : value_{std::move(value)} {}

  constexpr bool operator==(const Unexpected<E>& rhs) const {
    return value_ == rhs.value();
  }
  constexpr bool operator!=(const Unexpected<E>& rhs) const {
    return value_ != rhs.value();
  }

  constexpr E value() const {
    return value_;
  }

private:
  E value_;
};

template <typename T, typename E>
class Expected {
public:
  using value_t = T;
  using error_t = E;
  using expected_t = Expected<T, E>;
  using unexpected_t = Unexpected<E>;

  constexpr Expected() = delete;
  constexpr Expected(const value_t& value)
      : value_{value} {}
  constexpr Expected(value_t&& value)
      : value_{std::move(value)} {}
  constexpr Expected(const unexpected_t& unexpected)
      : value_{unexpected} {}
  constexpr Expected(unexpected_t&& unexpected)
      : value_{std::move(unexpected)} {}

  constexpr bool operator==(const expected_t& rhs) const {
    if (has_value() != rhs.has_value()) return false;
    return has_value() ? value() == rhs.value() : error() == rhs.error();
  }
  constexpr bool operator!=(const expected_t& rhs) const {
    if (has_value() != rhs.has_value()) return true;
    return has_value() ? value() != rhs.value() : error() != rhs.error();
  }

  constexpr explicit operator bool() const {
    return has_value();
  }
  constexpr bool has_value() const {
    return std::holds_alternative<value_t>(value_);
  }

  constexpr error_t error() const {
    return std::get<unexpected_t>(value_).value();
  }

  constexpr operator value_t() const {
    return std::get<value_t>(value_);
  }
  constexpr const value_t& value() const {
    return std::get<value_t>(value_);
  }

private:
  std::variant<value_t, unexpected_t> value_;
};

}  // namespace hypp::detail
