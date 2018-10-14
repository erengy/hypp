#pragma once

#include <hypp/detail/parser.hpp>
#include <hypp/parser/error.hpp>
#include <hypp/parser/expected.hpp>
#include <hypp/parser/limits.hpp>
#include <hypp/parser/syntax.hpp>
#include <hypp/status.hpp>

namespace hypp::parser {

Expected<StatusCode> ParseStatusCode(Parser& parser) {
  // status-code = 3DIGIT
  const auto view = parser.Match(limits::kStatusCode, syntax::IsDigit);

  if (view.size() != limits::kStatusCode) {
    return Unexpected{Error::Invalid_Status_Code};
  }

  return StatusCode{view};
}

Expected<std::string_view> ParseReasonPhrase(Parser& parser) {
  // reason-phrase = *( HTAB / SP / VCHAR / obs-text )
  return parser.Match(limits::kReasonPhrase,
      [](const char c) {
        switch (c) {
          case syntax::kHTAB:
          case syntax::kSP:
            return true;
          default:
            return syntax::IsVchar(c) || syntax::IsObsText(c);
        }
      });
}

}  // namespace hypp::parser
