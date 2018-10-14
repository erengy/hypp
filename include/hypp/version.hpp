#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/parser/syntax.hpp>

namespace hypp {

class HttpVersion {
public:
  constexpr HttpVersion() = default;
  constexpr HttpVersion(const char major, const char minor)
      : major{major}, minor{minor} {}

  // HTTP-version = HTTP-name "/" DIGIT "." DIGIT
  std::string to_string() const {
    using namespace parser::syntax;
    return detail::util::concat(kHttpName, '/', major, '.', minor);
  }

  char major = '\0';
  char minor = '\0';
};

}  // namespace hypp
