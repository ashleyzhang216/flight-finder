#include "common_data_types.h"
#include "parser.h"
#include <fstream>

const time_t boundary = 1734921599; // hardcode boundary for 2024-12-22 23:59:59 (Unix timestamp), can get end_ts from constraint?
const time_t layover_time = 0;// hardcode layover time

// naive implementation of search
std::string flight_finder::search() {

    itinerary best_itinerary;
    //uint max_legs = 0;

    std::function<void(itinerary&, flight_id)> dfs = 
    [&](itinerary& prev_itinerary, flight_id current_flight_id) {
          
        itinerary current_itinerary = prev_itinerary.add(current_flight_id, flights);
        best_itinerary = itinerary::max(best_itinerary, current_itinerary, origin);
        
        /* Debug
        if (best_itinerary.legs >= max_legs){
            max_legs = best_itinerary.legs;
            best_itinerary.built = true;
            std::cout << best_itinerary.legs << std::endl;
            std::cout << best_itinerary.serialize(flights) << std::endl<< std::endl;
            
        }
        */
        
        // if the arrival time is beyond the boundary, then no more next connecting flight
        if (flights[current_flight_id].arrive_ts > boundary) {
            //current_itinerary.built = true;
            //std::cout << flights[current_flight_id].serialize() << std::endl;
            //std::cout << current_itinerary.serialize(flights) << std::endl<< std::endl;
            return;
        }

        // explore all possible connecting flights
        for (size_t next_flight_id = 0; next_flight_id < flights.size(); ++next_flight_id) {
            const flight& current_flight = flights[current_flight_id];
            const flight& next_flight = flights[next_flight_id];

            // check if the next flight can connect to the current one
            if (current_flight.to == next_flight.from && current_flight.arrive_ts + layover_time <= next_flight.depart_ts) {
                dfs(current_itinerary, next_flight_id);
            }
        }

    };

    if (origin.has_value()) {
        itinerary start_itinerary(origin.value());
        // Find all flights starting from the origin
        for(size_t i = 0; i < flights.size(); ++i){
            if (flights[flight_id{i}].from == origin) {
                itinerary current_itinerary(origin.value());
                dfs(current_itinerary, flight_id{i});
            }
        }

    } else {
        // If no origin is provided, iterate over all airports
        for (const auto& [airport_name, airport_node] : nodes) {
            itinerary start_itinerary(airport_name);
            // Find all flights starting from the origin
            for(size_t i = 0; i < flights.size(); ++i){
                if (flights[flight_id{i}].from == origin) {
                    itinerary current_itinerary = itinerary();
                    dfs(current_itinerary, flight_id{i});
                }
            }
        }
    }

    //return "Optimal Itinerary:\n" + best_itinerary.serialize(flights);
    return best_itinerary.serialize(flights);
}
 
int main(int argc, char **argv)
{
    // TODO: check correctness, generate test suite and golden_ref

    flight_constraints constrs = cli("naive", argc, argv);
    std::cout << "running naive" << std::endl;

    // Example usage of parser
    std::string directory = "flight_arrival_results";
    std::vector<flight> flights = parse_flights_from_directory(directory, constrs);

    /* For correctness checking
    std::ofstream outFile("flights.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return 1; // Exit with an error code
    }
    for (const flight& f : flights)
        outFile << f.serialize();

    outFile.close();
    */

 
    // Initiate flight_finder
    flight_finder finder(std::move(flights), constrs.origin);

    std::cout <<finder.search() << std::endl;

    return 0;
}