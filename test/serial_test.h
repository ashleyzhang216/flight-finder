#define REMOVE_MAIN_FUNC
#include "../src/serial.cpp"
#undef REMOVE_MAIN_FUNC

#include "catch/catch.hpp"

TEST_CASE("serial top5 depart=ATL d=150 cabin=Economy", "[serial],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::ATL),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search();

    const std::string expected{
        "1. American flight from ATL @ 5:20 AM to DFW @ 6:59 AM (Nonstop) in Economy for $177\
        2. American flight from DFW @ 10:25 AM to DEN @ 11:46 AM (Nonstop) in Economy for $299\
        3. Delta flight from DEN @ 2:18 PM to DFW @ 10:00 PM (1 stop in ATL) in Economy for $509\
        4. American flight from DFW @ 10:30 PM to DEN @ 3:15 PM+1 (1 stop in PHX) in Economy for $362"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("serial top5 depart=DEN d=150 cabin=Economy", "[serial],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::DEN),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search();

    const std::string expected{
        "1. Delta flight from DEN @ 2:18 PM to DFW @ 10:00 PM (1 stop in ATL) in Economy for $509\
        2. American flight from DFW @ 10:30 PM to DEN @ 3:15 PM+1 (1 stop in PHX) in Economy for $362"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("serial top5 depart=DFW d=150 cabin=Economy", "[serial],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::DFW),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search();

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
        .cabin = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::LAX),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search();

    const std::string expected{
        "1. JetBlue flight from LAX @ 8:45 PM to ORD @ 9:26 AM+1 (1 stop in JFK) in Economy for $659"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("serial top5 depart=ORD d=150 cabin=Economy", "[serial],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::ORD),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search();

    const std::string expected{
        "1. American flight from ORD @ 11:56 AM to DEN @ 5:51 PM (1 stop in DFW) in Economy for $429\
        2. American flight from DEN @ 6:46 PM to LAX @ 10:03 PM (1 stop in PHX) in Economy for $164"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("serial top5 d=150 cabin=Economy", "[serial],[top5],[quick],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::make_optional(cabin::ECONOMY),
        .origin = std::nullopt,
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search();

    const std::string expected{
        "1. American flight from ATL @ 5:20 AM to DFW @ 6:59 AM (Nonstop) in Economy for $177\
        2. American flight from DFW @ 10:25 AM to DEN @ 11:46 AM (Nonstop) in Economy for $299\
        3. Delta flight from DEN @ 2:18 PM to DFW @ 10:00 PM (1 stop in ATL) in Economy for $509\
        4. American flight from DFW @ 10:30 PM to DEN @ 3:15 PM+1 (1 stop in PHX) in Economy for $362"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

/*
TODO: check if naive golden ref matches this

TEST_CASE("serial top5 all", "[serial],[top5],[all]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin =std::nullopt,
        .origin = std::nullopt,
        .start_ts = std::nullopt,
        .div_n = 1
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search();

    const std::string expected{
        "1. American flight from ATL @ 5:20 AM to DFW @ 6:59 AM (Nonstop) in First for $382\
        2. American flight from DFW @ 7:19 AM to DEN @ 8:32 AM (Nonstop) in Business for $359\
        3. Frontier flight from DEN @ 8:36 AM to DFW @ 11:40 AM (Nonstop) in Economy for $150\
        4. Frontier flight from DFW @ 11:45 AM to ATL @ 2:56 PM (Nonstop) in Economy for $118\
        5. Delta flight from ATL @ 3:10 PM to ORD @ 4:12 PM (Nonstop) in Business for $464\
        6. Southwest flight from ORD @ 4:20 PM to DEN @ 6:05 PM (Nonstop) in Economy for $194\
        7. American flight from DEN @ 6:46 PM to LAX @ 10:03 PM (1 stop in PHX) in Economy for $164\
        8. Spirit flight from LAX @ 10:39 PM to ATL @ 8:38 PM+1 (2 stops in DTW, FLL) in Economy for $260"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}
*/
