#define REMOVE_MAIN_FUNC
#include "../src/serial.cpp"
#undef REMOVE_MAIN_FUNC

#include "catch/catch.hpp"

TEST_CASE("serial top5 depart=ATL d=150 cabin=Economy", "[serial],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .fare_class = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::ATL),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::SERIAL>();

    const std::string expected{
        "1. Southwest flight from ATL @ 8:05 AM to DEN @ 1:00 PM (1 stop in MDW) in Economy for $234\
        2. Delta flight from DEN @ 2:18 PM to DFW @ 10:00 PM (1 stop in ATL) in Economy for $509\
        3. American flight from DFW @ 10:30 PM to DEN @ 3:15 PM+1 (1 stop in PHX) in Economy for $362"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("serial top5 depart=DEN d=150 cabin=Economy", "[serial],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .fare_class = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::DEN),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::SERIAL>();

    const std::string expected{
        "1. Delta flight from DEN @ 2:18 PM to DFW @ 10:00 PM (1 stop in ATL) in Economy for $509\
        2. American flight from DFW @ 10:30 PM to DEN @ 3:15 PM+1 (1 stop in PHX) in Economy for $362"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("serial top5 depart=DFW d=150 cabin=Economy", "[serial],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .fare_class = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::DFW),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::SERIAL>();

    const std::string expected{
        "1. American flight from DFW @ 10:25 AM to DEN @ 11:46 AM (Nonstop) in Economy for $299\
        2. Delta flight from DEN @ 2:18 PM to DFW @ 10:00 PM (1 stop in ATL) in Economy for $509\
        3. American flight from DFW @ 10:30 PM to DEN @ 3:15 PM+1 (1 stop in PHX) in Economy for $362"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("serial top5 depart=LAX d=150 cabin=Economy", "[serial],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .fare_class = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::LAX),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::SERIAL>();

    const std::string expected{
        "1. JetBlue flight from LAX @ 8:45 PM to ORD @ 9:26 AM+1 (1 stop in JFK) in Economy for $659"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("serial top5 depart=ORD d=150 cabin=Economy", "[serial],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .fare_class = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::ORD),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::SERIAL>();

    const std::string expected{
        "1. American flight from ORD @ 11:56 AM to DEN @ 5:51 PM (1 stop in DFW) in Economy for $429\
        2. American flight from DEN @ 6:46 PM to LAX @ 10:03 PM (1 stop in PHX) in Economy for $164"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("serial top5 d=150 cabin=Economy", "[serial],[top5],[quick],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .fare_class = std::make_optional(cabin::ECONOMY),
        .origin = std::nullopt,
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::SERIAL>();

    const std::string expected{
        "1. Southwest flight from ATL @ 8:05 AM to DEN @ 1:00 PM (1 stop in MDW) in Economy for $234\
        2. Delta flight from DEN @ 2:18 PM to DFW @ 10:00 PM (1 stop in ATL) in Economy for $509\
        3. American flight from DFW @ 10:30 PM to DEN @ 3:15 PM+1 (1 stop in PHX) in Economy for $362"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("serial top5 d=3", "[serial],[top5],[quick],[d]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .fare_class = std::nullopt,
        .origin = std::nullopt,
        .start_ts = std::nullopt,
        .div_n = 3
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::SERIAL>();

    const std::string expected{
        "1. American flight from ATL @ 5:10 AM to DFW @ 9:11 AM (1 stop in CLT) in Economy for $289\
        2. American flight from DFW @ 9:15 AM to LAX @ 10:41 AM (Nonstop) in Business for $766\
        3. Delta flight from LAX @ 11:05 AM to DFW @ 5:43 PM (2 stops in CVG, ATL) in Economy for $2071\
        4. American flight from DFW @ 5:55 PM to LAX @ 8:47 PM (1 stop in LAS) in First for $775\
        5. United flight from LAX @ 9:40 PM to DEN @ 12:42 PM+1 (2 stops in PIT, ORD) in Economy for $495"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("serial top5 d=2", "[serial],[top5],[quick],[d]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .fare_class = std::nullopt,
        .origin = std::nullopt,
        .start_ts = std::nullopt,
        .div_n = 2
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::SERIAL>();

    const std::string expected{
        "1. American flight from ATL @ 5:10 AM to DFW @ 11:38 AM (2 stops in CLT, STL) in First for $718\
        2. American flight from DFW @ 12:19 PM to LAX @ 3:22 PM (1 stop in PHX) in Economy for $323\
        3. United flight from LAX @ 4:00 PM to DFW @ 10:44 PM (2 stops in DCA, ORD) in Business for $977\
        4. American flight from DFW @ 10:49 PM to DEN @ 11:58 PM (Nonstop) in First for $259\
        5. American flight from DEN @ 11:59 PM to ATL @ 9:42 PM+1 (1 stop in CLT) in Business for $862"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}
