#include <iostream>
#include <string>
#include <vector>

#include "Field.h"
#include "Entity.h"
#include "utils.h"

struct coordinate {
    int X;
    int Y;
};

void prettyPrintWorld(const World_t& world);
void moveAnimal(World_t& world, const unsigned generation, Field_t animal);
void resolveCollisionForAnimal(World_t& world, Field_t animal);

int main() {

    // init config variables
    unsigned N_GEN, R, C, N;
    std::cin >> Rabbit::GEN_PROC;
    std::cin >> Fox::GEN_PROC >> Fox::GEN_FOOD;
    std::cin >> N_GEN >> R >> C >> N;

    // init world
    World_t world;
    world.reserve(R);
    for (auto x = 0u; x < R; ++x) {
        world.emplace_back();
        world[x].reserve(C);
        for (auto y = 0u; y < C; ++y) {
            world[x].emplace_back(x, y);
        }
    }


    // init entities
    for (auto i = 0u; i < N; ++i) {
        std::string sType;
        Field_t type = Field_t::EMPTY;
        unsigned x, y;

        std::cin >> sType >> x >> y;
        if (sType == "ROCK") {
            type = Field_t::ROCK;
        } else if (sType == "RABBIT") {
            type = Field_t::RABBIT;
        } else if (sType == "FOX") {
            type = Field_t::FOX;
        }

        if (type != Field_t::EMPTY) {
            world[x][y].addEntity(type);
        }
    }

    // simulate ecosystem
    for (unsigned generation = 0u; generation < N_GEN; generation++){
        std::cout << "\nGen " << generation << std::endl;
        prettyPrintWorld(world);

        moveAnimal(world, generation, Field_t::FOX);
        resolveCollisionForAnimal(world, Field_t::FOX);
        moveAnimal(world, generation, Field_t::RABBIT);
        resolveCollisionForAnimal(world, Field_t::RABBIT);
    }

    // DONE
    std::cout << std::endl << "Final: " << std::endl;
    prettyPrintWorld(world);

    return EXIT_SUCCESS;
}

void prettyPrintWorld(const World_t& world)  {
    // print a complete line of '-' encased by '+'
    auto printLine = [&]() {
        std::cout << '+';
        for (size_t i = 0; i < 2*world[0].size() + 1; ++i) {
            std::cout << '-';
        }
        std::cout << '+' << '\n';
    };



    printLine();

    for (size_t i = 0; i < world.size(); ++i) {
        std::cout << '|';

        for (size_t j = 0u; j < world[0].size(); ++j) {
            std::cout << ' ';
            switch (world[i][j].getContainedType()) {
                case Field_t::ROCK:
                    std::cout << '*';
                    break;
                case Field_t::RABBIT:
                    std::cout << 'R';
                    break;
                case Field_t::FOX:
                    std::cout << 'F';
                    break;
                case Field_t::EMPTY:
                default:
                    std::cout << ' ';
                    break;
            }
        }

        std::cout << " |\n";
    }

    printLine();
    std::cout << std::endl;
}


void moveAnimal(World_t& world, const unsigned generation, Field_t animal){
    for (size_t i = 0; i < world.size(); ++i) {
        for (size_t j = 0u; j < world[0].size(); ++j) {
            if (world[i][j].getContainedType() == animal){
                world[i][j].move(world, world[i][j].getContainedType(), generation);
            }
        }
    }
}

void resolveCollisionForAnimal(World_t& world, Field_t animal){
    for (size_t i = 0; i < world.size(); ++i) {
        for (size_t j = 0u; j < world[0].size(); ++j) {
            if (world[i][j].getContainedType() == animal){
                std::cout << "animal found" << std::endl;
                world[i][j].resolveCollisions(world[i][j].getContainedType());
            }
        }
    }
}

