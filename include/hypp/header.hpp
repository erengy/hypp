#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace hypp {

class Header {
public:
  constexpr Header() = default;

  class Field {
  public:
    constexpr Field() = default;
    constexpr Field(const std::string_view name, const std::string_view value)
        : name{name}, value{value} {}

    std::string name;
    std::string value;
  };

  std::vector<Field> fields;
};

}  // namespace hypp
