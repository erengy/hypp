#pragma once

#include <string_view>

#include <hypp/detail/limits.hpp>
#include <hypp/detail/parser.hpp>
#include <hypp/detail/syntax.hpp>
#include <hypp/parser/error.hpp>
#include <hypp/parser/expected.hpp>

namespace hypp::parser {

Expected<std::string_view> ParseMethod(Parser& parser) {
  // method = token
  const auto view = parser.Match(hypp::detail::limits::kMethod,
                                 hypp::detail::IsTchar);

  if (view.empty()) {
    return Unexpected{Error::Invalid_Method};
  }

  // > A server that receives a method longer than any that it implements SHOULD
  // respond with a 501 (Not Implemented) status code.
  // Reference: https://tools.ietf.org/html/rfc7230#section-3.1.1
  if (parser.Peek(hypp::detail::IsTchar)) {
    return Unexpected{Error::Not_Implemented};
  }

  return view;
}

}  // namespace hypp::parser
