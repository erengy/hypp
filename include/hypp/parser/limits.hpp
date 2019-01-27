#pragma once

#include <cstdint>
#include <limits>

namespace hypp::parser::limits {

// > When a received protocol element is parsed, the recipient MUST be able to
// parse any value of reasonable length. (...) HTTP does not have specific
// length limitations for many of its protocol elements because the lengths that
// might be appropriate will vary widely, depending on the deployment context
// and purpose of the implementation. (...) At a minimum, a recipient MUST be
// able to parse and process protocol element lengths that are at least as long
// as the values that it generates for those same protocol elements in other
// messages.
// Reference: https://tools.ietf.org/html/rfc7230#section-2.5

// > It is RECOMMENDED that all HTTP senders and recipients support, at a
// minimum, request-line lengths of 8000 octets.
// Reference: https://tools.ietf.org/html/rfc7230#section-3.1.1

// > HTTP does not place a predefined limit on the length of each header field
// or on the length of the header section as a whole.
// Reference: https://tools.ietf.org/html/rfc7230#section-3.2.5

// > These are minimum recommendations, chosen to be supportable even by
// implementations with limited resources; it is expected that most
// implementations will choose substantially higher limits.
// Reference: https://tools.ietf.org/html/rfc7230#section-9.3

// > Recipients ought to carefully limit the extent to which they process other
// protocol elements, including (but not limited to) request methods, response
// status phrases, header field-names, numeric values, and body chunks.
// Reference: https://tools.ietf.org/html/rfc7230#section-9.3

#define EXACTLY(x) (x)
#define ARBITRARY(x) (x)

constexpr size_t kMaxLimit = ARBITRARY(std::numeric_limits<std::uint16_t>::max());

// Generic
constexpr size_t kHttpName     = EXACTLY(4);     // "HTTP"
constexpr size_t kBody         = kMaxLimit;

// Request line
constexpr size_t kRequestLine  = kMaxLimit;
constexpr size_t kHttpVersion  = EXACTLY(8);     // "HTTP/1.1"
constexpr size_t kMethod       = ARBITRARY(64);  // "GET", "POST", "UPDATEREDIRECTREF", etc.

// Status line
constexpr size_t kStatusLine   = kMaxLimit;
constexpr size_t kStatusCode   = EXACTLY(3);     // "200", "404", etc.
constexpr size_t kReasonPhrase = ARBITRARY(64);  // "OK", "Bad Request", "Network Authentication Required", etc.

// Header fields
constexpr size_t kHeaderFields = kMaxLimit;
constexpr size_t kFieldName    = kMaxLimit;
constexpr size_t kFieldValue   = kMaxLimit;

// URI
constexpr size_t kURI          = kMaxLimit;
constexpr size_t kScheme       = ARBITRARY(64);  // "http", "https", "file", "ftp", "mailto", "tel", etc.
constexpr size_t kPort         = ARBITRARY(16);  // "80", "8888", etc.

#undef EXACTLY
#undef ARBITRARY

}  // namespace hypp::parser::limits
