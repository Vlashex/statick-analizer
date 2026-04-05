#pragma once

#include <cstddef>

namespace statick::model {

enum class StatementKind {
    simple,
    loop,
    branch,
    block
};

struct StatementNode {
    StatementKind kind {StatementKind::simple};
    std::size_t childIndex {0};
};

} // namespace statick::model
