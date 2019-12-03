#pragma once

#include "utils.h"
#include "Field.h"

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

    bool computeMove(const World_t &world, const unsigned *coords, Direction_t *direction, Field **target, const unsigned generation) const;
    bool canReproduce() const;
    void reproduce() { m_proc = 0; };

    virtual void incrementAge();

    unsigned getAge() const { return m_age; };

protected:
    explicit LivingEntity(const Field_t type);

private:
    unsigned m_age;
    unsigned m_proc;
};


class Rabbit : public LivingEntity {
public:
    Rabbit();

    static unsigned GEN_PROC;
};


class Fox : public LivingEntity {
public:
    Fox();

    bool isStarving() const { return m_hunger >= GEN_FOOD; };
    void eatRabbit() { m_hunger = 0; }

    void incrementAge() override;
    unsigned getHunger() const { return m_hunger; };

    static unsigned GEN_PROC, GEN_FOOD;

private:
    unsigned m_hunger;
};

inline bool operator< (const Rabbit& lhs, const Rabbit& rhs) { return lhs.getAge() < rhs.getAge(); };
inline bool operator< (const Fox& lhs, const Fox& rhs) {
    if (lhs.getAge() == rhs.getAge()) return lhs.getHunger() > rhs.getHunger();
    else return lhs.getAge() < rhs.getAge();
};

