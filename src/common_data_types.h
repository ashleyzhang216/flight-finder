#ifndef COMMON_DATA_TYPES_H
#define COMMON_DATA_TYPES_H

#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <exception>

namespace common_data_types_ns
{
    /**
     * @brief all unique airlines
     */
    enum airline
    {
        Delta,
        Southwest,
        American,
        Allegiant,
        Alaska,
        Southern,
        Tropic,
        Hawaiian,
        JetBlue,
        Key,
        Sun,
        Contour,
        Avelo,
        Swiss,
        Cape,
        Spirit,
        British,
        SkyWest,
        Breeze,
        United,
        Lufthansa,
        Frontier,
        Silver,
        ITA,
        JSX,
    };

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
    };

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

    /**
     * @brief one flight
     */
    struct flight
    {
        size_t id;
        airline airline;
        airport from_iota;
        airport to_iota;
        time_t depart_ts;        // unix epoch, seconds
        time_t arrive_ts;        // unix epoch, seconds
        std::string depart_time; // human readable format
        std::string arrive_time; // human readable format
        std::string stops;
        uint num_stops; // 0 -> nonstop
        cabin cabin;
        uint price; // in USD
    };

    // convert from airport IOTA to name, for printing
    std::unordered_map<airport, std::string> airport_name = std::unordered_map<airport, std::string>({
        {ATL, "Hartsfield–Jackson Atlanta International Airport"},
        {LAX, "Los Angeles International Airport"},
        {DFW, "Dallas/Fort Worth International Airport"},
        {DEN, "Denver International Airport"},
        {ORD, "O'Hare International Airport"},
        {JFK, "John F. Kennedy International Airport"},
        {MCO, "Orlando International Airport"},
        {LAS, "Harry Reid International Airport"},
        {CLT, "Charlotte Douglas International Airport"},
        {MIA, "Miami International Airport"},
        {SEA, "Seattle–Tacoma International Airport"},
        {EWR, "Newark Liberty International Airport"},
        {SFO, "San Francisco International Airport"},
        {PHX, "Phoenix Sky Harbor International Airport"},
        {IAH, "George Bush Intercontinental Airport"},
        {BOS, "Logan International Airport"},
        {FLL, "Fort Lauderdale–Hollywood International Airport"},
        {MSP, "Minneapolis–Saint Paul International Airport"},
        {LGA, "LaGuardia Airport"},
        {DTW, "Detroit Metropolitan Airport"},
        {PHL, "Philadelphia International Airport"},
        {SLC, "Salt Lake City International Airport"},
        {BWI, "Baltimore/Washington International Airport"},
        {DCA, "Ronald Reagan Washington National Airport"},
        {SAN, "San Diego International Airport"},
        {IAD, "Dulles International Airport"},
        {TPA, "Tampa International Airport"},
        {BNA, "Nashville International Airport"},
        {AUS, "Austin–Bergstrom International Airport"},
        {MDW, "Midway International Airport"},
        {HNL, "Daniel K. Inouye International Airport"},
        {DAL, "Dallas Love Field"},
        {PDX, "Portland International Airport"},
        {STL, "St. Louis Lambert International Airport"},
        {RDU, "Raleigh-Durham International Airport"},
        {HOU, "William P. Hobby Airport"},
        {SMF, "Sacramento International Airport"},
        {MSY, "Louis Armstrong New Orleans International Airport"},
        {SJC, "Norman Y. Mineta San José International Airport"},
        {SNA, "John Wayne Airport"},
        {MCI, "Kansas City International Airport"},
        {OAK, "San Francisco Bay Oakland International Airport"},
        {SAT, "San Antonio International Airport"},
        {RSW, "Southwest Florida International Airport"},
        {CLE, "Cleveland Hopkins International Airport"},
        {IND, "Indianapolis International Airport"},
        {PIT, "Pittsburgh International Airport"},
        {CVG, "Cincinnati/Northern Kentucky International Airport"},
        {CMH, "John Glenn Columbus International Airport"},
        {PBI, "Palm Beach International Airport"},
    });

    // singleton for use with id_vec
    struct flight_id
    {
        size_t id;
    };

    // singleton for use with id_vec
    struct flight_idx
    {
        size_t id;
    };

}; // common_data_types_ns

#endif // COMMON_DATA_TYPES_H