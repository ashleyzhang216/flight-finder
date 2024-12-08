#include "common_data_types.h"

int main(int argc, char** argv) {
    flight_constraints constrs = cli("parallel", argc, argv);
    (void)constrs;

    std::cout << "parallel" << std::endl;
    
    return 0;
}