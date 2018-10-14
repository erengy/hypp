#pragma once

#include <utility>

namespace hypp::detail {

// A naive implementation of the std::expected proposal
// Reference: https://wg21.link/P0323r6

template <typename E>
class Unexpected {
public:
  Unexpected() = delete;
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

template <typename T, typename E, E default_error>
class Expected {
public:
  using value_t = T;
  using error_t = E;
  using expected_t = Expected<T, E, default_error>;
  using unexpected_t = Unexpected<E>;

  constexpr Expected() = delete;
  constexpr Expected(const value_t& value)
      : value_{value}, unexpected_{default_error} {}
  constexpr Expected(value_t&& value)
      : value_{std::move(value)}, unexpected_{default_error} {}
  constexpr Expected(const unexpected_t& unexpected)
      : unexpected_{unexpected} {}
  constexpr Expected(unexpected_t&& unexpected)
      : unexpected_{std::move(unexpected)} {}

  constexpr bool operator==(const expected_t& rhs) const {
    return value_ == rhs.value();
  }
  constexpr bool operator!=(const expected_t& rhs) const {
    return value_ != rhs.value();
  }

  constexpr explicit operator bool() const {
    return unexpected_.value() == default_error;
  }
  constexpr bool has_value() const {
    return unexpected_.value() == default_error;
  }

  constexpr error_t error() const {
    return unexpected_.value();
  }

  constexpr operator value_t() const {
    return value_;
  }
  constexpr const value_t& value() const {
    return value_;
  }
  constexpr value_t& value() {
    return value_;
  }

private:
  value_t value_;
  unexpected_t unexpected_;
};

}  // namespace hypp::detail
