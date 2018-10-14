#pragma once

#include <algorithm>
#include <functional>
#include <string_view>

namespace hypp::detail {

class Parser {
public:
  using pred_t = std::function<bool(const char)>;
  using size_t = std::string_view::size_type;
  using view_t = std::string_view;

  constexpr Parser(const view_t v) : v_{v} {}

  constexpr bool empty() const {
    return v_.empty();
  }
  constexpr size_t size() const {
    return v_.size();
  }

  constexpr bool Peek(const char c) const {
    return !v_.empty() && v_.front() == c;
  }
  constexpr bool Peek(const view_t s) const {
    return v_.size() >= s.size() && v_.compare(0, s.size(), s) == 0;
  }
  bool Peek(const pred_t p) const {
    return !v_.empty() && p(v_.front());
  }
  bool Peek(size_t count, const pred_t p) const {
    const auto s = v_.substr(0, std::min(count, v_.size()));
    return s.empty() && std::all_of(s.begin(), s.end(), p);
  }

  char Match(const pred_t p) {
    if (!v_.empty()) {
      const char c = v_.front();
      if (p(c)) {
        v_.remove_prefix(1);
        return c;
      }
    }
    return '\0';
  }
  view_t Match(const size_t count, const pred_t p) {
    const auto s = v_.substr(0, std::min(count, v_.size()));
    if (!s.empty()) {
      const auto it = std::find_if_not(s.begin(), s.end(), p);
      const auto size = std::distance(s.begin(), it);
      const view_t r{s.substr(0, size)};
      v_.remove_prefix(size);
      return r;
    }
    return {};
  }

  constexpr view_t ReadAll() {
    const view_t v{v_};
    v_.remove_prefix(v_.size());
    return v;
  }

  constexpr bool Skip(const char c) {
    if (Peek(c)) {
      v_.remove_prefix(1);
      return true;
    }
    return false;
  }
  constexpr bool Skip(const view_t s) {
    if (Peek(s)) {
      v_.remove_prefix(s.size());
      return true;
    }
    return false;
  }

  constexpr bool Strip(const char c) {
    const auto pos = v_.find_first_not_of(c);
    if (pos != view_t::npos && pos > 0) {
      v_.remove_prefix(pos);
      return true;
    }
    return false;
  }
  constexpr bool Strip(const view_t s) {
    const auto pos = v_.find_first_not_of(s);
    if (pos != view_t::npos && pos > 0) {
      v_.remove_prefix(pos);
      return true;
    }
    return false;
  }

private:
  view_t v_;
};

}  // namespace hypp::detail

namespace hypp {

using Parser = detail::Parser;

}  // namespace hypp
