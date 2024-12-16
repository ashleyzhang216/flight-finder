#include "common_data_types.h"
#include "parser.h"

// parallel implementation of search
template<>
std::string flight_finder::search<OptLevel::PARALLEL>() {
    // TODO: implement
    return "not implemented";
}

#ifndef REMOVE_MAIN_FUNC
int main(int argc, char** argv) {
    flight_constraints constrs = cli("parallel", argc, argv);

    std::cout << "running parallel" << std::endl;

    std::string directory = "flight_concurr_arr_results";
    std::vector<flight> flights = parse_flights_from_directory(directory, constrs);

    flight_finder ff(std::move(flights), constrs.origin);
    std::cout << ff.search<OptLevel::PARALLEL>() << std::endl;
    
    return 0;
}
#endif // REMOVE_MAIN_FUNC