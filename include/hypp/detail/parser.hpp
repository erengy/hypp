#pragma once

#include <algorithm>
#include <functional>
#include <string_view>

namespace hypp::detail {

class Parser {
public:
  using view_t = std::string_view;
  using size_t = view_t::size_type;

  using pred_t = std::function<bool(const char)>;
  using func_t = std::function<size_t(const view_t)>;

  constexpr Parser(const view_t v) : v_{v} {}

  constexpr bool empty() const {
    return v_.empty();
  }
  constexpr size_t size() const {
    return v_.size();
  }

  constexpr bool peek(const char c) const {
    return !v_.empty() && v_.front() == c;
  }
  constexpr bool peek(const view_t s) const {
    return v_.size() >= s.size() && v_.compare(0, s.size(), s) == 0;
  }
  bool peek(const pred_t p) const {
    return !v_.empty() && p(v_.front());
  }
  size_t peek(const func_t f) const {
    return !v_.empty() ? f(v_) : size_t{0};
  }

  size_t count(size_t n, const pred_t p) const {
    n = std::min(n, v_.size());
    const auto it = std::find_if_not(v_.begin(), v_.begin() + n, p);
    return std::distance(v_.begin(), it);
  }
  size_t count(size_t n, const func_t f) const {
    view_t v{v_.substr(0, n)};
    while (!v.empty() && (n = f(v))) {
      v.remove_prefix(n);
    }
    return v_.size() - v.size();
  }

  char match(const pred_t p) {
    return peek(p) ? read() : '\0';
  }
  view_t match(const func_t f) {
    return read(peek(f));
  }
  view_t match(const size_t n, const pred_t p) {
    return read(count(n, p));
  }
  view_t match(const size_t n, const func_t f) {
    return read(count(n, f));
  }
  view_t match(const size_t n, const pred_t p, const func_t f) {
    const auto f_both = [&](const view_t v) {
      return p(v.front()) ? size_t{1} : f(v);
    };
    return read(count(n, f_both));
  }

  constexpr char read() {
    const char c = !v_.empty() ? v_.front() : '\0';
    v_.remove_prefix(c ? 1 : 0);
    return c;
  }
  constexpr view_t read(const size_t n) {
    const view_t s = v_.substr(0, n);
    v_.remove_prefix(s.size());
    return s;
  }
  constexpr view_t read_all() {
    return read(v_.size());
  }

  constexpr bool remove(const size_t n) {
    const size_t rn = std::min(n, v_.size());
    v_.remove_prefix(rn);
    return n && n == rn;
  }

  constexpr bool skip(const char c) {
    return peek(c) && remove(size_t{1});
  }
  constexpr bool skip(const view_t s) {
    return peek(s) && remove(s.size());
  }

  constexpr bool strip(const view_t s) {
    const auto pos = v_.find_first_not_of(s);
    return pos != view_t::npos && remove(pos);
  }

private:
  view_t v_;
};

}  // namespace hypp::detail

namespace hypp {

using Parser = detail::Parser;

}  // namespace hypp
