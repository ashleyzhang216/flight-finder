#ifndef COMMON_DATA_TYPES_H
#define COMMON_DATA_TYPES_H

#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <optional>
#include <exception>
#include <algorithm>
// #include <immintrin.h>

#include "utils.h"
#include "lib/src/cxxopts.hpp"

/**
 * @brief all unique airlines
 */
enum airline
{
    DELTA,
    SOUTHWEST,
    AMERICAN,
    ALLEGIANT,
    ALASKA,
    SOUTHERN,
    TROPIC,
    HAWAIIAN,
    JETBLUE,
    KEY,
    SUN,
    CONTOUR,
    AVELO,
    SWISS,
    CAPE,
    SPIRIT,
    BRITISH,
    SKYWEST,
    BREEZE,
    UNITED,
    LUFTHANSA,
    FRONTIER,
    SILVER,
    ITA,
    JSX,
    INVALID_AIRLINE
};

// convert from airline enum to airline name, for printing
extern std::unordered_map<airline, std::string> airline_name;

// convert user input
extern std::unordered_map<std::string, airline> airline_of_str;

/**
 * @brief top 50 airports
 */
enum airport
{
    ATL,
    LAX,
    DFW,
    DEN,
    ORD,
    JFK,
    MCO,
    LAS,
    CLT,
    MIA,
    SEA,
    EWR,
    SFO,
    PHX,
    IAH,
    BOS,
    FLL,
    MSP,
    LGA,
    DTW,
    PHL,
    SLC,
    BWI,
    DCA,
    SAN,
    IAD,
    TPA,
    BNA,
    AUS,
    MDW,
    HNL,
    DAL,
    PDX,
    STL,
    RDU,
    HOU,
    SMF,
    MSY,
    SJC,
    SNA,
    MCI,
    OAK,
    SAT,
    RSW,
    CLE,
    IND,
    PIT,
    CVG,
    CMH,
    PBI,
    INVALID_AIRPORT
};

// convert from airport IOTA to name, for printing
extern std::unordered_map<airport, std::string> airport_name;

// convert from airport IOTA to IOTA string, for printing
extern std::unordered_map<airport, std::string> airport_code;

// convert user input
extern std::unordered_map<std::string, airport> airport_of_str;

/**
 * @brief types of fare classes offered
 */
enum cabin
{
    ECONOMY = 0,
    PREMIUM_ECONOMY = 1,
    BUSINESS = 2,
    FIRST = 3
};

// convert from cabin to name, for printing
extern std::unordered_map<cabin, std::string> cabin_name;

// convert user input
extern std::unordered_map<std::string, cabin> cabin_of_str;

/**
 * @brief one flight
 */
struct flight
{
    size_t id;
    airline airline;
    airport from;
    airport to;
    time_t depart_ts;        // unix epoch, seconds
    time_t arrive_ts;        // unix epoch, seconds
    std::string depart_time; // human readable format
    std::string arrive_time; // human readable format
    std::string stops;
    uint num_stops; // 0 -> nonstop
    cabin cabin;
    uint price; // in USD

    std::string serialize() const;
};

// singleton for use with id_vec
struct flight_id
{
    size_t id;

    flight_id(size_t id) : id(id) {}
};

// singleton for use with id_vec
struct flight_idx
{
    size_t id;

    flight_idx(size_t id) : id(id) {}
};

/**
 * @brief represents flights strung together
 */
struct itinerary
{
    std::vector<flight_id> flight_ids;
    airport origin;
    uint legs;
    bool built;

    // standard constructor for opt table
    itinerary() : legs(0u), built(false) {}

    // for default, empty itinerary, used for opt_table[-1]
    itinerary(airport o) : origin(o), legs(0u), built(true) {}

    // print
    std::string serialize(const id_vec<flight_id, flight> flights) const;

    // returns better of two itinerary, taking mandated origin into account
    static itinerary max(const itinerary& lhs, const itinerary& rhs, const std::optional<airport>& origin) {
        // choose itinerary from right origin, if one of them has it
        if(origin.has_value() && lhs.origin != rhs.origin) {
            if(lhs.origin == origin.value()) {
                itinerary next = lhs;
                next.built = false; // caller needs to set this to true, after a compiler fence
                return next;
            } else if(rhs.origin == origin.value()) {
                itinerary next = rhs;
                next.built = false; // caller needs to set this to true, after a compiler fence
                return next;
            }
        }

        // choose itinerary with more legs
        if(lhs.legs != rhs.legs) {
            itinerary next = lhs.legs > rhs.legs ? lhs : rhs;
            next.built = false; // caller needs to set this to true, after a compiler fence
            return next;
        }

        // tiebreaker: choose itinerary with more flights
        if(lhs.flight_ids.size() != rhs.flight_ids.size()) {
            itinerary next = lhs.flight_ids.size() > rhs.flight_ids.size() ? lhs : rhs;
            next.built = false; // caller needs to set this to true, after a compiler fence
            return next;
        }
        
        // tiebreaker: choose first itinerary with higher flight id
        for(size_t i = 0; i < lhs.flight_ids.size(); ++i) {
            if(lhs.flight_ids.at(i).id != rhs.flight_ids.at(i).id) {
                itinerary next = lhs.flight_ids.at(i).id > rhs.flight_ids.at(i).id ? lhs : rhs;
                next.built = false; // caller needs to set this to true, after a compiler fence
                return next;
            }
        }
        
        // truly equal
        itinerary next = lhs;
        next.built = false; // caller needs to set this to true, after a compiler fence
        return next;
    }

    // returns new itinerary as the result of appending a flight to this itinerary
    itinerary add(flight_id id, const id_vec<flight_id, flight>& flights) const {
        itinerary next = *this;

        // ensure next flight takes off from the airport we currently at
        assert(next.flight_ids.size() == 0 || flights[next.flight_ids.back()].to == flights[id].from);

        next.flight_ids.push_back(id);
        next.origin = flights[next.flight_ids.front()].from;
        next.legs += flights[id].num_stops + 1u;
        next.built = false; // caller needs to set this to true, after a compiler fence

        return next;
    }
};

/**
 * @brief restrictions on flights allowed to be used
 * @note if specified, each item is the only thing allowed
 */
struct flight_constraints
{
    std::optional<std::vector<airline>> airlines; // only airlines we want to fly on
    std::optional<cabin> cabin;                   // only cabin we want to fly in
    std::optional<airport> origin;                // airport all itineraries have to depart from
    std::optional<time_t> start_ts;               // first ts at which a flight in our itinerary can take off
    std::optional<time_t> end_ts;                 // last ts at which a flight in our itinerary can land
    std::optional<uint> div_n;                    // taking a mod portion to limit the number of flights considered for queries (e.g. mod 5 --> 20% of N)
};

/**
 * @brief represents one airport and all its incoming flights
 */
struct airport_node
{
    airport_node() = default;

    // if built, opt_table[idx] is best itinerary after flight idx arrives
    id_vec<flight_idx, itinerary> opt_table;

    // all inbound flights, sorted by arrival time, increaing
    id_vec<flight_idx, flight_id> arriving_flights;
};

/**
 * @brief parent class
 */
class flight_finder
{
public:
    flight_finder(std::vector<flight> &&f, const std::optional<airport> &origin);
    
    // naive/serial/parallel only differ from having different implementations for this function
    // return result of serialize() call on best itinerary
    std::string search();

protected:
    // origin airport, if has value
    std::optional<airport> origin;

    // all flights
    id_vec<flight_id, flight> flights;

    // flight_idx in corresponding airport, but still need to find out what airport this is
    // unused for naive
    id_vec<flight_id, flight_idx> flight_indices;

    // all airport nodes, 50 in total
    // unused for naive
    std::unordered_map<airport, airport_node> nodes;
};

// parse params
flight_constraints cli(const std::string &name, int argc, char **argv);

#endif // COMMON_DATA_TYPES_H