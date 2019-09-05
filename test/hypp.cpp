#include <cassert>
#include <initializer_list>
#include <iostream>

#include <hypp.hpp>

namespace {

void test_version(const hypp::Version& version,
                  const char major, const char minor) {
  assert(version.major == major);
  assert(version.minor == minor);
}

void test_header_fields(const hypp::HeaderFields& header_fields,
    const std::initializer_list<hypp::HeaderField>& fields) {
  assert(header_fields.size() == fields.size());

  for (auto it = fields.begin(); it != fields.end(); ++it) {
    const size_t i = std::distance(fields.begin(), it);
    assert(header_fields[i].name == it->name);
    assert(header_fields[i].value == it->value);
  }
}

void test_request() {
  constexpr auto example =
      // RFC 7230 Section 2.1
      "GET /hello.txt HTTP/1.1\r\n"
      "User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3\r\n"
      "Host: www.example.com\r\n"
      "Accept-Language: en, mi\r\n"
      "\r\n";

  const auto expected = hypp::ParseRequest(example);
  assert(expected);
  const auto& r = expected.value();

  assert(r.start_line.method == "GET");
  assert(r.start_line.target.form == hypp::RequestTarget::Form::Origin);
  assert(r.start_line.target.uri.path == "/hello.txt");
  test_version(r.start_line.version, '1', '1');
  test_header_fields(r.header_fields, {
      {"User-Agent", "curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3"},
      {"Host", "www.example.com"},
      {"Accept-Language", "en, mi"},
    });
  assert(r.body.empty());

  assert(hypp::to_string(r) == example);
}

void test_response() {
  constexpr auto example =
      // RFC 7230 Section 2.1
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
      "Hello World! My payload includes a trailing CRLF.\r\n";

  const auto expected = hypp::ParseResponse(example);
  assert(expected);
  const auto& r = expected.value();

  test_version(r.start_line.version, '1', '1');
  assert(r.start_line.code == 200);
  test_header_fields(r.header_fields, {
      {"Date", "Mon, 27 Jul 2009 12:28:53 GMT"},
      {"Server", "Apache"},
      {"Last-Modified", "Wed, 22 Jul 2009 19:15:56 GMT"},
      {"ETag", "\"34aa387-d-1568eb00\""},
      {"Accept-Ranges", "bytes"},
      {"Content-Length", "51"},
      {"Vary", "Accept-Encoding"},
      {"Content-Type", "text/plain"},
    });
  assert(r.body == "Hello World! My payload includes a trailing CRLF.\r\n");

  assert(hypp::to_string(r) == example);
}

}  // namespace

int main() {
  test_request();
  test_response();
  std::cout << "Passed all tests!\n";
  return 0;
}
