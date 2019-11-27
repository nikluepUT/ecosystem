#include <iostream>
#include <string>
#include <vector>

#include "Field.h"

struct coordinate {
    int X;
    int Y;
};

void printDetailedWorld(const Field_t* const* world);
/*
void tmp_move_rabbits(Field_t* const* world, int generation);
void move_rabbit(Field_t *const *world, int generation, int X, int Y);
void move_fox(Field_t *const *world, int generation, int X, int Y);
std::vector<coordinate>
check_adjacent_cells_for_condition(const Field_t *const *world, Field_t condition, int X, int Y);
*/
unsigned readVal() {
    unsigned val;
    std::cin >> val;
    return val;
}

// globals that wont change throughout the runtime
const auto GEN_PROC_RABBITS = readVal(), GEN_PROC_FOXES = readVal(), GEN_FOOD_FOXES = readVal(),
        N_GEN = readVal(), R = readVal(), C = readVal(), N = readVal();


int main() {

    auto world = new Field_t*[R];
    for (auto i = 0u; i < R; ++i) {
        world[i] = new Field_t[C];
        std::fill(world[i], world[i] + C, Field_t::EMPTY);
    }

    for (auto i = 0u; i < N; ++i) {
        std::string type;
        unsigned x, y;

        std::cin >> type >> x >> y;
        if (type == "ROCK") {
            world[x][y] = Field_t::ROCK;
        } else if (type == "RABBIT") {
            world[x][y] = Field_t::RABBIT;
        } else if (type == "FOX") {
            world[x][y] = Field_t::FOX;
        }
    }

    for (int generation = 0; generation < N_GEN; generation++){
        std::cout << "\nGen " << generation << std::endl;
        printDetailedWorld(world);

        //tmp_move_rabbits(world, generation);
    }
    return EXIT_SUCCESS;
}

void printDetailedWorld(const Field_t *const *world)  {
    auto printLine = []() {
        std::cout << '+';
        for (auto i = 0u; i < 2*C + 1; ++i) {
            std::cout << '-';
        }
        std::cout << '+' << '\n';
    };

    printLine();

    for (auto i = 0u; i < R; ++i) {
        std::cout << '|';

        for (auto j = 0u; j < C; ++j) {
            std::cout << ' ';
            switch (world[i][j]) {
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