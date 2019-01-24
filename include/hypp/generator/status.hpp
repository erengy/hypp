#pragma once

#include <string>

#include <hypp/parser/limits.hpp>
#include <hypp/status.hpp>

namespace hypp {

// status-code = 3DIGIT
std::string to_string(const status::code_t& code) {
  return std::to_string(code).substr(0, parser::limits::kStatusCode);
}

}  // namespace hypp
