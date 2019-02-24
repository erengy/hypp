#pragma once

#include <hypp/detail/limits.hpp>
#include <hypp/detail/parser.hpp>
#include <hypp/detail/syntax.hpp>
#include <hypp/parser/error.hpp>
#include <hypp/header.hpp>

namespace hypp::parser {

Expected<std::string_view> ParseHeaderFieldName(Parser& parser) {
  // field-name = token
  const auto name = parser.Match(hypp::detail::limits::kFieldName,
                                 hypp::detail::IsTchar);
  if (name.empty()) {
    return Unexpected{Error::Invalid_Header_Name};
  }
  return name;
}

Expected<std::string_view> ParseHeaderFieldValue(Parser& parser) {
  // field-value   = *( field-content / obs-fold )
  // field-content = field-vchar [ 1*( SP / HTAB ) field-vchar ]
  // field-vchar   = VCHAR / obs-text
  //
  // https://www.rfc-editor.org/errata/eid4189
  // https://github.com/httpwg/http-core/issues/19
  //
  // Note that empty values are allowed.
  return parser.Match(hypp::detail::limits::kFieldValue,
      [&parser](const char c) {
        switch (c) {
          default:
            return hypp::detail::IsVchar(c) || hypp::detail::IsObsText(c);
          case hypp::detail::syntax::kSP:
          case hypp::detail::syntax::kHTAB:
            return parser.Peek(hypp::detail::IsVchar) ||
                   parser.Peek(hypp::detail::IsObsText);
        }
      });
}

Expected<Header::Field> ParseHeaderField(Parser& parser) {
  Header::Field header_field;

  // field-name
  if (const auto expected = ParseHeaderFieldName(parser)) {
    header_field.name = expected.value();
  } else {
    return Unexpected{expected.error()};
  }

  // > No whitespace is allowed between the header field-name and colon.
  // Reference: https://tools.ietf.org/html/rfc7230#section-3.2.4

  // ":"
  if (!parser.Skip(':')) {
    return Unexpected{Error::Invalid_Header_Format};
  }

  // OWS
  parser.Strip(hypp::detail::syntax::kWhitespace);

  // field-value
  if (const auto expected = ParseHeaderFieldValue(parser)) {
    header_field.value = expected.value();
  } else {
    return Unexpected{expected.error()};
  }

  // OWS
  parser.Strip(hypp::detail::syntax::kWhitespace);

  return header_field;
}

Expected<Header> ParseHeaderFields(Parser& parser) {
  Header header;

  const auto initial_size = parser.size();

  while (!parser.empty()) {
    if (initial_size - parser.size() > hypp::detail::limits::kHeaderFields) {
      return Unexpected{Error::Request_Header_Fields_Too_Large};
    }

    if (parser.Peek(hypp::detail::syntax::kCRLF)) {
      break;  // Empty line indicates the end of the header section
    }

    // *( header-field CRLF )
    if (const auto expected = ParseHeaderField(parser)) {
      header.fields.push_back(std::move(expected.value()));
    } else {
      return Unexpected{expected.error()};
    }
    if (!parser.Skip(hypp::detail::syntax::kCRLF)) {
      return Unexpected{Error::Invalid_Header_Format};
    }
  }

  return header;
}

}  // namespace hypp::parser
