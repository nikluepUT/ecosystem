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

class Rock final: public Entity {
public:
    Rock();
};

class LivingEntity : public Entity {
public:
    ~LivingEntity() override = default;

    virtual bool computeMove(World_t &world, const unsigned *coords, const unsigned generation, Field **target,
                     Direction_t *direction) const;
    virtual bool canReproduce() const = 0;
    virtual std::shared_ptr<LivingEntity> reproduce() = 0;

    virtual void incrementAge();


    unsigned getProc() const { return m_proc; };

protected:
    explicit LivingEntity(const Field_t type);
    unsigned m_proc;

private:
    unsigned m_age;
};


class Rabbit final: public LivingEntity {
public:
    Rabbit();

    std::shared_ptr<LivingEntity> reproduce() override;

    bool canReproduce() const override;

    static unsigned GEN_PROC;
};


class Fox final : public LivingEntity {
public:
    Fox();

    bool isStarving() const { return m_hunger >= GEN_FOOD; };
    void eatRabbit() { m_hunger = 0; }

    void incrementAge() override;
    unsigned getHunger() const { return m_hunger; }

    bool computeMove(World_t &world, const unsigned *coords, const unsigned generation, Field **target,
                     Direction_t *direction) const override;

    std::shared_ptr<LivingEntity> reproduce() override;

    bool canReproduce() const override;;

    static unsigned GEN_PROC, GEN_FOOD;

private:
    unsigned m_hunger;
};

inline bool operator< (const Rabbit& lhs, const Rabbit& rhs) { return lhs.getProc() < rhs.getProc(); };
inline bool operator< (const Fox& lhs, const Fox& rhs) {
    if (lhs.getProc() == rhs.getProc()) return lhs.getHunger() > rhs.getHunger();
    else return lhs.getProc() < rhs.getProc();
};

