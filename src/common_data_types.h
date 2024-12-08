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
    uint legs;
    bool built;

    itinerary() : legs(0u), built(false) {}

    // TODO: implement operator< for std::max()
    // this needs to consider both the # of legs, and also origin airport if we specified one

    // TODO: implement + operator to add flight to itinerary

    std::string serialize(const id_vec<flight_id, flight> flights) const;
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
    // TODO: To support strong and weak scaling analysis, can add a paramater/scale factor for number of flights per airport we want to consider (say 100% of flights, etc. which is our N for analysis)
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

    // TODO: implement

protected:
    // origin airport, if has value
    std::optional<airport> origin;

    // all flights
    id_vec<flight_id, flight> flights;

    // flight_idx in corresponding airport, but still need to find out what airport this is
    id_vec<flight_id, flight_idx> flight_indices;

    // all airport nodes, 50 in total
    std::unordered_map<airport, airport_node> nodes;
};

// parse params
flight_constraints cli(const std::string &name, int argc, char **argv);

#endif // COMMON_DATA_TYPES_H