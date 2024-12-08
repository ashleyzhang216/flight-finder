#include "common_data_types.h"

int main(int argc, char** argv) {
    flight_constraints constrs = cli("serial", argc, argv);
    (void)constrs;

    std::cout << "serial" << std::endl;
    
    return 0;
}