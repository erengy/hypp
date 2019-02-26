#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/uri.hpp>

namespace hypp {

namespace detail {

bool VerifyUriAuthority(const Uri::Authority& authority) {
  // > A sender MUST NOT generate an "http" URI with an empty host identifier.
  // Reference: https://tools.ietf.org/html/rfc7230#section-2.7.1
  if (authority.host.empty()) {
    return false;
  }

  return true;
}

bool VerifyUri(const Uri& uri) {
  // "http" scheme considers a missing authority or empty host invalid.
  // Reference: https://tools.ietf.org/html/rfc3986#section-3.2.2
  //
  // > A sender MUST NOT generate an "http" URI with an empty host identifier.
  // Reference: https://tools.ietf.org/html/rfc7230#section-2.7.1
  if (!uri.authority.has_value() || uri.authority->host.empty()) {
    return false;
  }

  // > If a URI contains an authority component, then the path component must
  // either be empty or begin with a slash ("/") character.
  // Reference: https://tools.ietf.org/html/rfc3986#section-3.3
  if (uri.authority.has_value()) {
    if (!uri.path.empty() && uri.path.front() != '/') {
      return false;
    }
  // > If a URI does not contain an authority component, then the path cannot
  // begin with two slash characters ("//").
  // Reference: https://tools.ietf.org/html/rfc3986#section-3.3
  } else {
    if (uri.path.size() >= 2 && uri.path.substr(0, 2) == "//") {
      return false;
    }
  }

  return true;
}

}  // namespace detail

// authority = [ userinfo "@" ] host [ ":" port ]
std::string to_string(const Uri::Authority& authority) {
  if (!detail::VerifyUriAuthority(authority)) {
    return {};
  }

  std::string output;

  if (authority.user_info.has_value()) {
    output += detail::concat(*authority.user_info, '@');
  }

  output += authority.host;

  // > URI producers and normalizers should omit the ":" delimiter that
  // separates host from port if the port component is empty.
  // Reference: https://tools.ietf.org/html/rfc3986#section-3.2
  if (authority.port.has_value()) {
    output += detail::concat(':', *authority.port);
  }

  return output;
}

// http-URI = "http:" "//" authority path-abempty [ "?" query ]
//            [ "#" fragment ]
std::string to_string(const Uri& uri) {
  if (!detail::VerifyUri(uri)) {
    return {};
  }

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

  output += detail::concat("//", to_string(*uri.authority));

  // > A path is always defined for a URI, though the defined path may be empty
  // (zero length).
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
