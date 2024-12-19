#include "common_data_types.h"
#include "parser.h"

#include <omp.h>

constexpr size_t NUM_DEP_THREADS = 16;
constexpr size_t NUM_MAX_THREADS = 2;

// parallel implementation of search
template<>
std::string flight_finder::search<OptLevel::PARALLEL>() {
    // to help track dependencies
    id_vec<flight_id, std::optional<flight_id> > deps_incoming(std::vector<std::optional<flight_id> >(flights.size(), std::nullopt));
    id_vec<flight_id, std::optional<flight_id> > deps_prev(std::vector<std::optional<flight_id> >(flights.size(), std::nullopt));

    time_point<high_resolution_clock> start_dep_ts = high_resolution_clock::now();

    // #pragma omp parallel num_threads(NUM_DEP_THREADS)
    // {
        // returns optional including flight_id of incoming flight dependency, if it exists
        auto get_incoming = [this](size_t i, const flight_id& cur_id, const airport& depart_airport) -> std::optional<flight_id> {
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
                // no itinerary in time, no incoming dependency
                return std::nullopt;
            }

            flight_id best_id = *(it - 1);
            assert(depart_airport == flights[best_id].to);
            return std::make_optional(best_id);
        };

        // returns optional including flight_id of prev flight dependency, if it exists
        auto get_prev = [this](size_t i, const flight_idx& cur_idx, const airport& dest_airport) -> std::optional<flight_id> {
            if(cur_idx.id == 0ul) {
                // this is first flight arriving at dest airport, no earlier option exists
                return std::nullopt;
            }

            // return flight_id of one lower flight_idx
            return std::make_optional(nodes.at(dest_airport).arriving_flights[flight_idx(cur_idx.id - 1ul)]);
        };

        #pragma omp parallel for num_threads(NUM_DEP_THREADS)
        for(size_t i = 0; i < flights.size(); ++i) {
            const flight_id cur_id = flight_id(i);
            const flight_idx cur_idx = flight_indices[cur_id];
            const airport dest_airport = flights[cur_id].to;
            const airport depart_airport = flights[cur_id].from;

            // #pragma omp critical
            // {
            //     printf("finding deps of %lu with thread: %i\n", i, omp_get_thread_num());
            // }
            deps_incoming[cur_id] = get_incoming(i, cur_id, depart_airport);
            deps_prev[cur_id] = get_prev(i, cur_idx, dest_airport);
        }
    // }

    time_point<high_resolution_clock> start_build_ts = high_resolution_clock::now();

    // #pragma omp parallel num_threads(NUM_MAX_THREADS)
    // {
        // to help track resolved dependencies
        std::vector<int> built(flights.size(), 0);

        // analogous to one loop iteration in serial
        auto build_single_flight = [this, &built](size_t i, const itinerary& incoming, const itinerary& prev) {
            const flight_id cur_id = flight_id(i);
            const flight_idx cur_idx = flight_indices[cur_id];
            const airport dest_airport = flights[cur_id].to;

            nodes.at(dest_airport).opt_table[cur_idx] = itinerary::max(incoming, prev, origin);

            assert(built[i] == 0);
            built[i] = 1;
        };

        // #pragma omp single nowait
        for(size_t i = 0; i < flights.size(); ++i) {
            const flight_id cur_id = flight_id(i);
            const airport dest_airport = flights[cur_id].to;
            const airport depart_airport = flights[cur_id].from;

            const bool has_incoming = deps_incoming[cur_id].has_value();
            const bool has_prev     = deps_prev[cur_id].has_value();

            if(has_incoming) {
                if(has_prev) {
                    const itinerary& incoming = nodes.at(depart_airport).opt_table[flight_indices[deps_incoming[cur_id].value()]];
                    const itinerary& prev     = nodes.at(dest_airport).opt_table[flight_indices[deps_prev[cur_id].value()]];

                    const size_t dep_i = deps_incoming[cur_id].value().id;
                    const size_t dep_p = deps_prev[cur_id].value().id;

                    // #pragma omp critical
                    // {
                    //     printf("building %lu with thread: %i\n", i, omp_get_thread_num());
                    // }
                    // #pragma omp task depend(out: *(built.data() + i)) depend(in: *(built.data() + dep_i), *(built.data() + dep_p))
                    build_single_flight(i, incoming.add(cur_id, flights), prev);
                } else {
                    const itinerary& incoming = nodes.at(depart_airport).opt_table[flight_indices[deps_incoming[cur_id].value()]];

                    const size_t dep_i = deps_incoming[cur_id].value().id;

                    // #pragma omp critical
                    // {
                    //     printf("building %lu with thread: %i\n", i, omp_get_thread_num());
                    // }
                    // #pragma omp task depend(out: *(built.data() + i)) depend(in: *(built.data() + dep_i))
                    build_single_flight(i, incoming.add(cur_id, flights), itinerary(dest_airport));
                }
            } else {
                if(has_prev) {
                    const itinerary& prev     = nodes.at(dest_airport).opt_table[flight_indices[deps_prev[cur_id].value()]];

                    const size_t dep_p = deps_prev[cur_id].value().id;

                    // #pragma omp critical
                    // {
                    //     printf("building %lu with thread: %i\n", i, omp_get_thread_num());
                    // }
                    // #pragma omp task depend(out: *(built.data() + i)) depend(in: *(built.data() + dep_p))
                    build_single_flight(i, itinerary(depart_airport).add(cur_id, flights), prev);
                } else {
                    // #pragma omp critical
                    // {
                    //     printf("building %lu with thread: %i\n", i, omp_get_thread_num());
                    // }
                    // #pragma omp task depend(out: *(built.data() + i))
                    build_single_flight(i, itinerary(depart_airport).add(cur_id, flights), itinerary(dest_airport));
                }
            }
        }
    // }

    time_point<high_resolution_clock> start_max_ts = high_resolution_clock::now();

    std::optional<itinerary> best = origin.has_value() ? std::make_optional(itinerary(origin.value())) : std::nullopt;
    for(const auto& pair : nodes) {
        const size_t opt_size = pair.second.opt_table.size();
        if(opt_size) {
            const itinerary candidate = pair.second.opt_table.vec.back();
            best = best.has_value() ? itinerary::max(best.value(), candidate, origin) : candidate;
        }
    }
    assert(best.has_value());

    time_point<high_resolution_clock> end_ts = high_resolution_clock::now();

    std::cout << "finding deps: " << std::chrono::duration_cast<std::chrono::milliseconds>(start_build_ts - start_dep_ts).count() << "ms" << std::endl;
    std::cout << "building: " << std::chrono::duration_cast<std::chrono::milliseconds>(start_max_ts - start_build_ts).count() << "ms" << std::endl;
    std::cout << "finding max: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_ts - start_max_ts).count() << "ms" << std::endl;

    return best.value().serialize(flights);
}

#ifndef REMOVE_MAIN_FUNC
int main(int argc, char** argv) {
    flight_constraints constrs = cli("parallel", argc, argv);

    std::cout << "running parallel" << std::endl;

    // std::string directory = "naive_test/top_5_airports_flight_arrival_results";
    std::string directory = "flight_concurr_arr_results";
    std::vector<flight> flights = parse_flights_from_directory(directory, constrs);

    flight_finder ff(std::move(flights), constrs.origin);
    
    time_point<high_resolution_clock> start = high_resolution_clock::now();
    asm volatile ("" ::: "memory");
    std::cout << ff.search<OptLevel::PARALLEL>() << std::endl;
    asm volatile ("" ::: "memory");
    time_point<high_resolution_clock> end = high_resolution_clock::now();

    auto execution_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "execution time: " << execution_ms << "ms" << std::endl;
    
    return 0;
}
#endif // REMOVE_MAIN_FUNC