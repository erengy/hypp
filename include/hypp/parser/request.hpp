#pragma once

#include <hypp/detail/parser.hpp>
#include <hypp/detail/syntax.hpp>
#include <hypp/parser/message.hpp>
#include <hypp/parser/method.hpp>
#include <hypp/parser/uri.hpp>
#include <hypp/parser/version.hpp>
#include <hypp/error.hpp>
#include <hypp/request.hpp>

namespace hypp {

// request-target = origin-form
//                / absolute-form
//                / authority-form
//                / asterisk-form
inline Expected<RequestTarget> ParseRequestTarget(Parser& parser) {
  RequestTarget request_target;

  // origin-form = absolute-path [ "?" query ]
  if (parser.peek('/')) {
    request_target.form = RequestTarget::Form::Origin;
    if (const auto expected = detail::ParseUriPath(parser, detail::kUriAbsolutePath)) {
      request_target.uri.path = expected.value();
    } else {
      return Unexpected{Error::Invalid_Request_Target};
    }
    if (parser.skip('?')) {
      if (const auto expected = detail::ParseUriQuery(parser)) {
        request_target.uri.query = expected.value();
      } else {
        return Unexpected{Error::Invalid_Request_Target};
      }
    }
    return request_target;
  }

  // absolute-form = absolute-URI
  if (const auto expected = detail::ParseAbsoluteUri(parser)) {
    request_target.form = RequestTarget::Form::Absolute;
    request_target.uri = expected.value();
    return request_target;
  }

  // asterisk-form = "*"
  if (parser.skip('*')) {
    request_target.form = RequestTarget::Form::Asterisk;
    return request_target;
  }

  // authority-form = authority
  if (const auto expected = detail::ParseUriAuthority(parser)) {
    request_target.form = RequestTarget::Form::Authority;
    request_target.uri.authority = expected.value();
    return request_target;
  }

  return Unexpected{Error::Invalid_Request_Target};
}

// request-line = method SP request-target SP HTTP-version CRLF
inline Expected<RequestLine> ParseRequestLine(Parser& parser) {
  RequestLine request_line;

  // > In the interest of robustness, a server that is expecting to receive
  // and parse a request-line SHOULD ignore at least one empty line (CRLF)
  // received prior to the request-line.
  // Reference: https://tools.ietf.org/html/rfc7230#section-3.5
  parser.skip(detail::syntax::kCRLF);

  // method SP
  if (const auto expected = ParseMethod(parser)) {
    request_line.method = expected.value();
  } else {
    return Unexpected{expected.error()};
  }
  if (!parser.skip(detail::syntax::kSP)) {
    return Unexpected{Error::Bad_Request};
  }

  // request-target SP
  if (const auto expected = ParseRequestTarget(parser)) {
    request_line.target = expected.value();
  } else {
    return Unexpected{expected.error()};
  }
  if (!parser.skip(detail::syntax::kSP)) {
    return Unexpected{Error::Bad_Request};
  }

  // HTTP-version CRLF
  if (const auto expected = ParseVersion(parser)) {
    request_line.version = expected.value();
  } else {
    return Unexpected{expected.error()};
  }
  if (!parser.skip(detail::syntax::kCRLF)) {
    return Unexpected{Error::Bad_Request};
  }

  return request_line;
}

inline Expected<RequestLine> ParseStartLine(Parser& parser, const Request&) {
  return ParseRequestLine(parser);
}

inline Expected<Request> ParseRequest(const std::string_view view) {
  return ParseMessage<Request>(view);
}

}  // namespace hypp
