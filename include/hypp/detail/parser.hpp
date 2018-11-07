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

  constexpr bool Peek(const char c) const {
    return !v_.empty() && v_.front() == c;
  }
  constexpr bool Peek(const view_t s) const {
    return v_.size() >= s.size() && v_.compare(0, s.size(), s) == 0;
  }
  bool Peek(const pred_t p) const {
    return !v_.empty() && p(v_.front());
  }
  size_t Peek(const func_t f) const {
    return !v_.empty() ? f(v_) : size_t{0};
  }

  size_t Count(size_t count, const pred_t p) const {
    count = std::min(count, v_.size());
    const auto it = std::find_if_not(v_.begin(), v_.begin() + count, p);
    return std::distance(v_.begin(), it);
  }
  size_t Count(size_t count, const func_t f) const {
    view_t v{v_.substr(0, count)};
    while (!v.empty() && (count = f(v))) {
      v.remove_prefix(count);
    }
    return v_.size() - v.size();
  }

  char Match(const pred_t p) {
    return Peek(p) ? Read() : '\0';
  }
  view_t Match(const func_t f) {
    return Read(Peek(f));
  }
  view_t Match(const size_t count, const pred_t p) {
    return Read(Count(count, p));
  }
  view_t Match(const size_t count, const func_t f) {
    return Read(Count(count, f));
  }
  view_t Match(const size_t count, const pred_t p, const func_t f) {
    const auto f_both = [&](const view_t v) {
      return p(v.front()) ? size_t{1} : f(v);
    };
    return Read(Count(count, f_both));
  }

  constexpr char Read() {
    const char c = !v_.empty() ? v_.front() : '\0';
    v_.remove_prefix(c ? 1 : 0);
    return c;
  }
  constexpr view_t Read(const size_t count) {
    const view_t s = v_.substr(0, count);
    v_.remove_prefix(s.size());
    return s;
  }
  constexpr view_t ReadAll() {
    return Read(v_.size());
  }

  constexpr bool Remove(const size_t count) {
    const size_t rcount = std::min(count, v_.size());
    v_.remove_prefix(rcount);
    return count && count == rcount;
  }

  constexpr bool Skip(const char c) {
    return Peek(c) && Remove(size_t{1});
  }
  constexpr bool Skip(const view_t s) {
    return Peek(s) && Remove(s.size());
  }

  constexpr bool Strip(const view_t s) {
    const auto pos = v_.find_first_not_of(s);
    return pos != view_t::npos && Remove(pos);
  }

private:
  view_t v_;
};

}  // namespace hypp::detail

namespace hypp {

using Parser = detail::Parser;

}  // namespace hypp
