#pragma once

namespace statick::model {

enum class TokenType {
    identifier,
    number,
    operator_token,
    left_brace,
    right_brace,
    left_paren,
    right_paren,
    semicolon,
    keyword_for,
    keyword_if,
    keyword_else,
    keyword_switch,
    keyword_do,
    keyword_while,
    keyword_return
};

} // namespace statick::model
