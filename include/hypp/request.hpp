#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/message.hpp>
#include <hypp/method.hpp>
#include <hypp/uri.hpp>
#include <hypp/version.hpp>

namespace hypp {

class RequestTarget {
public:
  constexpr RequestTarget() = default;

  // Reference: https://tools.ietf.org/html/rfc7230#section-5.3
  enum class Form {
    Origin,     // e.g. "GET /where?q=now HTTP/1.1"
    Absolute,   // e.g. "GET http://www.example.org/pub/WWW/TheProject.html HTTP/1.1"
    Authority,  // e.g. "CONNECT www.example.com:80 HTTP/1.1"
    Asterisk,   // e.g. "OPTIONS * HTTP/1.1"
  };

  std::string to_string() const {
    switch (form) {
      // origin-form = absolute-path [ "?" query ]
      case Form::Origin:
        return uri.query.has_value() ?
            detail::util::concat(uri.path, '?', *uri.query) :
            uri.path;

      // absolute-form = absolute-URI
      case Form::Absolute:
      default:
        return uri.to_string();

      // authority-form = authority
      case Form::Authority:
        return uri.authority.has_value() ?
            uri.authority->to_string() : std::string{};

      // asterisk-form = "*"
      case Form::Asterisk:
        return "*";
    }
  }

  Form form = Form::Absolute;
  Uri uri;
};

class RequestLine {
public:
  constexpr RequestLine() = default;

  // request-line = method SP request-target SP HTTP-version CRLF
  std::string to_string() const {
    using namespace parser::syntax;
    return detail::util::concat(
        method.to_string(), kSP,
        target.to_string(), kSP,
        http_version.to_string(), kCRLF);
  }

  Method method;
  RequestTarget target;
  HttpVersion http_version;
};

class Request : public Message<RequestLine> {
public:
  constexpr Request() = default;
};

}  // namespace hypp
