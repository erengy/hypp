#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/generator/uri.hpp>
#include <hypp/generator/version.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/request.hpp>

namespace hypp {

// request-target = origin-form
//                / absolute-form
//                / authority-form
//                / asterisk-form
std::string to_string(const RequestTarget& target) {
  switch (target.form) {
    // origin-form = absolute-path [ "?" query ]
    default:
    case RequestTarget::Form::Origin: {
      // > If the target URI's path component is empty, the client MUST send
      // "/" as the path within the origin-form of request-target.
      // Reference: https://tools.ietf.org/html/rfc7230#section-5.3.1
      const std::string path = !target.uri.path.empty() ? target.uri.path : "/";
      return target.uri.query.has_value() ?
          detail::util::concat(path, '?', *target.uri.query) : path;
    }

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
  using namespace parser::syntax;
  return detail::util::concat(
      request_line.method.to_string(), kSP,
      to_string(request_line.target), kSP,
      to_string(request_line.http_version), kCRLF);
}

}  // namespace hypp
