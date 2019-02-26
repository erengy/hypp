#pragma once

#include <string>

#include <hypp/message.hpp>
#include <hypp/status.hpp>
#include <hypp/version.hpp>

namespace hypp {

struct StatusLine {
  Version version;
  status::code_t code = status::Code::k200_OK;
};

using Response = Message<StatusLine>;

}  // namespace hypp
