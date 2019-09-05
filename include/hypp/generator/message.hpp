#pragma once

#include <string>

#include <hypp/detail/syntax.hpp>
#include <hypp/detail/util.hpp>
#include <hypp/generator/header.hpp>
#include <hypp/message.hpp>

namespace hypp {

// HTTP-message = start-line *( header-field CRLF ) CRLF [ message-body ]
template <typename StartLine>
std::string to_string(const Message<StartLine>& message) {
  using namespace detail::syntax;
  return detail::concat(
      to_string(message.start_line),
      to_string(message.header_fields),
      kCRLF,
      message.body);
}

}  // namespace hypp
