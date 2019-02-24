#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/uri.hpp>

namespace hypp {

// authority = [ userinfo "@" ] host [ ":" port ]
std::string to_string(const Uri::Authority& authority) {
  std::string output;

  if (authority.user_info.has_value()) {
    output += detail::concat(*authority.user_info, '@');
  }

  output += authority.host;

  if (authority.port.has_value()) {
    output += detail::concat(':', *authority.port);
  }

  return output;
}

// http-URI = "http:" "//" authority path-abempty [ "?" query ]
//            [ "#" fragment ]
std::string to_string(const Uri& uri) {
  // > Note that we are careful to preserve the distinction between a
  // component that is undefined, meaning that its separator was not present
  // in the reference, and a component that is empty, meaning that the
  // separator was present and was immediately followed by the next component
  // separator or the end of the reference.
  // Reference: https://tools.ietf.org/html/rfc3986#section-5.3

  std::string output;

  if (uri.scheme.has_value()) {
    output += detail::concat(*uri.scheme, ':');
  }

  if (uri.authority.has_value()) {
    // > A sender MUST NOT generate an "http" URI with an empty host identifier.
    // Reference: https://tools.ietf.org/html/rfc7230#section-2.7.1
    if (uri.authority->host.empty()) {
      return {};
    }
    output += detail::concat("//", to_string(*uri.authority));
  }

  // > A path is always defined for a URI, though the defined path may be
  // empty (zero length).
  // Reference: https://tools.ietf.org/html/rfc3986#section-3.3
  output += uri.path;

  if (uri.query.has_value()) {
    output += detail::concat('?', *uri.query);
  }

  if (uri.fragment.has_value()) {
    output += detail::concat('#', *uri.fragment);
  }

  return output;
}

}  // namespace hypp
