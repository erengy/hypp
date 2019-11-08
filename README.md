# hypp

hypp is a client-oriented HTTP library for C++. It can parse and generate HTTP/1.1 messages and URIs.

To send or receive HTTP messages, you must use hypp along with another library such as [hypr](https://github.com/erengy/hypr).

## Conformance

hypp aims to conform to the syntax and semantics of HTTP/1.1 elements according to the following specification documents:

- [RFC 3986](https://tools.ietf.org/html/rfc3986) - Uniform Resource Identifier (URI): Generic Syntax
- [RFC 5234](https://tools.ietf.org/html/rfc5234) - Augmented BNF for Syntax Specifications: ABNF
- [RFC 7230](https://tools.ietf.org/html/rfc7230) - Hypertext Transfer Protocol (HTTP/1.1): Message Syntax and Routing
- [RFC 7231](https://tools.ietf.org/html/rfc7231) - Hypertext Transfer Protocol (HTTP/1.1): Semantics and Content

### Exceptions

- OPTIONAL requirements
- Obsolete grammar rules (e.g. obsolete line folding)

## Error Handling

hypp's parser functions return an `Expected` object, which contains either the expected result or an `Error`. For more information, see the `std::expected` proposal or the Usage section below.

hypp treats unsupported HTTP versions as errors, and does not attempt to recover usable protocol elements from invalid constructs.

## Usage

***This is a work in progress. Usage in public applications is not yet recommended.***

```cpp
#include <iostream>
#include <hypp.hpp>

int main() {
  // Returns hypp::Expected<hypp::Response>
  const auto expected = hypp::ParseResponse(
        // Example response from RFC 7230 Section 2.1
        "HTTP/1.1 200 OK\r\n"
        "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
        "Server: Apache\r\n"
        "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n"
        "ETag: \"34aa387-d-1568eb00\"\r\n"
        "Accept-Ranges: bytes\r\n"
        "Content-Length: 51\r\n"
        "Vary: Accept-Encoding\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello World! My payload includes a trailing CRLF.\r\n"
      );

  if (!expected) {
    std::cout << "Error: " << hypp::to_string(expected.error()) << '\n';
    return 1;
  }

  const auto& r = expected.value();
  std::cout << r.start_line.code << '\n';             // 200
  std::cout << r.header_fields.back().value << '\n';  // text/plain
  std::cout << r.body.substr(0, 12) << '\n';          // Hello World!

  return 0;
}
```

## License

Licensed under the [MIT License](https://opensource.org/licenses/MIT).
