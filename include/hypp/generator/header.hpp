#pragma once

#include <string>

#include <hypp/detail/syntax.hpp>
#include <hypp/detail/util.hpp>
#include <hypp/header.hpp>

namespace hypp {

// header-field = field-name ":" OWS field-value OWS
std::string to_string(const HeaderField& header_field) {
  using namespace detail::syntax;
  // > For protocol elements where optional whitespace is preferred to
  // improve readability, a sender SHOULD generate the optional whitespace
  // as a single SP.
  // Reference: https://tools.ietf.org/html/rfc7230#section-3.2.3
  return detail::concat(header_field.name, ':', kSP, header_field.value);
}

// *( header-field CRLF )
std::string to_string(const HeaderFields& header_fields) {
  using namespace detail::syntax;
  std::string output;
  for (const auto& header_field : header_fields) {
    output += to_string(header_field) + kCRLF;
  }
  return output;
}

}  // namespace hypp
