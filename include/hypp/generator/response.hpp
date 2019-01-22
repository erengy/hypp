#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/generator/version.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/response.hpp>

namespace hypp {

// status-line = HTTP-version SP status-code SP reason-phrase CRLF
std::string to_string(const StatusLine& status_line) {
  using namespace parser::syntax;
  return detail::util::concat(
      to_string(status_line.version), kSP,
      status_line.code.to_string(), kSP,
      status_line.code.phrase(), kCRLF);
}

}  // namespace hypp
