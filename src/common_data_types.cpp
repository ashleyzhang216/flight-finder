#include "common_data_types.h"

// print flight
std::string flight::serialize() const {
    std::stringstream ss;

    ss << airline << " flight from ";
    ss << airport_code.at(from_iota) << " @ " << depart_time << " to ";
    ss << airport_code.at(to_iota) << " @ " << arrive_time;
    ss << " (" << stops << ") in " << cabin_name.at(cabin) << " for $" << price << std::endl; 

    return ss.str();
}

// print itinerary
std::string itinerary::serialize(const id_vec<flight_id, flight> flights) const {
    assert(built);
    
    std::stringstream ss;
    for(size_t i = 0; i < flight_ids.size(); ++i) {
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
    {DELTA,	"Delta"},
    {SOUTHWEST,	"Southwest"},
    {AMERICAN,	"American"},
    {ALLEGIANT,	"Allegiant"},
    {ALASKA,	"Alaska"},
    {SOUTHERN,	"Southern"},
    {TROPIC,	"Tropic"},
    {HAWAIIAN,	"Hawaiian"},
    {JETBLUE,	"JetBlue"},
    {KEY,	"Key"},
    {SUN,	"Sun"},
    {CONTOUR,	"Contour"},
    {AVELO,	"Avelo"},
    {SWISS,	"Swiss"},
    {CAPE,	"Cape"},
    {SPIRIT,	"Spirit"},
    {BRITISH,	"British"},
    {SKYWEST,	"SkyWest"},
    {BREEZE,	"Breeze"},
    {UNITED,	"United"},
    {LUFTHANSA,	"Lufthansa"},
    {FRONTIER,	"Frontier"},
    {SILVER,	"Silver"},
    {ITA,	"ITA"},
    {JSX,	"JSX"},
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