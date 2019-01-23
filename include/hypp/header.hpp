#pragma once

#include <string>
#include <vector>

namespace hypp {

struct Header {
  struct Field {
    std::string name;
    std::string value;
  };

  std::vector<Field> fields;
};

}  // namespace hypp
