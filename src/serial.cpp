#include "common_data_types.h"

// serial implementation of search
std::string flight_finder::search() {
    return itinerary().serialize(flights);
}

int main(int argc, char** argv) {
    flight_constraints constrs = cli("serial", argc, argv);

    std::cout << "running serial" << std::endl;

    // TODO: call parser
    std::vector<flight> temp;

    flight_finder ff(std::move(temp), constrs.origin);
    std::cout << ff.search() << std::endl;
    
    return 0;
}
