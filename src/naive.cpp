#include "common_data_types.h"
#include "parser.h"
#include <fstream>

const time_t boundary = 1734911999; // hardcode boundary for 2024-12-22 23:59:59 (Unix timestamp), can get end_ts from constraint?
const time_t layover_time = 0;// hardcode layover time
 
// naive implementation of search
std::string flight_finder::search() {

    //std::ofstream outFile("debug.txt");
    itinerary best_itinerary;

    std::function<void(itinerary&, flight_id, int)> dfs = 
    [&](itinerary& prev_itinerary, flight_id current_flight_id, int depth) {
          
        itinerary current_itinerary = prev_itinerary.add(current_flight_id, flights);
        best_itinerary = itinerary::max(best_itinerary, current_itinerary, origin);
        //outFile << "Depth: "<<depth<<std::endl;
        //outFile << "Current flight: "<<std::endl<<flights[current_flight_id].serialize()<<std::endl;
        //outFile << "Current itinerary: "<<std::endl<<current_itinerary.serialize(flights)<<std::endl;
        best_itinerary.built = true;
        //outFile << "Current best itinerary: "<<std::endl<<best_itinerary.serialize(flights)<<std::endl;
        
        // if the arrival time is beyond the boundary, then no more next connecting flight
        if (flights[current_flight_id].arrive_ts + airport_tz[flights[current_flight_id].to] * 3600 > boundary) {
            //outFile << "End of the day, no more departing flights"<<std::endl;
            return;
        }

        // explore all possible connecting flights
        for (size_t next_flight_id = 0; next_flight_id < flights.size(); ++next_flight_id) {
            const flight& current_flight = flights[current_flight_id];
            const flight& next_flight = flights[next_flight_id];

            // check if the next flight can connect to the current one
            if (current_flight.to == next_flight.from && current_flight.arrive_ts + layover_time <= next_flight.depart_ts) {
                //outFile << "Explore next flight: "<<flights[next_flight_id].serialize()<<std::endl;
                dfs(current_itinerary, next_flight_id, depth + 1);
                //outFile << "Back to last layer"<<std::endl<<std::endl;
            }
        }

    };

    if (origin.has_value()) {
        itinerary start_itinerary(origin.value());
        // Find all flights starting from the origin
        for(size_t i = 0; i < flights.size(); ++i){
            if (flights[flight_id{i}].from == origin) {
                itinerary current_itinerary(origin.value());
                dfs(current_itinerary, flight_id{i}, 0);
            }
        }

    } else {
        // If no origin is provided, iterate over all airports
        size_t progress = 0ul; // DEBUG
        for (const auto& [airport_name, airport_node] : nodes) {
            std::cout << "searching airport " << progress << " of " << nodes.size() << std::endl; // DEBUG
            itinerary start_itinerary = itinerary();
            // Find all flights starting from the origin
            for(size_t i = 0; i < flights.size(); ++i){
                std::cout << "searching flight " << i << " of " << flights.size() << std::endl;
                if (flights[flight_id{i}].from == airport_name) {
                    itinerary current_itinerary = itinerary();
                    dfs(current_itinerary, flight_id{i}, 0);
                }
            }
            best_itinerary.built = true;
            //std::cout<< "Current best itinerary: "<<std::endl<<best_itinerary.serialize(flights)<<std::endl;
        }
    }

    return best_itinerary.serialize(flights);
}
 
#ifndef REMOVE_MAIN_FUNC
int main(int argc, char **argv)
{
    // TODO: timing, generate test suite for full dataset and golden_ref

    flight_constraints constrs = cli("naive", argc, argv);
    std::cout << "running naive" << std::endl;

    // Example usage of parser
    std::string directory = "naive_test/top_5_airports_flight_arrival_results";
    std::vector<flight> flights = parse_flights_from_directory(directory, constrs);

    /*//For correctness checking
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
#endif // REMOVE_MAIN_FUNC