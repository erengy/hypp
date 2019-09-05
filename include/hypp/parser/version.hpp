#pragma once

#include <optional>

#include <hypp/detail/parser.hpp>
#include <hypp/detail/syntax.hpp>
#include <hypp/error.hpp>
#include <hypp/version.hpp>

namespace hypp {

namespace detail {

constexpr std::optional<Error> VerifyVersion(const char major,
                                             const char minor) {
  // HTTP/0.9 messages do not indicate the protocol version. Higher major
  // versions of the protocol (e.g. HTTP/2.0) would have an incompatible
  // messaging syntax.
  if (major != '1') {
    // > The 505 (HTTP Version Not Supported) status code indicates that the
    // server does not support, or refuses to support, the major version of
    // HTTP that was used in the request message.
    // Reference: https://tools.ietf.org/html/rfc7231#section-6.6.6
    return Error::HTTP_Version_Not_Supported;
  }

  // HTTP/1.0
  if (minor == '0') {
    // > The 426 (Upgrade Required) status code indicates that the server
    // refuses to perform the request using the current protocol but might be
    // willing to do so after the client upgrades to a different protocol. The
    // server MUST send an Upgrade header field in a 426 response to indicate
    // the required protocol(s).
    // Reference: https://tools.ietf.org/html/rfc7231#section-6.5.15
    return Error::Upgrade_Required;
  }

  // We allow minor digits higher than '1', because:
  //
  // > When an HTTP message is received with a higher minor version number
  // than what the recipient implements, the recipient SHOULD process the
  // message as if it were in the highest minor version within that major
  // version to which the recipient is conformant.
  // Reference: https://tools.ietf.org/html/rfc7230#section-2.6

  return std::nullopt;
}

}  // namespace detail

// HTTP-version = HTTP-name "/" DIGIT "." DIGIT
constexpr Expected<Version> ParseVersion(Parser& parser) {
  // > The expectation to support HTTP/0.9 requests has been removed.
  // Reference: https://tools.ietf.org/html/rfc7230#appendix-A.2

  // HTTP-name "/"
  if (!parser.skip(detail::syntax::kHttpName) || !parser.skip('/')) {
    return Unexpected{Error::Invalid_HTTP_Name};
  }

  // DIGIT "." DIGIT
  const auto major = parser.match(detail::is_digit);
  if (!major) {
    return Unexpected{Error::Invalid_HTTP_Version};
  }
  if (!parser.skip('.')) {
    return Unexpected{Error::Invalid_HTTP_Version};
  }
  const auto minor = parser.match(detail::is_digit);
  if (!minor) {
    return Unexpected{Error::Invalid_HTTP_Version};
  }

  if (const auto error = detail::VerifyVersion(major, minor)) {
    return Unexpected{*error};
  }

  return Version{major, minor};
}

}  // namespace hypp
