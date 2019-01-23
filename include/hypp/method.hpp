#pragma once

namespace hypp::method {

// Reference: https://tools.ietf.org/html/rfc7231#section-4.1
// Reference: https://www.iana.org/assignments/http-methods
constexpr auto kGet = "GET";          // [RFC7231, Section 4.3.1]
constexpr auto kHead = "HEAD";        // [RFC7231, Section 4.3.2]
constexpr auto kPost = "POST";        // [RFC7231, Section 4.3.3]
constexpr auto kPut = "PUT";          // [RFC7231, Section 4.3.4]
constexpr auto kDelete = "DELETE";    // [RFC7231, Section 4.3.5]
constexpr auto kConnect = "CONNECT";  // [RFC7231, Section 4.3.6]
constexpr auto kOptions = "OPTIONS";  // [RFC7231, Section 4.3.7]
constexpr auto kTrace = "TRACE";      // [RFC7231, Section 4.3.8]

}  // namespace hypp::method
