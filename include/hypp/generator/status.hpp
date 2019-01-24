#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/parser/limits.hpp>
#include <hypp/status.hpp>

namespace hypp {

// status-code = 3DIGIT
std::string to_string(const status::code_t& code) {
  return detail::util::to_chars(code).substr(0, parser::limits::kStatusCode);
}

}  // namespace hypp
