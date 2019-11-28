#pragma once
#include <memory>
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

class Field;
class Entity;

using World_t = std::vector<std::vector<Field>>;


class Field {
public:
    Field(const unsigned x, const unsigned y);

    void move(World_t &world, const Field_t movingType);
    void addCollision(std::unique_ptr<Entity> newEntity, const Direction_t direction);
    void resolveCollisions(const Field_t movingType);

    Field_t getContainedType() const;
private:
    const unsigned m_coords[2];
    std::unique_ptr<Entity> m_entity;
    std::unique_ptr<Entity> m_collisions[static_cast<unsigned>(Direction_t::SIZE)];
};

class Entity {
public:
    Field_t getType() const { return m_type; };
    virtual ~Entity() = default;

protected:
    explicit Entity(const Field_t type);

private:
    const Field_t m_type;
};

class Rock : public Entity {
public:
    Rock();
};

class LivingEntity : public Entity {
public:
    ~LivingEntity() override = default;

    bool computeMove(const World_t &world, Field *target, Direction_t *direction, const unsigned *coords) const;
    bool canReproduce() const { return m_proc > 0; }; // TODO: use appropriate value
    void reproduce() { m_proc = 0; };

    virtual void incrementAge();

protected:
    explicit LivingEntity(const Field_t type);

private:
    unsigned m_age;
    unsigned m_proc;
};


class Rabbit : public LivingEntity {
public:
    Rabbit();
};


class Fox : public LivingEntity {
public:
    Fox();

    //TODO: use appropriate value
    bool isStarving() const { return m_hunger > 0; };
    void eatRabbit() { m_hunger = 0; }

    void incrementAge() override;

private:
    unsigned m_hunger;
};

bool operator> (const Rabbit& lhs, const Rabbit& rhs) { return false; };
bool operator> (const Fox& lhs, const Fox& rhs) { return false; };



