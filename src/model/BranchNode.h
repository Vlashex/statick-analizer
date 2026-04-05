#pragma once

#include <memory>

#include "BlockNode.h"

namespace statick::model {

enum class BranchKind {
    if_branch,
    switch_branch
};

struct BranchNode {
    BranchKind kind {BranchKind::if_branch};
    std::unique_ptr<BlockNode> primaryBranch {};
    std::unique_ptr<BlockNode> secondaryBranch {};
};

} // namespace statick::model
