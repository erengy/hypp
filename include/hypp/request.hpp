#pragma once

#include <string>

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

  // > The most common form of request-target is the origin-form.
  // Reference: https://tools.ietf.org/html/rfc7230#section-5.3.1
  Form form = Form::Origin;
  Uri uri;
};

class RequestLine {
public:
  constexpr RequestLine() = default;

  Method method;
  RequestTarget target;
  HttpVersion http_version;
};

class Request : public Message<RequestLine> {
public:
  constexpr Request() = default;
};

}  // namespace hypp
