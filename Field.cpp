//
// Created by nikluep on 27/11/2019.
//

#include "Field.h"

#include <vector>

Field::Field(const unsigned x, const unsigned y, const Field_t type)
    : m_coords{x, y}, m_collisions{}, m_type(type) {

}

void Field::addCollision(const Direction_t dir, Field *field) {
    m_collisions[dir] = field;
}

void Field::resolveCollisions() {

}

Empty::Empty(const unsigned x, const unsigned y)
    : Field(x, y, Field_t::EMPTY) {

}

Rock::Rock(const unsigned x, const unsigned y)
    : Field(x, y, Field_t::ROCK) {

}

Rabbit::Rabbit(const unsigned x, const unsigned y)
    : Field(x, y, Field_t::RABBIT), m_age(0) {

}

Rabbit::Rabbit(const unsigned x, const unsigned y, const Rabbit &oldRabbit)
    : Field(x, y, Field_t::RABBIT), m_age(oldRabbit.m_age) {

}

Fox::Fox(const unsigned x, const unsigned y)
    : Field(x, y, Field_t::FOX), m_age(0), m_hunger(0) {

}

Fox::Fox(const unsigned x, const unsigned y, const Fox &oldFox)
        : Field(x, y, Field_t::FOX), m_age(oldFox.m_age), m_hunger(oldFox.m_hunger){

}
