#pragma once

namespace hypp::parser {

// > HTTP does not define specific error handling mechanisms except when they
// have a direct impact on security, since different applications of the
// protocol require different error handling strategies.
// Reference: https://tools.ietf.org/html/rfc7230#section-2.5

enum class Error {
  // HTTP status codes
  OK,                               // 200
  Bad_Request,                      // 400
  Payload_Too_Large,                // 413
  URI_Too_Long,                     // 414
  Upgrade_Required,                 // 426
  Request_Header_Fields_Too_Large,  // 431
  Not_Implemented,                  // 501
  HTTP_Version_Not_Supported,       // 505

  // Header
  Invalid_Header_Format,
  Invalid_Header_Name,

  // Method
  Invalid_Method,

  // Response
  Bad_Response,

  // Status
  Invalid_Status_Code,

  // URI
  Address_Mechanism_Not_Supported,
  Invalid_URI,
  Invalid_URI_Scheme,
  Invalid_URI_Host,
  Invalid_URI_Path,

  // HTTP version
  Invalid_HTTP_Name,
  Invalid_HTTP_Version,
};

}  // namespace hypp::parser
