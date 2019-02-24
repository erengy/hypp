#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/detail/syntax.hpp>
#include <hypp/generator/uri.hpp>
#include <hypp/generator/version.hpp>
#include <hypp/request.hpp>

namespace hypp {

// request-target = origin-form
//                / absolute-form
//                / authority-form
//                / asterisk-form
std::string to_string(RequestTarget target) {
  // > A sender MUST NOT generate the userinfo subcomponent (and its "@"
  // delimiter) when an "http" URI reference is generated within a message
  // as a request target or header field value.
  // Reference: https://tools.ietf.org/html/rfc7230#section-2.7.1
  if (target.uri.authority.has_value()) {
    target.uri.authority->user_info.reset();
  }

  // > The target URI excludes the reference's fragment component, if any,
  // since fragment identifiers are reserved for client-side processing.
  // Reference: https://tools.ietf.org/html/rfc7230#section-5.1
  target.uri.fragment.reset();

  switch (target.form) {
    // origin-form = absolute-path [ "?" query ]
    default:
    case RequestTarget::Form::Origin:
      // > If the target URI's path component is empty, the client MUST send
      // "/" as the path within the origin-form of request-target.
      // Reference: https://tools.ietf.org/html/rfc7230#section-5.3.1
      if (target.uri.path.empty()) {
        target.uri.path = "/";
      }
      return target.uri.query.has_value() ?
          detail::concat(target.uri.path, '?', *target.uri.query) :
          target.uri.path;

    // absolute-form = absolute-URI
    case RequestTarget::Form::Absolute:
      return to_string(target.uri);

    // authority-form = authority
    case RequestTarget::Form::Authority:
      return target.uri.authority.has_value() ?
          to_string(*target.uri.authority) : std::string{};

    // asterisk-form = "*"
    case RequestTarget::Form::Asterisk:
      return "*";
  }
}

// request-line = method SP request-target SP HTTP-version CRLF
std::string to_string(const RequestLine& request_line) {
  using namespace detail::syntax;
  return detail::concat(
      request_line.method, kSP,
      to_string(request_line.target), kSP,
      to_string(request_line.http_version), kCRLF);
}

}  // namespace hypp
