#pragma once
#include <vector>
#include <memory>

#include "utils.h"


class Field;
class Entity;


using World_t = std::vector<std::vector<Field>>;

class Field {
public:
    Field(const unsigned x, const unsigned y);

    void addEntity(const Field_t newType);
    void move(World_t &world, const Field_t movingType, const unsigned generation);
    void addCollision(std::unique_ptr<Entity> newEntity, const Direction_t direction);
    void resolveCollisions(const Field_t movingType);

    Field_t getContainedType() const;
private:
    const unsigned m_coords[2];
    std::unique_ptr<Entity> m_entity;
    std::unique_ptr<Entity> m_collisions[static_cast<unsigned>(Direction_t::SIZE)];
};


