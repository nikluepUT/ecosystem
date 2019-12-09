#include <algorithm>

#include "Field.h"
#include "Entity.h"


Field::Field(const unsigned x, const unsigned y)
        : m_coords{x, y}, m_entity(nullptr), m_collisions{} {
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
    // only move if type matches
    if (!m_entity || m_entity->getType() != movingType) {
        return;
    }


    // kill starving foxes
    if (movingType == Field_t::FOX) {
        auto foxEntity = dynamic_cast<const Fox*>(m_entity.get());
        if (foxEntity->isStarving()) {
            m_entity = nullptr;
            return;
        }
    }

    // compute move by animal
    auto livingEntity = dynamic_cast<LivingEntity*>(m_entity.get());
    livingEntity->incrementAge();
    Field* moveTarget = nullptr;
    Direction_t direction = Direction_t::SIZE;
    if (!livingEntity->computeMove(world, m_coords, generation, &moveTarget, &direction)) {
        return;
    }

    // execute move and reproduce if possible
    moveTarget->addCollision(std::move(m_entity), direction); // -> m_entity = nullptr
    if (livingEntity->canReproduce()) {
        m_entity = livingEntity->reproduce();
    }
}

void Field::addCollision(std::shared_ptr<Entity> newEntity, const Direction_t direction) {
    m_collisions[static_cast<unsigned>(direction)] = std::move(newEntity);
}

void Field::resolveCollisions(const Field_t movingType) {
    // compute the surviving entity,
    std::shared_ptr<Entity>* survivor = nullptr;
    if (movingType == Field_t::RABBIT) {
        survivor = std::max_element(
                std::begin(m_collisions),
                std::end(m_collisions),
                [](const std::shared_ptr<Entity>& lhs, const std::shared_ptr<Entity>& rhs) {
                        if (!lhs) return true;
                        else if (!rhs) return false;
                        else return *dynamic_cast<Rabbit*>(lhs.get()) < *dynamic_cast<Rabbit*>(lhs.get());
                });
    } else if (movingType == Field_t::FOX) {
        survivor = std::max_element(
                std::begin(m_collisions),
                std::end(m_collisions),
                [](const std::shared_ptr<Entity>& lhs, const std::shared_ptr<Entity>& rhs) {
                        if (!lhs) return true;
                        else if (!rhs) return false;
                        else return *dynamic_cast<Fox*>(lhs.get()) < *dynamic_cast<Fox*>(lhs.get());
                });
    }

    // check if there is one at all
    if (!*survivor) {
        return;
    }

    // finalize move by survivor, let fox eat if possible
    if (movingType == Field_t::FOX && m_entity && m_entity->getType() == Field_t::RABBIT) {
        auto foxEntity = dynamic_cast<Fox*>(survivor->get());
        foxEntity->eatRabbit();
    }
    m_entity = std::move(*survivor);

    // kill all other contestants
    for (auto& pEntity : m_collisions) {
        pEntity = nullptr;
    }
}

Field_t Field::getContainedType() const {
    if (!m_entity) {
        return Field_t::EMPTY;
    }

    return m_entity->getType();
}

