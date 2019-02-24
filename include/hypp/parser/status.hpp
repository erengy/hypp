#pragma once

#include <hypp/detail/limits.hpp>
#include <hypp/detail/parser.hpp>
#include <hypp/detail/syntax.hpp>
#include <hypp/detail/util.hpp>
#include <hypp/error.hpp>
#include <hypp/status.hpp>

namespace hypp {

Expected<status::code_t> ParseStatusCode(Parser& parser) {
  // status-code = 3DIGIT
  const auto view = parser.match(detail::limits::kStatusCode, detail::is_digit);

  if (view.size() != detail::limits::kStatusCode) {
    return Unexpected{Error::Invalid_Status_Code};
  }

  return detail::from_chars<status::code_t>(view);
}

Expected<std::string_view> ParseReasonPhrase(Parser& parser) {
  // reason-phrase = *( HTAB / SP / VCHAR / obs-text )
  return parser.match(detail::limits::kReasonPhrase,
      [](const char c) {
        switch (c) {
          case detail::syntax::kHTAB:
          case detail::syntax::kSP:
            return true;
          default:
            return detail::is_vchar(c) || detail::is_obs_text(c);
        }
      });
}

}  // namespace hypp
