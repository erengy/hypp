#pragma once

#include <optional>

#include <hypp/detail/parser.hpp>
#include <hypp/parser/error.hpp>
#include <hypp/parser/expected.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/version.hpp>

namespace hypp::parser {

namespace detail {

constexpr std::optional<Error> VerifyHttpVersion(const char major,
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

Expected<HttpVersion> ParseHttpVersion(Parser& parser) {
  // HTTP-name "/"
  // HTTP-name = %x48.54.54.50 ; "HTTP", case-sensitive
  //
  // > HTTP-version is case-sensitive.
  // Reference: https://tools.ietf.org/html/rfc7230#section-2.6
  //
  // > The expectation to support HTTP/0.9 requests has been removed.
  // Reference: https://tools.ietf.org/html/rfc7230#appendix-A.2
  if (!parser.Skip("HTTP/")) {
    return Unexpected{Error::Invalid_HTTP_Name};
  }

  // DIGIT "." DIGIT
  const auto major = parser.Match(syntax::IsDigit);
  if (!major) {
    return Unexpected{Error::Invalid_HTTP_Version};
  }
  if (!parser.Skip('.')) {
    return Unexpected{Error::Invalid_HTTP_Version};
  }
  const auto minor = parser.Match(syntax::IsDigit);
  if (!minor) {
    return Unexpected{Error::Invalid_HTTP_Version};
  }

  if (const auto error = detail::VerifyHttpVersion(major, minor)) {
    return Unexpected{*error};
  }

  return HttpVersion{major, minor};
}

}  // namespace hypp::parser
