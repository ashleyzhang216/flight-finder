#include "common_data_types.h"

// serial implementation of search
std::string flight_finder::search() {
    for(size_t i = 0; i < flights.size(); ++i) {
        const flight_id cur_id = flight_id(i);
        const flight_idx cur_idx = flight_indices[cur_id];
        const airport dest_airport = flights[cur_id].to;
        const airport depart_airport = flights[cur_id].from;

        auto get_incoming = [this, &cur_id, &depart_airport]() -> itinerary {
            auto comp = [this](const flight_id& lhs, const time_t& rhs) -> bool {
                return flights[lhs].arrive_ts < rhs;
            };
            auto it = std::lower_bound(
                nodes.at(depart_airport).arriving_flights.vec.begin(),
                nodes.at(depart_airport).arriving_flights.vec.end(),
                flights[cur_id].depart_ts,
                comp
            );

            if(it == nodes.at(depart_airport).arriving_flights.vec.begin()) {
                // no itinerary in time, use blank itinerary
                return itinerary(depart_airport).add(cur_id, flights);
            }

            flight_id best_id = *(it - 1);
            itinerary& prev = nodes.at(depart_airport).opt_table[flight_indices[best_id]];
            assert(prev.built);
            
            return prev.add(cur_id, flights);
        };

        itinerary incoming = get_incoming();
        itinerary prev     = (cur_idx.id == 0ul) ? itinerary(dest_airport) : nodes.at(dest_airport).opt_table[flight_idx(cur_idx.id - 1ul)];
        itinerary best     = itinerary::max(incoming, prev, origin);

        nodes.at(dest_airport).opt_table[cur_idx] = std::move(best);
        // _mm_sfence();
        nodes.at(dest_airport).opt_table[cur_idx].built = true;

        // call std::lower_bound() to find last opt_table value of depart_airport that arrives before flights[cur_id].depart_ts

        // add that opt_table value to flights[cur_id]
        // get blank itinerary for dest_airport if cur_idx == 0, or dest_airport opt_table entry at cur_idx - 1

        // get max of these 2, set as opt_table[cur_idx] for dest_airport, then set built as true
    }

    // get max of last opt_table entry for all airports
    
    itinerary best = itinerary(origin.value_or(static_cast<airport>(0ul)));
    for(const auto& pair : nodes) {
        best = itinerary::max(best, pair.second.opt_table.vec.back(), origin);
    }

    return best.serialize(flights);
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
