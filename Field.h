#pragma once


#include <vector>

enum Direction_t : unsigned {
    NORTH = 0u,
    EAST,
    SOUTH,
    WEST,
    NONE
};

enum Field_t : unsigned {
    EMPTY = 0u,
    ROCK,
    RABBIT,
    FOX
};



class Field {
public:
    Field(const unsigned x, const unsigned y, const Field_t type);
    virtual ~Field() = 0;

    void addCollision(const Direction_t dir, Field* field);
    void resolveCollisions();

    virtual void move() {};

    const unsigned* getCoords() const { return m_coords; };

    Field_t getType() const { return m_type; };

    static std::vector<std::vector<Field*>> world;
protected:
    Field* m_collisions[5];
private:
    unsigned m_coords[2];
    const Field_t m_type;
};

class Empty : public Field {
public:
    Empty(const unsigned x, const unsigned y);
};


class Rock : public Field {
public:
    Rock(const unsigned x, const unsigned y);
private:
};

class Rabbit : public Field {
public:
    Rabbit(const unsigned x, const unsigned y);
    Rabbit(const unsigned x, const unsigned y, const Rabbit& oldRabbit);

    unsigned getAge() const { return m_age; };

    static bool compare(const Rabbit& lhs, const Rabbit& rhs);
private:
    unsigned m_age;
};


class Fox : public Field {
public:


    Fox(const unsigned x, const unsigned y);
    Fox(const unsigned x, const unsigned y, const Fox& oldFox);

    unsigned getAge() const { return m_age; };
    unsigned getHunger() const { return m_hunger; };

    void eat();


    static bool compare(const Fox& lhs, const Fox& rhs);
private:
    unsigned m_age;
    unsigned m_hunger;
};




