#pragma once

#include <string>
#include <string_view>

namespace hypp {

class Method {
public:
  constexpr Method() = default;
  constexpr Method(const std::string_view method) : method_{method} {}

  // Reference: https://tools.ietf.org/html/rfc7231#section-4.1
  // Reference: https://www.iana.org/assignments/http-methods
  static constexpr auto kGet = "GET";          // [RFC7231, Section 4.3.1]
  static constexpr auto kHead = "HEAD";        // [RFC7231, Section 4.3.2]
  static constexpr auto kPost = "POST";        // [RFC7231, Section 4.3.3]
  static constexpr auto kPut = "PUT";          // [RFC7231, Section 4.3.4]
  static constexpr auto kDelete = "DELETE";    // [RFC7231, Section 4.3.5]
  static constexpr auto kConnect = "CONNECT";  // [RFC7231, Section 4.3.6]
  static constexpr auto kOptions = "OPTIONS";  // [RFC7231, Section 4.3.7]
  static constexpr auto kTrace = "TRACE";      // [RFC7231, Section 4.3.8]

  std::string to_string() const {
    return method_;
  }

private:
  std::string method_;
};

}  // namespace hypp
