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
/*
void tmp_move_rabbits(Field_t* const* world, int generation);
void move_rabbit(Field_t *const *world, int generation, int X, int Y);
void move_fox(Field_t *const *world, int generation, int X, int Y);
std::vector<coordinate>
check_adjacent_cells_for_condition(const Field_t *const *world, Field_t condition, int X, int Y);
*/

int main() {

    unsigned N_GEN, R, C, N;
    std::cin >> Rabbit::GEN_PROC;
    std::cin >> Fox::GEN_PROC >> Fox::GEN_FOOD;
    std::cin >> N_GEN >> R >> C >> N;

    World_t world;
    world.reserve(R);
    for (auto x = 0u; x < R; ++x) {
        world.emplace_back();
        world[x].reserve(C);
        for (auto y = 0u; y < C; ++y) {
            world[x].emplace_back(x, y);
        }
    }


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

    for (auto generation = 0u; generation < N_GEN; generation++){
        std::cout << "\nGen " << generation << std::endl;
        prettyPrintWorld(world);
    }

    std::cout << std::endl << "Final: " << std::endl;
    prettyPrintWorld(world);

    return EXIT_SUCCESS;
}

void prettyPrintWorld(const World_t& world)  {
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

/*
void tmp_move_rabbits(Field_t* const* world, int generation) {
    std::vector<coordinate> rabbits;
    std::vector<coordinate> foxes;

    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            switch (world[i][j]) {
                case Field_t::RABBIT:
                    rabbits.push_back(coordinate{i,j});
                    break;
                case Field_t::FOX:
                    foxes.push_back(coordinate{i,j});
                    break;
                default:
                    break;
            }
        }
    }

    std::cout << "Iterating through rabbits:" << std::endl;
    for (int i = 0; i < rabbits.size(); ++i){
        move_rabbit(world, generation, rabbits[i].X, rabbits[i].Y);
    }

    std::cout << "\nIterating through foxes:" << std::endl;
    for (int i = 0; i < foxes.size(); ++i){
        move_fox(world, generation, foxes[i].X, foxes[i].Y);
    }
}

void move_rabbit(Field_t *const *world, int generation, int X, int Y){
    std::cout << "Calculate move for rabbit (" << X << "," << Y << ")" << std::endl;

    std::vector<coordinate> free_cells = check_adjacent_cells_for_condition(world, EMPTY, X, Y);

    if (free_cells.size() > 0){
        int seleted_cell = 0;
        if (free_cells.size() > 1){
            seleted_cell = (generation + X + Y) % free_cells.size();
        }
        std::cout << free_cells.size() << " free cells available, pick: (" << free_cells[seleted_cell].X << "," << free_cells[seleted_cell].Y << ")" << std::endl;
        world[free_cells[seleted_cell].X][free_cells[seleted_cell].Y] = RABBIT;

        //TODO: decide if procreate or not
        if (true) {
            world[X][Y] = EMPTY;
        }
    }
}

void move_fox(Field_t *const *world, int generation, int X, int Y){
    std::cout << "Calculate move for fox (" << X << "," << Y << ")" << std::endl;

    std::vector<coordinate> adjacent_rabbits = check_adjacent_cells_for_condition(world, RABBIT, X, Y);

    if (adjacent_rabbits.size() > 0){
        int seleted_cell = 0;
        if (adjacent_rabbits.size() > 1){
            seleted_cell = (generation + X + Y) % adjacent_rabbits.size();
        }
        std::cout << adjacent_rabbits.size() << " adjacent rabbits available, eat: (" << adjacent_rabbits[seleted_cell].X << "," << adjacent_rabbits[seleted_cell].Y << ")" << std::endl;
        world[adjacent_rabbits[seleted_cell].X][adjacent_rabbits[seleted_cell].Y] = FOX;
        //TODO: reset last meal

        //TODO: decide if procreate or not
        if (true) {
            world[X][Y] = EMPTY;
        }
    }
    else {
        std::vector<coordinate> free_cells = check_adjacent_cells_for_condition(world, EMPTY, X, Y);
        if (free_cells.size() > 0){
            int seleted_cell = 0;
            if (free_cells.size() > 1){
                seleted_cell = (generation + X + Y) % free_cells.size();
            }
            std::cout << free_cells.size() << " free cells available, pick: (" << free_cells[seleted_cell].X << "," << free_cells[seleted_cell].Y << ")" << std::endl;
            world[free_cells[seleted_cell].X][free_cells[seleted_cell].Y] = FOX;

            //TODO: decide if procreate or not
            if (true) {
                world[X][Y] = EMPTY;
            }
        }
    }
}

std::vector<coordinate> check_adjacent_cells_for_condition(const Field_t *const *world, Field_t condition, int X, int Y) {
    std::vector<coordinate> selected_cells;
    selected_cells.reserve(4);

    // check cell north
    if (X > 0 && world[X-1][Y] == condition){
        selected_cells.push_back({X - 1, Y});
    }
    // check cell east
    if (Y < R-1 && world[X][Y+1] == condition){
        selected_cells.push_back(coordinate{X, Y + 1});
    }
    // check cell south
    if (X < C-1 && world[X+1][Y] == condition){
        selected_cells.push_back(coordinate{X + 1, Y});
    }
    // check cell west
    if (Y > 0 && world[X][Y-1] == condition){
        selected_cells.push_back(coordinate{X, Y - 1});
    }

    return selected_cells;
}
 */