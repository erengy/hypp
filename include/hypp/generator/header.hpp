#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/header.hpp>

namespace hypp {

// header-field = field-name ":" OWS field-value OWS
std::string to_string(const Header::Field& field) {
  using namespace parser::syntax;
  // > For protocol elements where optional whitespace is preferred to
  // improve readability, a sender SHOULD generate the optional whitespace
  // as a single SP.
  // Reference: https://tools.ietf.org/html/rfc7230#section-3.2.3
  return detail::util::concat(field.name, ':', kSP, field.value);
}

// *( header-field CRLF )
std::string to_string(const Header& header) {
  using namespace parser::syntax;
  std::string output;
  for (const auto& field : header.fields) {
    output += to_string(field) + kCRLF;
  }
  return output;
}

}  // namespace hypp
