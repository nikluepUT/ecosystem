#include <iostream>
#include "Entity.h"

#include <vector>

Entity::Entity(const Field_t type)
        : m_type(type) {
}

Rock::Rock()
        : Entity(Field_t::ROCK) {
}

LivingEntity::LivingEntity(const Field_t type)
        : Entity(type), m_age(0), m_proc{0} {
}


std::vector<std::pair<Field*, Direction_t>> checkAdjacentCellsForCondition(World_t &world, const unsigned *coords, const Field_t condition){
    const unsigned x = coords[0], y = coords[1];

    std::vector<std::pair<Field*, Direction_t>> selected_cells;

    // check cell north
    if (x > 0 && world[x - 1][y].getContainedType() == condition){
        selected_cells.emplace_back(&world[x - 1][y], Direction_t::NORTH);
    }
    // check cell east
    if (y < world.size() - 1 && world[x][y + 1].getContainedType() == condition){
        selected_cells.emplace_back(&world[x][y + 1], Direction_t::EAST);
    }
    // check cell south
    if (x < world.at(0).size() - 1 && world[x + 1][y].getContainedType() == condition){
        selected_cells.emplace_back(&world[x + 1][y], Direction_t::SOUTH);
    }
    // check cell west
    if (y > 0 && world[x][y - 1].getContainedType() == condition){
        selected_cells.emplace_back(&world[x][y - 1], Direction_t::WEST);
    }

    return selected_cells;
}

bool selectTarget(const unsigned *coords, const unsigned generation,
        const std::vector<std::pair<Field*, Direction_t>>& freeCells, Direction_t *direction, Field **target) {

    if (!freeCells.empty()) {

        unsigned selected_cell = (generation + coords[0] + coords[1]) % freeCells.size();
        *target = freeCells.at(selected_cell).first;
        *direction = freeCells.at(selected_cell).second;
        return true;
    }
    return false;
}


bool
LivingEntity::computeMove(World_t &world, const unsigned *coords, const unsigned generation, Field **target,
                          Direction_t *direction) const {

    const auto freeCells = checkAdjacentCellsForCondition(world, coords, Field_t::EMPTY);
    return selectTarget(coords, generation, freeCells, direction, target);
}


void LivingEntity::incrementAge() {
    ++m_age;
    ++m_proc;
}

bool LivingEntity::canReproduce() const {
    const auto type = getType();
    switch (type) {
        case Field_t::RABBIT:
           return m_proc >= Rabbit::GEN_PROC;
        case Field_t::FOX:
            return m_proc >= Fox::GEN_PROC;
        default:
            return false;
    }
}

unsigned Rabbit::GEN_PROC = 0;

Rabbit::Rabbit()
        : LivingEntity(Field_t::RABBIT) {
}

std::shared_ptr<LivingEntity> Rabbit::reproduce() {
    m_proc = 0;
    return std::move(std::make_shared<Rabbit>());
}


unsigned Fox::GEN_PROC = 0;
unsigned Fox::GEN_FOOD = 0;

Fox::Fox()
        : LivingEntity(Field_t::FOX), m_hunger(0) {
}

void Fox::incrementAge() {
    LivingEntity::incrementAge();
    ++m_hunger;
}

bool Fox::computeMove(World_t &world, const unsigned *coords, const unsigned generation, Field **target,
                      Direction_t *direction) const {
    const auto adjacentRabbits = checkAdjacentCellsForCondition(world, coords, Field_t::RABBIT);
    if (selectTarget(coords, generation, adjacentRabbits, direction, target)) {
        return true;
    }
    return LivingEntity::computeMove(world, coords, generation, target, direction);
}

std::shared_ptr<LivingEntity> Fox::reproduce() {
    m_proc = 0;
    return std::move(std::make_shared<Fox>());
}
