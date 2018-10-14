#pragma once

#include <hypp/detail/expected.hpp>
#include <hypp/parser/error.hpp>

namespace hypp::parser {

using Unexpected = hypp::detail::Unexpected<Error>;

template <typename T>
using Expected = hypp::detail::Expected<T, Error, Error::OK>;

}  // namespace hypp::parser
