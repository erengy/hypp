#pragma once

#include <string>

#include <hypp/detail/syntax.hpp>
#include <hypp/detail/util.hpp>
#include <hypp/version.hpp>

namespace hypp {

// HTTP-version = HTTP-name "/" DIGIT "." DIGIT
inline std::string to_string(const Version& version) {
  using namespace detail::syntax;
  return detail::concat(kHttpName, '/', version.major, '.', version.minor);
}

}  // namespace hypp
