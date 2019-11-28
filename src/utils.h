#pragma once
#include <vector>


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