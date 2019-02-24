#pragma once

#include <string_view>

#include <hypp/detail/limits.hpp>
#include <hypp/detail/parser.hpp>
#include <hypp/detail/syntax.hpp>
#include <hypp/error.hpp>

namespace hypp {

Expected<std::string_view> ParseMethod(Parser& parser) {
  // method = token
  const auto view = parser.match(detail::limits::kMethod, detail::is_tchar);

  if (view.empty()) {
    return Unexpected{Error::Invalid_Method};
  }

  // > A server that receives a method longer than any that it implements SHOULD
  // respond with a 501 (Not Implemented) status code.
  // Reference: https://tools.ietf.org/html/rfc7230#section-3.1.1
  if (parser.peek(detail::is_tchar)) {
    return Unexpected{Error::Not_Implemented};
  }

  return view;
}

}  // namespace hypp
