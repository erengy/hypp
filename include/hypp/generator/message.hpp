#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/generator/header.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/message.hpp>

namespace hypp {

// HTTP-message = start-line *( header-field CRLF ) CRLF [ message-body ]
template <typename StartLine>
std::string to_string(const Message<StartLine>& message) {
  using namespace parser::syntax;
  return detail::concat(
      to_string(message.start_line),
      to_string(message.header),
      kCRLF,
      message.body);
}

}  // namespace hypp
