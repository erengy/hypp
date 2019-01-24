#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/generator/status.hpp>
#include <hypp/generator/version.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/response.hpp>

namespace hypp {

// status-line = HTTP-version SP status-code SP reason-phrase CRLF
std::string to_string(const StatusLine& status_line) {
  using namespace parser::syntax;
  return detail::concat(
      to_string(status_line.version), kSP,
      to_string(status_line.code), kSP,
      status::GetPhrase(status_line.code), kCRLF);
}

}  // namespace hypp
