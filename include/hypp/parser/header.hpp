#pragma once

#include <hypp/detail/parser.hpp>
#include <hypp/parser/error.hpp>
#include <hypp/parser/expected.hpp>
#include <hypp/parser/limits.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/header.hpp>

namespace hypp::parser {

Expected<Header::Field> ParseHeaderField(Parser& parser) {
  // field-name = token
  const auto name = parser.Match(limits::kFieldName, syntax::IsTchar);
  if (name.empty()) {
    return Unexpected{Error::Invalid_Header_Name};
  }

  // > No whitespace is allowed between the header field-name and colon.
  // Reference: https://tools.ietf.org/html/rfc7230#section-3.2.4

  // ":"
  if (!parser.Skip(':')) {
    return Unexpected{Error::Invalid_Header_Format};
  }

  // OWS
  parser.Strip(syntax::kWhitespace);

  // field-value   = *( field-content / obs-fold )
  // field-content = field-vchar [ 1*( SP / HTAB ) field-vchar ]
  // field-vchar   = VCHAR / obs-text
  const auto value = parser.Match(limits::kFieldValue,
      [&parser](const char c) {
        switch (c) {
          default:
            return syntax::IsVchar(c) || syntax::IsObsText(c);
          case syntax::kSP:
          case syntax::kHTAB:
            return parser.Peek(syntax::IsVchar);
        }
      });
  if (value.empty()) {
    // Empty values are allowed
  }

  // OWS
  parser.Strip(syntax::kWhitespace);

  return Header::Field{name, value};
}

Expected<Header> ParseHeaderFields(Parser& parser) {
  Header header;

  const auto initial_size = parser.size();

  while (!parser.empty()) {
    if (initial_size - parser.size() > limits::kHeaderFields) {
      return Unexpected{Error::Request_Header_Fields_Too_Large};
    }

    if (parser.Peek(syntax::kCRLF)) {
      break;  // Empty line indicates the end of the header section
    }

    // *( header-field CRLF )
    if (const auto expected = ParseHeaderField(parser)) {
      header.fields.push_back(std::move(expected.value()));
    } else {
      return Unexpected{expected.error()};
    }
    if (!parser.Skip(syntax::kCRLF)) {
      return Unexpected{Error::Invalid_Header_Format};
    }
  }

  return header;
}

}  // namespace hypp::parser
