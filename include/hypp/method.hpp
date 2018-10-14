#pragma once

#include <string>
#include <string_view>

namespace hypp {

class Method {
public:
  constexpr Method() = default;
  constexpr Method(const std::string_view method) : method_{method} {}

  // @TODO: Add all methods in HTTP Method Registry
  // Reference: https://tools.ietf.org/html/rfc7231#section-4.1
  // Reference: https://tools.ietf.org/html/rfc5789
  // Reference: https://www.iana.org/assignments/http-methods
  static constexpr auto kConnect = "CONNECT";
  static constexpr auto kDelete = "DELETE";
  static constexpr auto kGet = "GET";
  static constexpr auto kHead = "HEAD";
  static constexpr auto kOptions = "OPTIONS";
  static constexpr auto kPatch = "PATCH";
  static constexpr auto kPost = "POST";
  static constexpr auto kPut = "PUT";
  static constexpr auto kTrace = "TRACE";

  std::string to_string() const {
    return method_;
  }

private:
  std::string method_;
};

}  // namespace hypp
