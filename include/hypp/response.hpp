#pragma once

#include <string>

#include <hypp/message.hpp>
#include <hypp/status.hpp>
#include <hypp/version.hpp>

namespace hypp {

class StatusLine {
public:
  constexpr StatusLine() = default;

  HttpVersion version;
  StatusCode code;
};

class Response : public Message<StatusLine> {
public:
  constexpr Response() = default;
};

}  // namespace hypp
