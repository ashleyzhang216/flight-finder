#include "common_data_types.h"

// parallel implementation of search
template<>
std::string flight_finder::search<OptLevel::PARALLEL>() {
    // TODO: implement
    return "not implemented";
}

#ifndef REMOVE_MAIN_FUNC
int main(int argc, char** argv) {
    flight_constraints constrs = cli("parallel", argc, argv);
    (void)constrs;

    std::cout << "parallel" << std::endl;
    
    return 0;
}
#endif // REMOVE_MAIN_FUNC