#pragma once

#include <cstddef>
#include <string>

#include "../model/BlockNode.h"
#include "../model/BranchNode.h"
#include "../model/FunctionNodeList.h"
#include "../model/LoopNode.h"
#include "../model/StatementNode.h"
#include "../model/TokenList.h"

namespace statick::parser {

class SimpleParser {
public:
    [[nodiscard]] model::FunctionNodeList parse(const model::TokenList& tokens);

private:
    [[nodiscard]] const model::Token& peek(std::size_t offset = 0) const;
    bool match(model::TokenType type);
    model::Token consume(model::TokenType type, const std::string& error);

    [[nodiscard]] bool isAtEnd(std::size_t offset = 0) const;
    [[nodiscard]] bool check(model::TokenType type, std::size_t offset = 0) const;
    [[nodiscard]] bool isFunctionStart() const;
    [[nodiscard]] std::size_t findClosingParen(std::size_t openIndex) const;

    void skipParentheses();
    void parseNextBlockItem(model::BlockNode& block, model::FunctionNode& owner);

    [[nodiscard]] model::FunctionNode parseFunction();
    [[nodiscard]] std::unique_ptr<model::BlockNode> parseBlock(model::FunctionNode& owner);
    [[nodiscard]] std::unique_ptr<model::BlockNode> parseStatementOrBlock(model::FunctionNode& owner);
    [[nodiscard]] model::StatementNode parseSimpleStatement();
    [[nodiscard]] std::unique_ptr<model::LoopNode> parseLoop(model::FunctionNode& owner);
    [[nodiscard]] std::unique_ptr<model::BranchNode> parseIf(model::FunctionNode& owner);
    [[nodiscard]] std::unique_ptr<model::BranchNode> parseSwitch(model::FunctionNode& owner);

    const model::TokenList* tokens_ {nullptr};
    std::size_t current_ {0};
};

} // namespace statick::parser
