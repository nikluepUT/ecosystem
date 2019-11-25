#include <iostream>
#include <string>

enum Object_t : unsigned {
    EMPTY = 0u,
    ROCK,
    RABBIT,
    FOX
};

void printDetailedWorld(const unsigned* const* world);



unsigned readVal() {
    unsigned val;
    std::cin >> val;
    return val;
}

// globals that wont change throughout the runtime
const auto GEN_PROC_RABBITS = readVal(), GEN_PROC_FOXES = readVal(), GEN_FOOD_FOXES = readVal(),
        N_GEN = readVal(), R = readVal(), C = readVal(), N = readVal();


int main() {

    auto world = new unsigned*[R];
    for (auto i = 0u; i < R; ++i) {
        world[i] = new unsigned[C];
        std::fill(world[i], world[i] + C, Object_t::EMPTY);
    }

    for (auto i = 0u; i < N; ++i) {
        std::string type;
        unsigned x, y;

        std::cin >> type >> x >> y;
        if (type == "ROCK") {
            world[x][y] = Object_t::ROCK;
        } else if (type == "RABBIT") {
            world[x][y] = Object_t::RABBIT;
        } else if (type == "FOX") {
            world[x][y] = Object_t::FOX;
        }
    }


    printDetailedWorld(world);
    return EXIT_SUCCESS;
}

void printDetailedWorld(const unsigned *const *world)  {
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
                case Object_t::ROCK:
                    std::cout << '*';
                    break;
                case Object_t::RABBIT:
                    std::cout << 'R';
                    break;
                case Object_t::FOX:
                    std::cout << 'F';
                    break;
                case Object_t::EMPTY:
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