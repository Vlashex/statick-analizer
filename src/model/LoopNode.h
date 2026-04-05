#pragma once

#include <memory>

#include "BlockNode.h"

namespace statick::model {

struct LoopNode {
    std::unique_ptr<BlockNode> body {};
};

} // namespace statick::model
