#include "common_data_types.h"

int main(int argc, char** argv) {
    flight_constraints constrs = cli("naive", argc, argv);
    (void)constrs;

    std::cout << "naive" << std::endl;
    
    return 0;
}