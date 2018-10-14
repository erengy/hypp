#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <hypp/detail/util.hpp>
#include <hypp/parser/syntax.hpp>

namespace hypp {

class Header {
public:
  constexpr Header() = default;

  class Field {
  public:
    constexpr Field() = default;
    constexpr Field(const std::string_view name, const std::string_view value)
        : name{name}, value{value} {}

    // header-field = field-name ":" OWS field-value OWS
    std::string to_string() const {
      using namespace parser::syntax;
      // > For protocol elements where optional whitespace is preferred to
      // improve readability, a sender SHOULD generate the optional whitespace
      // as a single SP.
      // Reference: https://tools.ietf.org/html/rfc7230#section-3.2.3
      return detail::util::concat(name, ':', kSP, value);
    }

    std::string name;
    std::string value;
  };

  // *( header-field CRLF )
  std::string to_string() const {
    using namespace parser::syntax;
    std::string output;
    for (const auto& field : fields) {
      output += field.to_string() + kCRLF;
    }
    return output;
  }

  std::vector<Field> fields;
};

}  // namespace hypp
