#pragma once

#include <hypp/detail/parser.hpp>
#include <hypp/detail/syntax.hpp>
#include <hypp/parser/message.hpp>
#include <hypp/parser/status.hpp>
#include <hypp/parser/version.hpp>
#include <hypp/error.hpp>
#include <hypp/response.hpp>

namespace hypp {

// status-line = HTTP-version SP status-code SP reason-phrase CRLF
inline Expected<StatusLine> ParseStatusLine(Parser& parser) {
  StatusLine status_line;

  // HTTP-version SP
  if (const auto expected = ParseVersion(parser)) {
    status_line.version = expected.value();
  } else {
    return Unexpected{expected.error()};
  }
  if (!parser.skip(detail::syntax::kSP)) {
    return Unexpected{Error::Bad_Response};
  }

  // status-code SP
  if (const auto expected = ParseStatusCode(parser)) {
    status_line.code = expected.value();
  } else {
    return Unexpected{expected.error()};
  }
  if (!parser.skip(detail::syntax::kSP)) {
    return Unexpected{Error::Bad_Response};
  }

  // reason-phrase CRLF
  //
  // > A client SHOULD ignore the reason-phrase content.
  // Reference: https://tools.ietf.org/html/rfc7230#section-3.1.2
  ParseReasonPhrase(parser);
  if (!parser.skip(detail::syntax::kCRLF)) {
    return Unexpected{Error::Bad_Response};
  }

  return status_line;
}

inline Expected<StatusLine> ParseStartLine(Parser& parser, const Response&) {
  return ParseStatusLine(parser);
}

inline Expected<Response> ParseResponse(const std::string_view view) {
  return ParseMessage<Response>(view);
}

}  // namespace hypp
