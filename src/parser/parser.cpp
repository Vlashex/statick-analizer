#include "parser.h"

#include <memory>
#include <stdexcept>
#include <utility>

#include "../model/BlockNode.h"
#include "../model/BranchNode.h"
#include "../model/FunctionNode.h"
#include "../model/LoopNode.h"
#include "../model/StatementNode.h"
#include "../model/Token.h"
#include "../model/TokenType.h"

namespace statick::parser {

const model::Token& SimpleParser::peek(std::size_t offset) const {
    if (isAtEnd(offset)) {
        throw std::runtime_error("Unexpected end of token stream.");
    }

    return (*tokens_)[current_ + offset];
}

bool SimpleParser::match(model::TokenType type) {
    if (!check(type)) {
        return false;
    }

    ++current_;
    return true;
}

model::Token SimpleParser::consume(model::TokenType type, const std::string& error) {
    if (!check(type)) {
        throw std::runtime_error(error);
    }

    return (*tokens_)[current_++];
}

bool SimpleParser::isAtEnd(std::size_t offset) const {
    return tokens_ == nullptr || current_ + offset >= tokens_->size();
}

bool SimpleParser::check(model::TokenType type, std::size_t offset) const {
    return !isAtEnd(offset) && peek(offset).type == type;
}

bool SimpleParser::isFunctionStart() const {
    if (!check(model::TokenType::identifier) || !check(model::TokenType::left_paren, 1)) {
        return false;
    }

    const std::size_t closingParen = findClosingParen(current_ + 1);
    return !isAtEnd(closingParen - current_ + 1) && (*tokens_)[closingParen + 1].type == model::TokenType::left_brace;
}

std::size_t SimpleParser::findClosingParen(std::size_t openIndex) const {
    std::size_t depth = 0;

    for (std::size_t i = openIndex; i < tokens_->size(); ++i) {
        if ((*tokens_)[i].type == model::TokenType::left_paren) {
            ++depth;
        } else if ((*tokens_)[i].type == model::TokenType::right_paren) {
            --depth;

            if (depth == 0) {
                return i;
            }
        }
    }

    throw std::runtime_error("Expected ')'.");
}

void SimpleParser::skipParentheses() {
    consume(model::TokenType::left_paren, "Expected '('.");

    std::size_t depth = 1;

    while (depth > 0) {
        if (match(model::TokenType::left_paren)) {
            ++depth;
            continue;
        }

        if (match(model::TokenType::right_paren)) {
            --depth;
            continue;
        }

        if (isAtEnd()) {
            throw std::runtime_error("Expected ')'.");
        }

        ++current_;
    }
}

void SimpleParser::parseNextBlockItem(model::BlockNode& block, model::FunctionNode& owner) {
    if (isFunctionStart()) {
        owner.nestedFunctions.push_back(parseFunction());
        return;
    }

    if (match(model::TokenType::keyword_for)) {
        block.loops.push_back(parseLoop(owner));

        model::StatementNode statement;
        statement.kind = model::StatementKind::loop;
        statement.childIndex = block.loops.size() - 1;
        block.statements.push_back(std::move(statement));
        return;
    }

    if (match(model::TokenType::keyword_if)) {
        block.branches.push_back(parseIf(owner));

        model::StatementNode statement;
        statement.kind = model::StatementKind::branch;
        statement.childIndex = block.branches.size() - 1;
        block.statements.push_back(std::move(statement));
        return;
    }

    if (match(model::TokenType::keyword_switch)) {
        block.branches.push_back(parseSwitch(owner));

        model::StatementNode statement;
        statement.kind = model::StatementKind::branch;
        statement.childIndex = block.branches.size() - 1;
        block.statements.push_back(std::move(statement));
        return;
    }

    if (check(model::TokenType::left_brace)) {
        block.blocks.push_back(parseBlock(owner));

        model::StatementNode statement;
        statement.kind = model::StatementKind::block;
        statement.childIndex = block.blocks.size() - 1;
        block.statements.push_back(std::move(statement));
        return;
    }

    block.statements.push_back(parseSimpleStatement());
}

model::FunctionNode SimpleParser::parseFunction() {
    model::FunctionNode function;
    function.name = consume(model::TokenType::identifier, "Expected function name.").lexeme;

    skipParentheses();
    function.body = parseBlock(function);
    return function;
}

std::unique_ptr<model::BlockNode> SimpleParser::parseBlock(model::FunctionNode& owner) {
    consume(model::TokenType::left_brace, "Expected '{'.");

    auto block = std::make_unique<model::BlockNode>();

    while (!check(model::TokenType::right_brace)) {
        if (isAtEnd()) {
            throw std::runtime_error("Expected '}'.");
        }

        parseNextBlockItem(*block, owner);
    }

    consume(model::TokenType::right_brace, "Expected '}'.");
    return block;
}

std::unique_ptr<model::BlockNode> SimpleParser::parseStatementOrBlock(model::FunctionNode& owner) {
    if (check(model::TokenType::left_brace)) {
        return parseBlock(owner);
    }

    auto block = std::make_unique<model::BlockNode>();
    parseNextBlockItem(*block, owner);
    return block;
}

model::StatementNode SimpleParser::parseSimpleStatement() {
    while (!check(model::TokenType::semicolon)) {
        if (check(model::TokenType::left_brace) || check(model::TokenType::right_brace) || isAtEnd()) {
            throw std::runtime_error("Expected ';'.");
        }

        ++current_;
    }

    consume(model::TokenType::semicolon, "Expected ';'.");

    model::StatementNode statement;
    statement.kind = model::StatementKind::simple;
    return statement;
}

std::unique_ptr<model::LoopNode> SimpleParser::parseLoop(model::FunctionNode& owner) {
    auto loop = std::make_unique<model::LoopNode>();
    skipParentheses();
    loop->body = parseStatementOrBlock(owner);
    return loop;
}

std::unique_ptr<model::BranchNode> SimpleParser::parseIf(model::FunctionNode& owner) {
    auto branch = std::make_unique<model::BranchNode>();
    branch->kind = model::BranchKind::if_branch;

    skipParentheses();
    branch->primaryBranch = parseStatementOrBlock(owner);

    if (match(model::TokenType::keyword_else)) {
        branch->secondaryBranch = parseStatementOrBlock(owner);
    }

    return branch;
}

std::unique_ptr<model::BranchNode> SimpleParser::parseSwitch(model::FunctionNode& owner) {
    auto branch = std::make_unique<model::BranchNode>();
    branch->kind = model::BranchKind::switch_branch;

    skipParentheses();
    branch->primaryBranch = parseStatementOrBlock(owner);
    return branch;
}

model::FunctionNodeList SimpleParser::parse(const model::TokenList& tokens) {
    tokens_ = &tokens;
    current_ = 0;

    model::FunctionNodeList functions;

    while (!isAtEnd()) {
        if (isFunctionStart()) {
            functions.push_back(parseFunction());
            continue;
        }

        ++current_;
    }

    return functions;
}

} // namespace statick::parser
