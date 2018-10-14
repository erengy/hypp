#pragma once

#include <string>

#include <hypp/detail/util.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/header.hpp>

namespace hypp {

template <typename StartLine>
class Message {
public:
  constexpr Message() = default;

  // HTTP-message = start-line *( header-field CRLF ) CRLF [ message-body ]
  std::string to_string() const {
    using namespace parser::syntax;
    return detail::util::concat(
        start_line.to_string(), header.to_string(), kCRLF, body);
  }

  StartLine start_line;  // start-line = request-line / status-line
  Header header;
  std::string body;
};

}  // namespace hypp
