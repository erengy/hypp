#pragma once

#include <optional>
#include <string>

namespace hypp {

class Uri {
public:
  constexpr Uri() = default;

  class Authority {
  public:
    constexpr Authority() = default;

    std::optional<std::string> user_info;
    std::string host;
    std::optional<std::string> port;
  };

  std::optional<std::string> scheme;
  std::optional<Authority> authority;
  std::string path;
  std::optional<std::string> query;
  std::optional<std::string> fragment;
};

}  // namespace hypp
