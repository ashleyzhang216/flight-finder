#include "common_data_types.h"
#include "parser.h"

int main(int argc, char **argv)
{
    flight_constraints constrs = cli("naive", argc, argv);

    // Example usage of parser
    std::string directory = "flight_arrival_results";
    std::vector<flight> flights = parse_flights_from_directory(directory, constrs);

    // Initiate flight_finder
    flight_finder finder(std::move(flights), constrs.origin);

    std::cout << "naive" << std::endl;

    return 0;
}