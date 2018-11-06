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
  static constexpr auto kAcl = "ACL";                              // [RFC3744, Section 8.1]
  static constexpr auto kBaselineControl = "BASELINE-CONTROL";     // [RFC3253, Section 12.6]
  static constexpr auto kBind = "BIND";                            // [RFC5842, Section 4]
  static constexpr auto kCheckIn = "CHECKIN";                      // [RFC3253, Section 4.4, Section 9.4]
  static constexpr auto kCheckOut = "CHECKOUT";                    // [RFC3253, Section 4.3, Section 8.8]
  static constexpr auto kConnect = "CONNECT";                      // [RFC7231, Section 4.3.6]
  static constexpr auto kCopy = "COPY";                            // [RFC4918, Section 9.8]
  static constexpr auto kDelete = "DELETE";                        // [RFC7231, Section 4.3.5]
  static constexpr auto kGet = "GET";                              // [RFC7231, Section 4.3.1]
  static constexpr auto kHead = "HEAD";                            // [RFC7231, Section 4.3.2]
  static constexpr auto kLabel = "LABEL";                          // [RFC3253, Section 8.2]
  static constexpr auto kLink = "LINK";                            // [RFC2068, Section 19.6.1.2]
  static constexpr auto kLock = "LOCK";                            // [RFC4918, Section 9.10]
  static constexpr auto kMerge = "MERGE";                          // [RFC3253, Section 11.2]
  static constexpr auto kMkActivity = "MKACTIVITY";                // [RFC3253, Section 13.5]
  static constexpr auto kMkCalendar = "MKCALENDAR";                // [RFC4791, Section 5.3.1] [RFC8144, Section 2.3]
  static constexpr auto kMkCol = "MKCOL";                          // [RFC4918, Section 9.3] [RFC5689, Section 3] [RFC8144, Section 2.3]
  static constexpr auto kMkRedirectRef = "MKREDIRECTREF";          // [RFC4437, Section 6]
  static constexpr auto kMkWorkspace = "MKWORKSPACE";              // [RFC3253, Section 6.3]
  static constexpr auto kMove = "MOVE";                            // [RFC4918, Section 9.9]
  static constexpr auto kOptions = "OPTIONS";                      // [RFC7231, Section 4.3.7]
  static constexpr auto kOrderPatch = "ORDERPATCH";                // [RFC3648, Section 7]
  static constexpr auto kPatch = "PATCH";                          // [RFC5789, Section 2]
  static constexpr auto kPost = "POST";                            // [RFC7231, Section 4.3.3]
  static constexpr auto kPri = "PRI";                              // [RFC7540, Section 3.5]
  static constexpr auto kPropFind = "PROPFIND";                    // [RFC4918, Section 9.1] [RFC8144, Section 2.1]
  static constexpr auto kPropPatch = "PROPPATCH";                  // [RFC4918, Section 9.2] [RFC8144, Section 2.2]
  static constexpr auto kPut = "PUT";                              // [RFC7231, Section 4.3.4]
  static constexpr auto kRebind = "REBIND";                        // [RFC5842, Section 6]
  static constexpr auto kReport = "REPORT";                        // [RFC3253, Section 3.6] [RFC8144, Section 2.1]
  static constexpr auto kSearch = "SEARCH";                        // [RFC5323, Section 2]
  static constexpr auto kTrace = "TRACE";                          // [RFC7231, Section 4.3.8]
  static constexpr auto kUnbind = "UNBIND";                        // [RFC5842, Section 5]
  static constexpr auto kUncheckout = "UNCHECKOUT";                // [RFC3253, Section 4.5]
  static constexpr auto kUnlink = "UNLINK";                        // [RFC2068, Section 19.6.1.3]
  static constexpr auto kUnlock = "UNLOCK";                        // [RFC4918, Section 9.11]
  static constexpr auto kUpdate = "UPDATE";                        // [RFC3253, Section 7.1]
  static constexpr auto kUpdateRedirectRef = "UPDATEREDIRECTREF";  // [RFC4437, Section 7]
  static constexpr auto kVersionControl = "VERSION-CONTROL";       // [RFC3253, Section 3.5]

  std::string to_string() const {
    return method_;
  }

private:
  std::string method_;
};

}  // namespace hypp
