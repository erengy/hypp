#pragma once

#include <hypp/detail/limits.hpp>
#include <hypp/detail/parser.hpp>
#include <hypp/detail/syntax.hpp>
#include <hypp/detail/util.hpp>
#include <hypp/parser/error.hpp>
#include <hypp/status.hpp>

namespace hypp::parser {

Expected<status::code_t> ParseStatusCode(Parser& parser) {
  // status-code = 3DIGIT
  const auto view = parser.Match(hypp::detail::limits::kStatusCode,
                                 hypp::detail::IsDigit);

  if (view.size() != hypp::detail::limits::kStatusCode) {
    return Unexpected{Error::Invalid_Status_Code};
  }

  return hypp::detail::from_chars<status::code_t>(view);
}

Expected<std::string_view> ParseReasonPhrase(Parser& parser) {
  // reason-phrase = *( HTAB / SP / VCHAR / obs-text )
  return parser.Match(hypp::detail::limits::kReasonPhrase,
      [](const char c) {
        switch (c) {
          case hypp::detail::syntax::kHTAB:
          case hypp::detail::syntax::kSP:
            return true;
          default:
            return hypp::detail::IsVchar(c) || hypp::detail::IsObsText(c);
        }
      });
}

}  // namespace hypp::parser
