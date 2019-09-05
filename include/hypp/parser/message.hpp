#pragma once

#include <string_view>

#include <hypp/detail/limits.hpp>
#include <hypp/detail/parser.hpp>
#include <hypp/detail/syntax.hpp>
#include <hypp/parser/header.hpp>
#include <hypp/error.hpp>
#include <hypp/message.hpp>

namespace hypp {

// message-body = *OCTET
Expected<std::string_view> ParseMessageBody(Parser& parser) {
  if (parser.size() > detail::limits::kBody) {
    return Unexpected{Error::Payload_Too_Large};
  }

  return parser.read_all();
}

// HTTP-message = start-line *( header-field CRLF ) CRLF [ message-body ]
template <typename MessageT>
Expected<MessageT> ParseMessage(const std::string_view view) {
  MessageT message;

  Parser parser{view};

  // start-line
  if (const auto expected = ParseStartLine(parser, message)) {
    message.start_line = expected.value();
  } else {
    return Unexpected{expected.error()};
  }

  // > A recipient that receives whitespace between the start-line and the
  // first header field MUST either reject the message as invalid or consume
  // each whitespace-preceded line without further processing of it.
  // Reference: https://tools.ietf.org/html/rfc7230#section-3
  if (parser.strip(detail::syntax::kWhitespace)) {
    return Unexpected{Error::Invalid_Header_Format};
  }

  // *( header-field CRLF ) CRLF
  if (const auto expected = ParseHeaderFields(parser)) {
    message.header_fields = expected.value();
  } else {
    return Unexpected{expected.error()};
  }
  if (!parser.skip(detail::syntax::kCRLF)) {
    return Unexpected{Error::Invalid_Header_Format};
  }

  // [ message-body ]
  if (const auto expected = ParseMessageBody(parser)) {
    message.body = expected.value();
  } else {
    return Unexpected{expected.error()};
  }

  return message;
}

}  // namespace hypp
