#include "StatementNode.h"

#include <utility>

#include "BlockNode.h"
#include "BranchNode.h"
#include "LoopNode.h"

namespace statick::model {

StatementNode::~StatementNode() = default;

StatementNode StatementNode::makeSimple() {
    StatementNode statement;
    statement.kind = StatementKind::simple;
    return statement;
}

StatementNode StatementNode::makeLoop(std::unique_ptr<LoopNode> loopNode) {
    StatementNode statement;
    statement.kind = StatementKind::loop;
    statement.loop = std::move(loopNode);
    return statement;
}

StatementNode StatementNode::makeBranch(std::unique_ptr<BranchNode> branchNode) {
    StatementNode statement;
    statement.kind = StatementKind::branch;
    statement.branch = std::move(branchNode);
    return statement;
}

StatementNode StatementNode::makeBlock(std::unique_ptr<BlockNode> blockNode) {
    StatementNode statement;
    statement.kind = StatementKind::block;
    statement.block = std::move(blockNode);
    return statement;
}

} // namespace statick::model
