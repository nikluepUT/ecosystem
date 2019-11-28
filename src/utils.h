#pragma once
#include <vector>

class Field;
using World_t = std::vector<std::vector<Field>>;

enum class Direction_t : unsigned {
    NORTH = 0u,
    EAST,
    SOUTH,
    WEST,
    SIZE
};

enum class Field_t : unsigned {
    EMPTY = 0u,
    ROCK,
    RABBIT,
    FOX,
    SIZE
};