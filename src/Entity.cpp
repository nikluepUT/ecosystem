#include <iostream>
#include "Entity.h"

Entity::Entity(const Field_t type)
        : m_type(type) {
}

Rock::Rock()
        : Entity(Field_t::ROCK) {
}

LivingEntity::LivingEntity(const Field_t type)
        : Entity(type), m_age(0), m_proc{0} {
}


std::vector<std::pair<Field*, Direction_t>> check_adjacent_cells_for_condition(const World_t &world, const unsigned *coords, Field_t condition){
    unsigned X = coords[0];
    unsigned Y = coords[1];

    std::vector<std::pair<Field*, Direction_t>> selected_cells;

    // check cell north
    if (X > 0 && world[X-1][Y].getContainedType() == condition){
        std::cout << "North free" << std::endl;
        //selected_cells.insert(std::pair<Field*, Direction_t>((Field *const) &world[X-1][Y], Direction_t::NORTH));
        selected_cells.push_back(std::pair<Field*, Direction_t>((Field *const) &world[X-1][Y], Direction_t::NORTH));
    }
    // check cell east
    if (Y < world.size()-1 && world[X][Y+1].getContainedType() == condition){
        std::cout << "East free" << std::endl;
        selected_cells.push_back(std::pair<Field*, Direction_t>((Field *const) &world[X][Y+1], Direction_t::EAST));
        //free_cells_vector.push_back((Field *const) &world[X][Y+1]);
    }
    // check cell south
    if (X < world.at(0).size()-1 && world[X+1][Y].getContainedType() == condition){
        std::cout << "South free" << std::endl;
        selected_cells.push_back(std::pair<Field*, Direction_t>((Field *const) &world[X+1][Y], Direction_t::SOUTH));
        //free_cells_vector.push_back((Field *const) &world[X+1][Y]);
    }
    // check cell west
    if (Y > 0 && world[X][Y-1].getContainedType() == condition){
        std::cout << "West free" << std::endl;
        selected_cells.push_back(std::pair<Field*, Direction_t>((Field *const) &world[X][Y-1], Direction_t::WEST));
        //free_cells_vector.push_back((Field *const) &world[X][Y-1]);
    }

    return selected_cells;
}

bool
LivingEntity::computeMove(const World_t &world, const unsigned *coords, Direction_t *direction, Field **target, const unsigned generation) const {
    const auto type = getType();


    if (type == Field_t::RABBIT){
        std::cout << "Move rabbit" << std::endl;

        std::vector<std::pair<Field*, Direction_t>> free_cells = check_adjacent_cells_for_condition(world, coords, Field_t::EMPTY);
        if (free_cells.size() > 0){
            unsigned selected_cell = 0;
            if (free_cells.size() > 1){
                selected_cell = (generation + coords[0] + coords[1]) % free_cells.size();
            }

            std::cout << "selected cell: " << selected_cell << std::endl;

            *target = free_cells.at(selected_cell).first;
            *direction = free_cells.at(selected_cell).second;
            return true;
        }
        return false;
    }
    if (type == Field_t::FOX){
        std::cout << "Move fox" << std::endl;
        std::vector<std::pair<Field*, Direction_t>> adjacent_rabbits = check_adjacent_cells_for_condition(world, coords, Field_t::RABBIT);
        if (adjacent_rabbits.size() > 0){
            unsigned selected_cell = 0;
            if (adjacent_rabbits.size() > 1){
                selected_cell = (generation + coords[0] + coords[1]) % adjacent_rabbits.size();
            }

            std::cout << "selected rabbit: " << selected_cell << std::endl;

            *target = adjacent_rabbits.at(selected_cell).first;
            *direction = adjacent_rabbits.at(selected_cell).second;
            return true;
        }
        else {
            std::cout << "No adjacent rabbit" << std::endl;
            std::vector<std::pair<Field*, Direction_t>> free_cells = check_adjacent_cells_for_condition(world, coords, Field_t::EMPTY);
            if (free_cells.size() > 0){
                unsigned selected_cell = 0;
                if (free_cells.size() > 1){
                    selected_cell = (generation + coords[0] + coords[1]) % free_cells.size();
                }

                std::cout << "selected cell: " << selected_cell << std::endl;

                *target = free_cells.at(selected_cell).first;
                *direction = free_cells.at(selected_cell).second;
                return true;
            }
            return false;
        }
    }
    return false;
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


unsigned Fox::GEN_PROC = 0;
unsigned Fox::GEN_FOOD = 0;

Fox::Fox()
        : LivingEntity(Field_t::FOX), m_hunger(0) {
}

void Fox::incrementAge() {
    LivingEntity::incrementAge();
    ++m_hunger;
}
