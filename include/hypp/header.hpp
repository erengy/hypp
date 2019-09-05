#pragma once

#include <string>
#include <vector>

namespace hypp {

struct HeaderField {
  std::string name;
  std::string value;
};

using HeaderFields = std::vector<HeaderField>;

}  // namespace hypp
