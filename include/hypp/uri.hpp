#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <hypp/detail/uri.hpp>
#include <hypp/detail/util.hpp>

namespace hypp {

class Uri {
public:
  constexpr Uri() = default;

  class Authority {
  public:
    constexpr Authority() = default;

    // authority = [ userinfo "@" ] host [ ":" port ]
    std::string to_string() const {
      std::string authority;
      if (!user_info.empty()) {
        // @TODO:
        // > A sender MUST NOT generate the userinfo subcomponent (and its "@"
        // delimiter) when an "http" URI reference is generated within a message
        // as a request target or header field value.
        // Reference: https://tools.ietf.org/html/rfc7230#section-2.7.1
        authority += detail::util::concat(user_info, '@');
      }
      authority += host;
      if (!port.empty()) {
        authority += detail::util::concat(':', port);
      }
      return authority;
    }

    std::string user_info;
    std::string host;
    std::string port;
  };

  std::string to_string() const {
    // > A sender MUST NOT generate an "http" URI with an empty host identifier.
    // Reference: https://tools.ietf.org/html/rfc7230#section-2.7.1
    if (authority.has_value() && authority->host.empty()) {
      return {};
    }

    // > Note that we are careful to preserve the distinction between a
    // component that is undefined, meaning that its separator was not present
    // in the reference, and a component that is empty, meaning that the
    // separator was present and was immediately followed by the next component
    // separator or the end of the reference.
    // Reference: https://tools.ietf.org/html/rfc3986#section-5.3

    // http-URI = "http:" "//" authority path-abempty [ "?" query ]
    //            [ "#" fragment ]
    std::string uri;
    if (scheme.has_value()) {
      uri += detail::util::concat(*scheme, ':');
    }
    if (authority.has_value()) {
      uri += detail::util::concat("//", authority->to_string());
    }
    uri += path;
    if (query.has_value()) {
      uri += detail::util::concat('?', *query);
    }
    if (fragment.has_value()) {
      uri += detail::util::concat('#', *fragment);
    }
    return uri;
  }

  std::optional<std::string> scheme;
  std::optional<Authority> authority;
  std::string path;
  std::optional<std::string> query;
  std::optional<std::string> fragment;
};

}  // namespace hypp
