#include "World.h"
#include <memory>
#include <algorithm>


Field::Field(const unsigned x, const unsigned y)
    : m_coords{x, y}, m_entity(nullptr), m_collisions{} {
}

void Field::move(World_t &world, const Field_t movingType) {
    if (!m_entity) {
        return;
    }

    if (m_entity->getType() != movingType) {
        return;
    }

    if (movingType == Field_t::FOX) {
        auto foxEntity = dynamic_cast<const Fox*>(m_entity.get());
        if (foxEntity->isStarving()) {
            m_entity = nullptr;
            return;
        }
    }

    auto livingEntity = dynamic_cast<LivingEntity*>(m_entity.get());
    Field* moveTarget = nullptr;
    Direction_t direction = Direction_t::SIZE;
    if (!livingEntity->computeMove(world, moveTarget, &direction, m_coords)) {
        return;
    }


    moveTarget->addCollision(std::move(m_entity), direction); // -> m_entity = nullptr
    if (livingEntity->canReproduce()) {
        livingEntity->reproduce();
        m_entity = std::make_unique<Rabbit>();
    }
}

void Field::addCollision(std::unique_ptr<Entity> newEntity, const Direction_t direction) {
    m_collisions[static_cast<unsigned>(direction)] = std::move(newEntity);
}

void Field::resolveCollisions(const Field_t movingType) {
    std::unique_ptr<Entity>* survivor = nullptr;
    if (movingType == Field_t::RABBIT) {
        survivor  = std::max_element(std::begin(m_collisions), std::end(m_collisions),
                [](const std::unique_ptr<Entity>& lhs, const std::unique_ptr<Entity>& rhs) {
                    if (!lhs) return false;
                    else if (!rhs) return true;
                    else return *dynamic_cast<Rabbit*>(lhs.get()) > *dynamic_cast<Rabbit*>(lhs.get());
        });
    } else if (movingType == Field_t::FOX) {
        survivor  = std::max_element(std::begin(m_collisions), std::end(m_collisions),
                [](const std::unique_ptr<Entity>& lhs, const std::unique_ptr<Entity>& rhs) {
                    if (!lhs) return false;
                    else if (!rhs) return true;
                    else return *dynamic_cast<Fox*>(lhs.get()) > *dynamic_cast<Fox*>(lhs.get());
        });
    }
    if (!survivor || !*survivor) {
        return;
    }

    if (movingType == Field_t::FOX && !m_entity && m_entity->getType() == Field_t::RABBIT) {
        auto foxEntity = dynamic_cast<Fox*>(survivor->get());
        foxEntity->eatRabbit();
    }
    m_entity = std::move(*survivor);
}

Field_t Field::getContainedType() const {
    if (!m_entity) {
        return Field_t::EMPTY;
    }

    return m_entity->getType();
}

Entity::Entity(const Field_t type)
    : m_type(type) {
}

Rock::Rock()
    : Entity(Field_t::ROCK) {
}

LivingEntity::LivingEntity(const Field_t type)
    : Entity(type), m_age(0), m_proc{0} {
}

bool LivingEntity::computeMove(const World_t &world, Field *target, Direction_t *direction, const unsigned *coords) const {
    //TODO: implement
    return false;
}

void LivingEntity::incrementAge() {
    ++m_age;
    ++m_proc;
}

Rabbit::Rabbit()
    : LivingEntity(Field_t::RABBIT) {
}

Fox::Fox()
    : LivingEntity(Field_t::FOX), m_hunger(0) {
}

void Fox::incrementAge() {
    LivingEntity::incrementAge();
    ++m_hunger;
}
