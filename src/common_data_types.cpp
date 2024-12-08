#include "common_data_types.h"

// parse input parameters
flight_constraints cli(const std::string &name, int argc, char **argv)
{
    cxxopts::Options options(name, "Find different flight routes");

    options.add_options()("a,airlines", "Allowed airlines, default: all", cxxopts::value<std::vector<std::string>>())("c,cabin", "Allowed cabin, default: all", cxxopts::value<std::string>())("o,origin", "Origin airport code", cxxopts::value<std::string>())("s,start", "Earliest departure time, default: any", cxxopts::value<uint>()) // take std::string in HH:MM and convert to ts
        ("e,end", "Latest arrival time, default: any", cxxopts::value<uint>())                                                                                                                                                                                                                                                               // take std::string in HH:MM and convert to ts
        ("h,help", "Print usage");

    auto result = options.parse(argc, argv);

    // ############### help ###############

    // print help msg if specified
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    flight_constraints constrs;

    // ############### airline ##############

    if (result.count("airlines"))
    {
        std::vector<std::string> airline_names = result["airlines"].as<std::vector<std::string>>();

        std::vector<airline> airlines;
        for (const std::string &name : airline_names)
        {
            airlines.push_back(airline_of_str.at(name));
        }

        constrs.airlines = std::make_optional<std::vector<airline>>(std::move(airlines));
    }

    // ############### cabin ###############

    if (result.count("cabin"))
    {
        constrs.cabin = std::make_optional<cabin>(cabin_of_str.at(result["cabin"].as<std::string>()));
    }

    // ############### origin ###############

    if (result.count("origin"))
    {
        constrs.origin = std::make_optional<airport>(airport_of_str.at(result["origin"].as<std::string>()));
    }

    // ############### start ###############

    if (result.count("start"))
    {
        constrs.start_ts = std::make_optional<time_t>(result["start"].as<uint>());
    }

    // ############### end ###############

    if (result.count("end"))
    {
        constrs.end_ts = std::make_optional<time_t>(result["end"].as<uint>());
    }

    return constrs;
}

// print flight
std::string flight::serialize() const
{
    std::stringstream ss;

    ss << airline << " flight from ";
    ss << airport_code.at(from_iota) << " @ " << depart_time << " to ";
    ss << airport_code.at(to_iota) << " @ " << arrive_time;
    ss << " (" << stops << ") in " << cabin_name.at(cabin) << " for $" << price << std::endl;

    return ss.str();
}

// print itinerary
std::string itinerary::serialize(const id_vec<flight_id, flight> flights) const
{
    assert(built);

    std::stringstream ss;
    for (size_t i = 0; i < flight_ids.size(); ++i)
    {
        ss << std::to_string(i + 1ul) << ". " << flights[flight_ids[i]].serialize();
    }

    return ss.str();
}

// convert from cabin to name, for printing
std::unordered_map<cabin, std::string> cabin_name = std::unordered_map<cabin, std::string>({
    {ECONOMY, "Economy"},
    {PREMIUM_ECONOMY, "Premium Economy"},
    {BUSINESS, "Business"},
    {FIRST, "First"},
});

// convert from airline enum to airline name, for printing
std::unordered_map<airline, std::string> airline_name = std::unordered_map<airline, std::string>({
    {DELTA, "Delta"},
    {SOUTHWEST, "Southwest"},
    {AMERICAN, "American"},
    {ALLEGIANT, "Allegiant"},
    {ALASKA, "Alaska"},
    {SOUTHERN, "Southern"},
    {TROPIC, "Tropic"},
    {HAWAIIAN, "Hawaiian"},
    {JETBLUE, "JetBlue"},
    {KEY, "Key"},
    {SUN, "Sun"},
    {CONTOUR, "Contour"},
    {AVELO, "Avelo"},
    {SWISS, "Swiss"},
    {CAPE, "Cape"},
    {SPIRIT, "Spirit"},
    {BRITISH, "British"},
    {SKYWEST, "SkyWest"},
    {BREEZE, "Breeze"},
    {UNITED, "United"},
    {LUFTHANSA, "Lufthansa"},
    {FRONTIER, "Frontier"},
    {SILVER, "Silver"},
    {ITA, "ITA"},
    {JSX, "JSX"},
});

// convert user input
std::unordered_map<std::string, airline> airline_of_str = std::unordered_map<std::string, airline>({
    {"Delta", DELTA},
    {"Southwest", SOUTHWEST},
    {"American", AMERICAN},
    {"Allegiant", ALLEGIANT},
    {"Alaska", ALASKA},
    {"Southern", SOUTHERN},
    {"Tropic", TROPIC},
    {"Hawaiian", HAWAIIAN},
    {"JetBlue", JETBLUE},
    {"Key", KEY},
    {"Sun", SUN},
    {"Contour", CONTOUR},
    {"Avelo", AVELO},
    {"Swiss", SWISS},
    {"Cape", CAPE},
    {"Spirit", SPIRIT},
    {"British", BRITISH},
    {"SkyWest", SKYWEST},
    {"Breeze", BREEZE},
    {"United", UNITED},
    {"Lufthansa", LUFTHANSA},
    {"Frontier", FRONTIER},
    {"Silver", SILVER},
    {"ITA", ITA},
    {"JSX", JSX},
});

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

// convert from airport IOTA to IOTA string, for printing
std::unordered_map<airport, std::string> airport_code = std::unordered_map<airport, std::string>({
    {ATL, "ATL"},
    {LAX, "LAX"},
    {DFW, "DFW"},
    {DEN, "DEN"},
    {ORD, "ORD"},
    {JFK, "JFK"},
    {MCO, "MCO"},
    {LAS, "LAS"},
    {CLT, "CLT"},
    {MIA, "MIA"},
    {SEA, "SEA"},
    {EWR, "EWR"},
    {SFO, "SFO"},
    {PHX, "PHX"},
    {IAH, "IAH"},
    {BOS, "BOS"},
    {FLL, "FLL"},
    {MSP, "MSP"},
    {LGA, "LGA"},
    {DTW, "DTW"},
    {PHL, "PHL"},
    {SLC, "SLC"},
    {BWI, "BWI"},
    {DCA, "DCA"},
    {SAN, "SAN"},
    {IAD, "IAD"},
    {TPA, "TPA"},
    {BNA, "BNA"},
    {AUS, "AUS"},
    {MDW, "MDW"},
    {HNL, "HNL"},
    {DAL, "DAL"},
    {PDX, "PDX"},
    {STL, "STL"},
    {RDU, "RDU"},
    {HOU, "HOU"},
    {SMF, "SMF"},
    {MSY, "MSY"},
    {SJC, "SJC"},
    {SNA, "SNA"},
    {MCI, "MCI"},
    {OAK, "OAK"},
    {SAT, "SAT"},
    {RSW, "RSW"},
    {CLE, "CLE"},
    {IND, "IND"},
    {PIT, "PIT"},
    {CVG, "CVG"},
    {CMH, "CMH"},
    {PBI, "PBI"},
});

// convert user input
std::unordered_map<std::string, airport> airport_of_str = std::unordered_map<std::string, airport>({
    {"ATL", ATL},
    {"LAX", LAX},
    {"DFW", DFW},
    {"DEN", DEN},
    {"ORD", ORD},
    {"JFK", JFK},
    {"MCO", MCO},
    {"LAS", LAS},
    {"CLT", CLT},
    {"MIA", MIA},
    {"SEA", SEA},
    {"EWR", EWR},
    {"SFO", SFO},
    {"PHX", PHX},
    {"IAH", IAH},
    {"BOS", BOS},
    {"FLL", FLL},
    {"MSP", MSP},
    {"LGA", LGA},
    {"DTW", DTW},
    {"PHL", PHL},
    {"SLC", SLC},
    {"BWI", BWI},
    {"DCA", DCA},
    {"SAN", SAN},
    {"IAD", IAD},
    {"TPA", TPA},
    {"BNA", BNA},
    {"AUS", AUS},
    {"MDW", MDW},
    {"HNL", HNL},
    {"DAL", DAL},
    {"PDX", PDX},
    {"STL", STL},
    {"RDU", RDU},
    {"HOU", HOU},
    {"SMF", SMF},
    {"MSY", MSY},
    {"SJC", SJC},
    {"SNA", SNA},
    {"MCI", MCI},
    {"OAK", OAK},
    {"SAT", SAT},
    {"RSW", RSW},
    {"CLE", CLE},
    {"IND", IND},
    {"PIT", PIT},
    {"CVG", CVG},
    {"CMH", CMH},
    {"PBI", PBI},
});

// convert user input
std::unordered_map<std::string, cabin> cabin_of_str = std::unordered_map<std::string, cabin>({
    {"Economy", ECONOMY},
    {"Premium", PREMIUM_ECONOMY},
    {"Business", BUSINESS},
    {"First", FIRST},
});
