#include <iostream>
#include <string>
#include <vector>
#include <omp.h>

#include "Field.h"
#include "Entity.h"
#include "utils.h"

void prettyPrintWorld(const World_t& world);
void moveAnimal(World_t& world, const unsigned generation, Field_t animal);
void resolveCollisionForAnimal(World_t& world, Field_t animal);

#define NUM_THREADS 1

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

#ifdef _OPENMP
    double start_time = omp_get_wtime();
#endif

    // simulate ecosystem
#pragma omp parallel num_threads(NUM_THREADS) default(none) shared(N_GEN, world, std::cout)
{
#ifdef _OPENMP
#pragma omp master
    {
        const auto numThreads = omp_get_num_threads();
        std::cout << "Running with " << numThreads << " threads!" << std::endl;
    };
#endif

    for (auto generation = 0u; generation < N_GEN; ++generation) {

#pragma omp master
        {
            std::cout << "\nGen " << generation << std::endl;
            prettyPrintWorld(world);
        }
#pragma omp barrier

        moveAnimal(world, generation + 1, Field_t::RABBIT);
        resolveCollisionForAnimal(world, Field_t::RABBIT);
        moveAnimal(world, generation + 1, Field_t::FOX);
        resolveCollisionForAnimal(world, Field_t::FOX);
    }

}
#ifdef _OPENMP
    double time = omp_get_wtime() - start_time;
#endif
    // DONE
    std::cout << std::endl << "Final: " << std::endl;
    prettyPrintWorld(world);
#ifdef _OPENMP
    std::cout << "Time: " << time*1000 << "ms" << std::endl;
#endif
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
#pragma omp for collapse(2)
    for (size_t i = 0; i < world.size(); ++i) {
        for (size_t j = 0u; j < world[0].size(); ++j) {
                world[i][j].move(world, animal, generation);
        }
    }
#pragma omp barrier
}

void resolveCollisionForAnimal(World_t& world, Field_t animal){
#pragma omp for collapse(2)
    for (size_t i = 0; i < world.size(); ++i) {
        for (size_t j = 0u; j < world[0].size(); ++j) {
            world[i][j].resolveCollisions(animal);
        }
    }
#pragma omp barrier
}

