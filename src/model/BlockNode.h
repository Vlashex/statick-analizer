#pragma once

#include <vector>

#include "StatementNode.h"

namespace statick::model {

struct BlockNode {
    std::vector<StatementNode> statements {};
};

} // namespace statick::model
