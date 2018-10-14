#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/message.hpp>
#include <hypp/status.hpp>
#include <hypp/version.hpp>

namespace hypp {

class StatusLine {
public:
  constexpr StatusLine() = default;

  // status-line = HTTP-version SP status-code SP reason-phrase CRLF
  std::string to_string() const {
    using namespace parser::syntax;
    return detail::util::concat(
        version.to_string(), kSP, code.to_string(), kSP, code.phrase(), kCRLF);
  }

  HttpVersion version;
  StatusCode code;
};

class Response : public Message<StatusLine> {
public:
  constexpr Response() = default;
};

}  // namespace hypp
