#pragma once

#include <string>

namespace hypp {

class HttpVersion {
public:
  constexpr HttpVersion() = default;
  constexpr HttpVersion(const char major, const char minor)
      : major{major}, minor{minor} {}

  char major = '\0';
  char minor = '\0';
};

}  // namespace hypp
