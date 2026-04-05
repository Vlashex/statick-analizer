#pragma once

#include <memory>
#include <string>
#include <vector>

#include "BlockNode.h"

namespace statick::model {

struct FunctionNode {
    std::string name {};
    std::unique_ptr<BlockNode> body {};
    std::vector<FunctionNode> nestedFunctions {};
};

} // namespace statick::model
