#include <algorithm>
#include <iostream>

#include "Field.h"
#include "Entity.h"


Field::Field(const unsigned x, const unsigned y)
        : m_coords{x, y}, m_entity(nullptr), m_collisions{} , m_hasMove(false) {
}


void Field::addEntity(const Field_t newType) {
    switch (newType) {
        case Field_t::ROCK:
            m_entity = std::make_shared<Rock>();
            break;
        case Field_t::RABBIT:
            m_entity = std::make_shared<Rabbit>();
            break;
        case Field_t::FOX:
            m_entity = std::make_shared<Fox>();
            break;
        default:
            return;
    }
}

void Field::move(World_t &world, const Field_t movingType, const unsigned generation) {
    m_hasMove = false;

    // only move if type matches
    if (!m_entity || m_entity->getType() != movingType) {
        return;
    }

    // compute move by animal
    auto livingEntity = dynamic_cast<LivingEntity*>(m_entity.get());
    livingEntity->incrementAge();
    Field* moveTarget = nullptr;
    Direction_t direction = Direction_t::SIZE;
    m_hasMove = livingEntity->computeMove(world, m_coords, generation, &moveTarget, &direction);
    if (!m_hasMove) {
        return;
    }

    // execute move and reproduce if possible
    moveTarget->addCollision(m_entity, direction);
}

void Field::addCollision(std::shared_ptr<Entity> &newEntity, const Direction_t direction) {
    m_collisions[static_cast<unsigned>(direction)] = newEntity;
}

void Field::resolveCollisions(const Field_t movingType) {
    if (getContainedType() == Field_t::ROCK) {
        return;
    }

    // reproduce if possible, if tile has a move there wont be any further contestants
    if (m_hasMove) {
        auto livingEntity = dynamic_cast<LivingEntity*>(m_entity.get());
        if (livingEntity->canReproduce()) {
            m_entity = livingEntity->reproduce();
        } else {
            m_entity.reset();
        }
        return;
    }

    // kill starving foxes
    if (movingType == Field_t::FOX && m_entity && m_entity->getType() == Field_t::FOX) {
        auto foxEntity = dynamic_cast<const Fox*>(m_entity.get());
        if (foxEntity->isStarving()) {
            m_entity.reset();
        }
    }

    // compute the surviving entity,
    std::shared_ptr<Entity>* survivor = nullptr;
    if (movingType == Field_t::RABBIT) {
        survivor = std::max_element(
                std::begin(m_collisions),
                std::end(m_collisions),
                [](const std::shared_ptr<Entity>& lhs, const std::shared_ptr<Entity>& rhs) {
                        if (!lhs) return true;
                        else if (!rhs) return false;
                        else return *dynamic_cast<Rabbit*>(lhs.get()) < *dynamic_cast<Rabbit*>(rhs.get());
                });
    } else if (movingType == Field_t::FOX) {
        survivor = std::max_element(
                std::begin(m_collisions),
                std::end(m_collisions),
                [](const std::shared_ptr<Entity>& lhs, const std::shared_ptr<Entity>& rhs) {
                        if (!lhs) return true;
                        else if (!rhs) return false;
                        else return *dynamic_cast<Fox*>(lhs.get()) < *dynamic_cast<Fox*>(rhs.get());
                });
    }

    // check if there is one at all
    if (!survivor || !(*survivor)) {
        return;
    }

    // finalize move by survivor, let fox eat if possible
    if (movingType == Field_t::FOX && getContainedType() == Field_t::RABBIT) {
        dynamic_cast<Fox*>(survivor->get())->eatRabbit();
    }
    m_entity = *survivor;



    // kill all other contestants
    for (auto& pEntity : m_collisions) {
        pEntity.reset();
    }
}

Field_t Field::getContainedType() const {
    if (!m_entity) {
        return Field_t::EMPTY;
    }

    return m_entity->getType();
}

