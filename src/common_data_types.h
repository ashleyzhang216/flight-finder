#ifndef COMMON_DATA_TYPES_H
#define COMMON_DATA_TYPES_H

#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <exception>

/**
 * @brief all unique airlines
 */
enum airline {
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
};

// convert from airline enum to airline name, for printing
extern std::unordered_map<airline, std::string> airline_name;

/**
 * @brief top 50 airports
 */
enum airport {
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
};

// convert from airport IOTA to name, for printing
extern std::unordered_map<airport, std::string> airport_name;

/**
 * @brief types of fare classes offered
 */
enum cabin {
    ECONOMY = 0,
    PREMIUM_ECONOMY = 1,
    BUSINESS = 2,
    FIRST = 3
};

/**
 * @brief one flight
 */
struct flight {
    size_t id;
    airline airline;
    airport from_iota;
    airport to_iota;
    time_t depart_ts; // unix epoch, seconds
    time_t arrive_ts; // unix epoch, seconds
    std::string depart_time; // human readable format
    std::string arrive_time; // human readable format
    std::string stops;
    uint num_stops; // 0 -> nonstop
    cabin cabin;
    uint price; // in USD
};

// TODO: define itinerary

// TODO: define flight finder

// TODO: define airport

// singleton for use with id_vec
struct flight_id {
    size_t id;
};

// singleton for use with id_vec
struct flight_idx {
    size_t id;
};

#endif // COMMON_DATA_TYPES_H