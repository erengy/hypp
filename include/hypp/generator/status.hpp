#pragma once

#include <string>

#include <hypp/detail/limits.hpp>
#include <hypp/detail/util.hpp>
#include <hypp/status.hpp>

namespace hypp {

// status-code = 3DIGIT
std::string to_string(const status::code_t& code) {
  return detail::to_chars(code).substr(0, detail::limits::kStatusCode);
}

}  // namespace hypp
