#pragma once

#include <hypp/detail/limits.hpp>
#include <hypp/detail/parser.hpp>
#include <hypp/detail/syntax.hpp>
#include <hypp/parser/error.hpp>
#include <hypp/parser/expected.hpp>
#include <hypp/uri.hpp>

namespace hypp::parser {

// scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
Expected<std::string_view> ParseUriScheme(Parser& parser) {
  if (!parser.Peek(hypp::detail::IsAlpha)) {
    return Unexpected{Error::Invalid_URI_Scheme};
  }
  return parser.Match(hypp::detail::limits::kScheme,
      [](const char c) {
        switch (c) {
          case '+': case '-': case '.':
            return true;
          default:
            return hypp::detail::IsAlpha(c) || hypp::detail::IsDigit(c);
        }
      });
}

////////////////////////////////////////////////////////////////////////////////

// userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
Expected<std::string_view> ParseUriUserInfo(Parser& parser) {
  return parser.Match(hypp::detail::limits::kURI,
      [](const char c) {
        switch (c) {
          case ':':
            return true;
          default:
            return hypp::detail::IsUnreserved(c) || hypp::detail::IsSubDelim(c);
        }
      },
      [](const std::string_view view) {
        return hypp::detail::IsPctEncoded(view);
      });
}

// IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
Expected<std::string_view> ParseIpLiteral(Parser& parser) {
  // "["
  if (!parser.Skip('[')) {
    return Unexpected{Error::Invalid_URI_Host};
  }

  // IPvFuture = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
  //
  // > If a URI containing an IP-literal that starts with "v" (case-
  // insensitive), indicating that the version flag is present, is dereferenced
  // by an application that does not know the meaning of that version flag, then
  // the application should return an appropriate error for "address mechanism
  // not supported".
  // Reference: https://tools.ietf.org/html/rfc3986#section-3.2.2
  if (parser.Peek('v') || parser.Peek('V')) {
    return Unexpected{Error::Address_Mechanism_Not_Supported};
  }

  // IPv6address =                            6( h16 ":" ) ls32
  //             /                       "::" 5( h16 ":" ) ls32
  //             / [               h16 ] "::" 4( h16 ":" ) ls32
  //             / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
  //             / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
  //             / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
  //             / [ *4( h16 ":" ) h16 ] "::"              ls32
  //             / [ *5( h16 ":" ) h16 ] "::"              h16
  //             / [ *6( h16 ":" ) h16 ] "::"
  //
  // ls32        = ( h16 ":" h16 ) / IPv4address
  //             ; least-significant 32 bits of address
  //
  // h16         = 1*4HEXDIG
  //             ; 16 bits of address represented in hexadecimal
  const auto view = parser.Match(hypp::detail::limits::kURI,
      [](const char c) {
        // @TODO: Parse properly
        switch (c) {
          case ':':
          case '.':  // for IPv4address
            return true;
          default:
            return hypp::detail::IsHexDigit(c);
        }
      });
  if (view.empty()) {
    return Unexpected{Error::Invalid_URI_Host};
  }

  // "]"
  if (!parser.Skip(']')) {
    return Unexpected{Error::Invalid_URI_Host};
  }

  return view;
}

// IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
Expected<std::string_view> ParseIpV4Address(Parser& parser) {
  Parser ip_parser{parser};

  for (int i = 0; i < 4; ++i) {
    if (i > 0 && !ip_parser.Skip('.')) {
      return Unexpected{Error::Invalid_URI_Host};
    }
    const auto dec_octet = ip_parser.Match(hypp::detail::IsDecOctet);
    if (dec_octet.empty()) {
      return Unexpected{Error::Invalid_URI_Host};
    }
  }

  return parser.Read(parser.size() - ip_parser.size());
}

// reg-name = *( unreserved / pct-encoded / sub-delims )
Expected<std::string_view> ParseRegisteredName(Parser& parser) {
  return parser.Match(hypp::detail::limits::kURI,
      [](const char c) {
        return hypp::detail::IsUnreserved(c) || hypp::detail::IsSubDelim(c);
      },
      [](const std::string_view view) {
        return hypp::detail::IsPctEncoded(view);
      });
}

// host = IP-literal / IPv4address / reg-name
Expected<std::string_view> ParseUriHost(Parser& parser) {
  std::string_view view;

  // > The syntax rule for host is ambiguous because it does not completely
  // distinguish between an IPv4address and a reg-name. In order to
  // disambiguate the syntax, we apply the "first-match-wins" algorithm: If
  // host matches the rule for IPv4address, then it should be considered an
  // IPv4 address literal and not a reg-name.
  // Reference: https://tools.ietf.org/html/rfc3986#section-3.2.2
  if (auto expected = ParseIpLiteral(parser)) {
    view = expected.value();
  } else if (expected = ParseIpV4Address(parser)) {
    view = expected.value();
  } else if (expected = ParseRegisteredName(parser)) {
    view = expected.value();
  }

  // > A sender MUST NOT generate an "http" URI with an empty host identifier.
  // A recipient that processes such a URI reference MUST reject it as invalid.
  // Reference: https://tools.ietf.org/html/rfc7230#section-2.7.1
  if (view.empty()) {
    return Unexpected{Error::Invalid_URI_Host};
  }

  return view;
}

// port = *DIGIT
Expected<std::string_view> ParseUriPort(Parser& parser) {
  return parser.Match(hypp::detail::limits::kPort, hypp::detail::IsDigit);
}

// authority = [ userinfo "@" ] host [ ":" port ]
Expected<Uri::Authority> ParseUriAuthority(Parser& parser) {
  Uri::Authority authority;

  // [ userinfo "@" ]
  Parser user_info_parser{parser};
  if (const auto expected = ParseUriUserInfo(user_info_parser)) {
    if (user_info_parser.Skip('@')) {
      authority.user_info = expected.value();
      parser.Remove(parser.size() - user_info_parser.size());
    }
  } else {
    return Unexpected{expected.error()};
  }

  // host
  if (const auto expected = ParseUriHost(parser)) {
    authority.host = expected.value();
  } else {
    return Unexpected{expected.error()};
  }

  // [ ":" port ]
  if (parser.Skip(':')) {
    if (const auto expected = ParseUriPort(parser)) {
      authority.port = expected.value();
    } else {
      return Unexpected{expected.error()};
    }
  }

  return authority;
}

////////////////////////////////////////////////////////////////////////////////

// Reference: https://tools.ietf.org/html/rfc3986#section-3.3
// Reference: https://tools.ietf.org/html/rfc7230#section-2.7
enum UriPathRules {
  kUriPathAbEmpty  = 1 << 0,
  kUriAbsolutePath = 1 << 1,  // RFC 7230
  kUriPathAbsolute = 1 << 2,
  kUriPathNoScheme = 1 << 3,
  kUriPathRootless = 1 << 4,
  kUriPathEmpty    = 1 << 5,
};

// path = path-abempty   ; begins with "/" or is empty
//      / path-absolute  ; begins with "/" but not "//"
//      / path-noscheme  ; begins with a non-colon segment
//      / path-rootless  ; begins with a segment
//      / path-empty     ; zero characters
//      / absolute-path  ; begins with "/" (RFC 7230)
Expected<std::string_view> ParseUriPath(Parser& parser, const int flags) {
  // segment       = *pchar
  // segment-nz    = 1*pchar
  // segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
  //               ; non-zero-length segment without any colon ":"
  const auto parse_segment = [](Parser& parser) {
    return parser.Match(hypp::detail::limits::kURI, hypp::detail::IsPchar);
  };
  const auto parse_segment_nz_nc = [](Parser& parser) {
    Parser segment_parser{parser};
    auto view = segment_parser.Match(hypp::detail::limits::kURI,
                                     hypp::detail::IsPchar);
    if (const auto pos = view.find(':'); pos != view.npos) {
      view = view.substr(0, pos);
    }
    return parser.Read(parser.size() - segment_parser.size());
  };
  const auto parse_slash_segment = [](Parser& parser) {
    if (!parser.Peek('/')) {
      return std::string_view{};
    }
    return parser.Match(hypp::detail::limits::kURI,
        [](const char c) {
          return c == '/';
        },
        [](const std::string_view view) {
          return hypp::detail::IsPchar(view);
        });
  };

  // path-abempty = *( "/" segment )
  //              ; begins with "/" or is empty
  if (flags & kUriPathAbEmpty) {
    return parse_slash_segment(parser);
  }

  // absolute-path = 1*( "/" segment )
  //
  // > An "absolute-path" rule is defined for protocol elements that can contain
  // a non-empty path component. (This rule differs slightly from the
  // path-abempty rule of RFC 3986, which allows for an empty path to be used in
  // references, and path-absolute rule, which does not allow paths that begin
  // with "//".)
  // Reference: https://tools.ietf.org/html/rfc7230#section-2.7
  if (flags & kUriAbsolutePath) {
    if (!parser.Peek('/')) {
      return Unexpected{Error::Invalid_URI_Path};
    }
    return parse_slash_segment(parser);
  }

  // path-absolute = "/" [ segment-nz *( "/" segment ) ]
  //               ; begins with "/" but not "//"
  if (flags & kUriPathAbsolute) {
    Parser path_parser{parser};
    if (path_parser.Skip('/')) {
      if (path_parser.Peek('/')) {
        return Unexpected{Error::Invalid_URI_Path};
      }
      const auto segment_nz = parse_segment(path_parser);
      if (!segment_nz.empty()) {
        parse_slash_segment(path_parser);
      }
      return parser.Read(parser.size() - path_parser.size());
    }
  }

  // path-noscheme = segment-nz-nc *( "/" segment )
  //               ; begins with a non-colon segment
  if (flags & kUriPathNoScheme) {
    Parser path_parser{parser};
    const auto segment_nz_nc = parse_segment_nz_nc(path_parser);
    if (!segment_nz_nc.empty()) {
      parse_slash_segment(path_parser);
      return parser.Read(parser.size() - path_parser.size());
    }
  }

  // path-rootless = segment-nz *( "/" segment )
  //               ; begins with a segment
  if (flags & kUriPathRootless) {
    Parser path_parser{parser};
    const auto segment_nz = parse_segment(path_parser);
    if (!segment_nz.empty()) {
      parse_slash_segment(path_parser);
      return parser.Read(parser.size() - path_parser.size());
    }
  }

  // path-empty = 0<pchar>
  //            ; zero characters
  if (flags & kUriPathEmpty) {
    return std::string_view{};
  }

  return Unexpected{Error::Invalid_URI_Path};
}

////////////////////////////////////////////////////////////////////////////////

// query = *( pchar / "/" / "?" )
Expected<std::string_view> ParseUriQuery(Parser& parser) {
  return parser.Match(hypp::detail::limits::kURI,
      [](const char c) {
        return c == '/' || c == '?';
      },
      [](const std::string_view view) {
        return hypp::detail::IsPchar(view);
      });
}

// fragment = *( pchar / "/" / "?" )
Expected<std::string_view> ParseUriFragment(Parser& parser) {
  return ParseUriQuery(parser);  // same ABNF rule
}

////////////////////////////////////////////////////////////////////////////////

// absolute-URI = scheme ":" hier-part [ "?" query ]
Expected<Uri> ParseAbsoluteUri(Parser& parser) {
  Uri uri;

  // scheme ":"
  if (const auto expected = ParseUriScheme(parser)) {
    uri.scheme = expected.value();
  } else {
    return Unexpected{expected.error()};
  }
  if (!parser.Skip(':')) {
    return Unexpected{Error::Invalid_URI};
  }

  // hier-part = "//" authority path-abempty
  //           / path-absolute
  //           / path-rootless
  //           / path-empty
  if (parser.Skip("//")) {
    if (const auto expected = ParseUriAuthority(parser)) {
      uri.authority = expected.value();
    } else {
      return Unexpected{expected.error()};
    }
  }
  const auto kPathRules = uri.authority ? kUriPathAbEmpty :
      kUriPathAbsolute | kUriPathRootless | kUriPathEmpty;
  if (const auto expected = ParseUriPath(parser, kPathRules)) {
    uri.path = expected.value();
  } else {
    return Unexpected{expected.error()};
  }

  // [ "?" query ]
  if (parser.Skip('?')) {
    if (const auto expected = ParseUriQuery(parser)) {
      uri.query = expected.value();
    } else {
      return Unexpected{expected.error()};
    }
  }

  return uri;
}

// URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
Expected<Uri> ParseUri(Parser& parser) {
  Uri uri;

  // Same components as absolute-URI
  if (const auto expected = ParseAbsoluteUri(parser)) {
    uri = expected.value();
  } else {
    return Unexpected{expected.error()};
  }

  // [ "#" fragment ]
  if (parser.Skip('#')) {
    if (const auto expected = ParseUriFragment(parser)) {
      uri.fragment = expected.value();
    } else {
      return Unexpected{expected.error()};
    }
  }

  return uri;
}

// partial-URI = relative-part [ "?" query ]
Expected<Uri> ParsePartialUri(Parser& parser) {
  Uri uri;

  // relative-part = "//" authority path-abempty
  //               / path-absolute
  //               / path-noscheme
  //               / path-empty
  if (parser.Skip("//")) {
    if (const auto expected = ParseUriAuthority(parser)) {
      uri.authority = expected.value();
    } else {
      return Unexpected{expected.error()};
    }
  }
  const auto kPathRules = uri.authority ? kUriPathAbEmpty :
      kUriPathAbsolute | kUriPathNoScheme | kUriPathEmpty;
  if (const auto expected = ParseUriPath(parser, kPathRules)) {
    uri.path = expected.value();
  } else {
    return Unexpected{expected.error()};
  }

  // [ "?" query ]
  if (parser.Skip('?')) {
    if (const auto expected = ParseUriQuery(parser)) {
      uri.query = expected.value();
    } else {
      return Unexpected{expected.error()};
    }
  }

  return uri;
}

// relative-ref = relative-part [ "?" query ] [ "#" fragment ]
Expected<Uri> ParseRelativeReference(Parser& parser) {
  Uri uri;

  // Same components as partial-URI
  if (const auto expected = ParsePartialUri(parser)) {
    uri = expected.value();
  } else {
    return Unexpected{expected.error()};
  }

  // [ "#" fragment ]
  if (parser.Skip('#')) {
    if (const auto expected = ParseUriFragment(parser)) {
      uri.fragment = expected.value();
    } else {
      return Unexpected{expected.error()};
    }
  }

  return uri;
}

// URI-reference = URI / relative-ref
Expected<Uri> ParseUriReference(Parser& parser) {
  Uri uri;

  // > If the URI-reference's prefix does not match the syntax of a scheme
  // followed by its colon separator, then the URI-reference is a relative
  // reference.
  // Reference: https://tools.ietf.org/html/rfc3986#section-4.1
  if (auto expected = ParseUri(parser)) {
    uri = expected.value();
  } else if (expected = ParseRelativeReference(parser)) {
    uri = expected.value();
  } else {
    return Unexpected{expected.error()};
  }

  return uri;
}

}  // namespace hypp::parser
