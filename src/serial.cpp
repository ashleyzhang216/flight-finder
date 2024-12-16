#include "common_data_types.h"
#include "parser.h"

// serial implementation of search
template<>
std::string flight_finder::search<OptLevel::SERIAL>() {
    time_t arrival = 0ul;

    for(size_t i = 0; i < flights.size(); ++i) {
        const flight_id cur_id = flight_id(i);
        const flight_idx cur_idx = flight_indices[cur_id];
        const airport dest_airport = flights[cur_id].to;
        const airport depart_airport = flights[cur_id].from;

        // DEBUG
        assert(flights[cur_id].arrive_ts >= arrival);
        arrival = flights[cur_id].arrive_ts;

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
    }

    std::optional<itinerary> best = origin.has_value() ? std::make_optional(itinerary(origin.value())) : std::nullopt;
    for(const auto& pair : nodes) {
        const size_t opt_size = pair.second.opt_table.size();
        if(opt_size) {
            const itinerary candidate = pair.second.opt_table.vec.back();
            best = best.has_value() ? itinerary::max(best.value(), candidate, origin) : candidate;
        }
    }
    assert(best.has_value());

    // DEBUG
    for(auto id : best.value().flight_ids) std::cout << id.id << ", ";
    std::cout << std::endl;

    return best.value().serialize(flights);
}

#ifndef REMOVE_MAIN_FUNC
int main(int argc, char** argv) {
    flight_constraints constrs = cli("serial", argc, argv);

    std::cout << "running serial" << std::endl;

    // Example usage of parser
    std::string directory = "naive_test/top_5_airports_flight_arrival_results";
    // std::string directory = "flight_correct_ts_arrival";
    // std::string directory = "flight_concurr_arr_results";
    std::vector<flight> flights = parse_flights_from_directory(directory, constrs);

    flight_finder ff(std::move(flights), constrs.origin);
    std::cout << ff.search<OptLevel::SERIAL>() << std::endl;
    
    return 0;
}
#endif // REMOVE_MAIN_FUNC