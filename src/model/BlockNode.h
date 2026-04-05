#pragma once

#include <memory>
#include <vector>

#include "StatementNode.h"

namespace statick::model {

struct LoopNode;
struct BranchNode;

struct BlockNode {
    std::vector<StatementNode> statements {};
    std::vector<std::unique_ptr<LoopNode>> loops {};
    std::vector<std::unique_ptr<BranchNode>> branches {};
    std::vector<std::unique_ptr<BlockNode>> blocks {};
};

} // namespace statick::model
