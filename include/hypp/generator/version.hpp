#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/version.hpp>

namespace hypp {

// HTTP-version = HTTP-name "/" DIGIT "." DIGIT
std::string to_string(const HttpVersion& version) {
  using namespace parser::syntax;
  return detail::concat(kHttpName, '/', version.major, '.', version.minor);
}

}  // namespace hypp
