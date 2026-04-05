#pragma once

#include <memory>

namespace statick::model {

struct LoopNode;
struct BranchNode;
struct BlockNode;

enum class StatementKind {
    simple,
    loop,
    branch,
    block
};

struct StatementNode {
    StatementKind kind {StatementKind::simple};
    std::unique_ptr<LoopNode> loop {};
    std::unique_ptr<BranchNode> branch {};
    std::unique_ptr<BlockNode> block {};

    StatementNode() = default;
    ~StatementNode();
    StatementNode(StatementNode&&) noexcept = default;
    StatementNode& operator=(StatementNode&&) noexcept = default;

    static StatementNode makeSimple();
    static StatementNode makeLoop(std::unique_ptr<LoopNode> loopNode);
    static StatementNode makeBranch(std::unique_ptr<BranchNode> branchNode);
    static StatementNode makeBlock(std::unique_ptr<BlockNode> blockNode);
};

} // namespace statick::model
