#pragma once

#include <string>

#include <hypp/detail/expected.hpp>
#include <hypp/status.hpp>

namespace hypp {

// > HTTP does not define specific error handling mechanisms except when they
// have a direct impact on security, since different applications of the
// protocol require different error handling strategies.
// Reference: https://tools.ietf.org/html/rfc7230#section-2.5

enum class Error {
  // HTTP status codes
  Bad_Request,
  Payload_Too_Large,
  URI_Too_Long,
  Upgrade_Required,
  Request_Header_Fields_Too_Large,
  Not_Implemented,
  HTTP_Version_Not_Supported,

  // Header
  Invalid_Header_Format,
  Invalid_Header_Name,

  // Method
  Invalid_Method,

  // Request
  Invalid_Request_Target,

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

using Unexpected = detail::Unexpected<Error>;

template <typename T>
using Expected = detail::Expected<T, Error>;

inline std::string to_string(const Error error) {
  switch (error) {
    case Error::Bad_Request:
      return status::to_phrase(status::k400_Bad_Request);
    case Error::Payload_Too_Large:
      return status::to_phrase(status::k413_Payload_Too_Large);
    case Error::URI_Too_Long:
      return status::to_phrase(status::k414_URI_Too_Long);
    case Error::Upgrade_Required:
      return status::to_phrase(status::k426_Upgrade_Required);
    case Error::Request_Header_Fields_Too_Large:
      return status::to_phrase(status::k431_Request_Header_Fields_Too_Large);
    case Error::Not_Implemented:
      return status::to_phrase(status::k501_Not_Implemented);
    case Error::HTTP_Version_Not_Supported:
      return status::to_phrase(status::k505_HTTP_Version_Not_Supported);
    case Error::Invalid_Header_Format:
      return "Invalid Header Format";
    case Error::Invalid_Header_Name:
      return "Invalid Header Name";
    case Error::Invalid_Method:
      return "Invalid Method";
    case Error::Invalid_Request_Target:
      return "Invalid Request Target";
    case Error::Bad_Response:
      return "Bad Response";
    case Error::Invalid_Status_Code:
      return "Invalid Status Code";
    case Error::Address_Mechanism_Not_Supported:
      return "Address Mechanism Not Supported";
    case Error::Invalid_URI:
      return "Invalid URI";
    case Error::Invalid_URI_Scheme:
      return "Invalid URI Scheme";
    case Error::Invalid_URI_Host:
      return "Invalid URI Host";
    case Error::Invalid_URI_Path:
      return "Invalid URI Path";
    case Error::Invalid_HTTP_Name:
      return "Invalid HTTP Name";
    case Error::Invalid_HTTP_Version:
      return "Invalid HTTP Version";
    default:
      return "Unknown Error";
  }
}

}  // namespace hypp
