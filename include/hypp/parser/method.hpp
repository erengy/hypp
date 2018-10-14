#pragma once

#include <hypp/detail/parser.hpp>
#include <hypp/parser/error.hpp>
#include <hypp/parser/expected.hpp>
#include <hypp/parser/limits.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/method.hpp>

namespace hypp::parser {

Expected<Method> ParseMethod(Parser& parser) {
  // method = token
  const auto view = parser.Match(limits::kMethod, syntax::IsTchar);

  if (view.empty()) {
    return Unexpected{Error::Invalid_Method};
  }

  // > A server that receives a method longer than any that it implements SHOULD
  // respond with a 501 (Not Implemented) status code.
  // Reference: https://tools.ietf.org/html/rfc7230#section-3.1.1
  if (parser.Peek(syntax::IsTchar)) {
    return Unexpected{Error::Not_Implemented};
  }

  return Method{view};
}

}  // namespace hypp::parser
