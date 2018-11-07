#pragma once

#include <hypp/detail/parser.hpp>
#include <hypp/parser/error.hpp>
#include <hypp/parser/expected.hpp>
#include <hypp/parser/message.hpp>
#include <hypp/parser/method.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/parser/uri.hpp>
#include <hypp/parser/version.hpp>
#include <hypp/request.hpp>

namespace hypp::parser {

Expected<RequestLine> ParseRequestLine(Parser& parser) {
  RequestLine request_line;

  // > In the interest of robustness, a server that is expecting to receive
  // and parse a request-line SHOULD ignore at least one empty line (CRLF)
  // received prior to the request-line.
  // Reference: https://tools.ietf.org/html/rfc7230#section-3.5
  parser.Skip(syntax::kCRLF);

  // method SP
  if (const auto expected = ParseMethod(parser)) {
    request_line.method = expected.value();
  } else {
    return Unexpected{expected.error()};
  }
  if (!parser.Skip(syntax::kSP)) {
    return Unexpected{Error::Bad_Request};
  }

  // request-target SP
  if (const auto expected = ParseUri(parser)) {
    request_line.target = expected.value();
  } else {
    return Unexpected{expected.error()};
  }
  if (!parser.Skip(syntax::kSP)) {
    return Unexpected{Error::Bad_Request};
  }

  // HTTP-version CRLF
  if (const auto expected = ParseHttpVersion(parser)) {
    request_line.http_version = expected.value();
  } else {
    return Unexpected{expected.error()};
  }
  if (!parser.Skip(syntax::kCRLF)) {
    return Unexpected{Error::Bad_Request};
  }

  return request_line;
}

Expected<RequestLine> ParseStartLine(Parser& parser, const Request&) {
  // request-line = method SP request-target SP HTTP-version CRLF
  return ParseRequestLine(parser);
}

Expected<Request> ParseRequest(const std::string_view view) {
  return ParseMessage<Request>(view);
}

}  // namespace hypp::parser
