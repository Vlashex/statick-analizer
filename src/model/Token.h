#pragma once

#include <cstddef>
#include <string>

#include "TokenType.h"

namespace statick::model {

struct Token {
    TokenType type {TokenType::identifier};
    std::string lexeme {};
    std::size_t index {0};
};

} // namespace statick::model
