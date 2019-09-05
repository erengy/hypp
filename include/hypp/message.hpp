#pragma once

#include <string>

#include <hypp/header.hpp>

namespace hypp {

template <typename StartLine>
struct Message {
  StartLine start_line;  // start-line = request-line / status-line
  HeaderFields header_fields;
  std::string body;
};

}  // namespace hypp
