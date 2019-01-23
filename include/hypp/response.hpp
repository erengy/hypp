#pragma once

#include <string>

#include <hypp/message.hpp>
#include <hypp/status.hpp>
#include <hypp/version.hpp>

namespace hypp {

struct StatusLine {
  HttpVersion version;
  StatusCode code;
};

using Response = Message<StatusLine>;

}  // namespace hypp
