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

bool
LivingEntity::computeMove(const World_t &world, const unsigned *coords, Direction_t *direction, Field **target) const {
    //TODO: implement
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
