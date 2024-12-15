#define REMOVE_MAIN_FUNC
#include "../src/naive.cpp"
#undef REMOVE_MAIN_FUNC

#include "catch/catch.hpp"

TEST_CASE("naive top5 depart=ATL d=150 cabin=Economy", "[naive],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::ATL),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::NAIVE>();

    const std::string expected{
        "1. American flight from ATL @ 5:20 AM to DFW @ 6:59 AM (Nonstop) in Economy for $177\
        2. American flight from DFW @ 10:25 AM to DEN @ 11:46 AM (Nonstop) in Economy for $299\
        3. Delta flight from DEN @ 2:18 PM to DFW @ 10:00 PM (1 stop in ATL) in Economy for $509\
        4. American flight from DFW @ 10:30 PM to DEN @ 3:15 PM+1 (1 stop in PHX) in Economy for $362"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("naive top5 depart=DEN d=150 cabin=Economy", "[naive],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::DEN),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::NAIVE>();

    const std::string expected{
        "1. Delta flight from DEN @ 2:18 PM to DFW @ 10:00 PM (1 stop in ATL) in Economy for $509\
        2. American flight from DFW @ 10:30 PM to DEN @ 3:15 PM+1 (1 stop in PHX) in Economy for $362"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("naive top5 depart=DFW d=150 cabin=Economy", "[naive],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::DFW),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::NAIVE>();

    const std::string expected{
        "1. American flight from DFW @ 10:25 AM to DEN @ 11:46 AM (Nonstop) in Economy for $299\
        2. Delta flight from DEN @ 2:18 PM to DFW @ 10:00 PM (1 stop in ATL) in Economy for $509\
        3. American flight from DFW @ 10:30 PM to DEN @ 3:15 PM+1 (1 stop in PHX) in Economy for $362"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("naive top5 depart=LAX d=150 cabin=Economy", "[naive],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::LAX),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::NAIVE>();

    const std::string expected{
        "1. JetBlue flight from LAX @ 8:45 PM to ORD @ 9:26 AM+1 (1 stop in JFK) in Economy for $659"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("naive top5 depart=ORD d=150 cabin=Economy", "[naive],[top5],[quick],[origin],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::make_optional(cabin::ECONOMY),
        .origin = std::make_optional(airport::ORD),
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::NAIVE>();

    const std::string expected{
        "1. American flight from ORD @ 11:56 AM to DEN @ 5:51 PM (1 stop in DFW) in Economy for $429\
        2. American flight from DEN @ 6:46 PM to LAX @ 10:03 PM (1 stop in PHX) in Economy for $164"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("naive top5 d=150 cabin=Economy", "[naive],[top5],[quick],[d],[cabin]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::make_optional(cabin::ECONOMY),
        .origin = std::nullopt,
        .start_ts = std::nullopt,
        .div_n = 150
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::NAIVE>();

    const std::string expected{
        "1. American flight from ATL @ 5:20 AM to DFW @ 6:59 AM (Nonstop) in Economy for $177\
        2. American flight from DFW @ 10:25 AM to DEN @ 11:46 AM (Nonstop) in Economy for $299\
        3. Delta flight from DEN @ 2:18 PM to DFW @ 10:00 PM (1 stop in ATL) in Economy for $509\
        4. American flight from DFW @ 10:30 PM to DEN @ 3:15 PM+1 (1 stop in PHX) in Economy for $362"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("naive top5 d=3", "[naive],[top5],[d]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::nullopt,
        .origin = std::nullopt,
        .start_ts = std::nullopt,
        .div_n = 3
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::NAIVE>();

    const std::string expected{
        "1. United flight from LAX @ 12:45 AM to DFW @ 8:54 AM (1 stop in IAH) in First for $523\
        2. American flight from DFW @ 8:59 AM to ORD @ 1:10 PM (1 stop in STL) in Business for $403\
        3. Key flight from ORD @ 1:40 PM to DEN @ 4:40 PM (1 stop in ATY) in Economy for $249\
        4. American flight from DEN @ 4:51 PM to DFW @ 7:53 PM (Nonstop) in First for $389\
        5. Frontier flight from DFW @ 8:01 PM to DEN @ 9:19 PM (Nonstop) in Economy for $104\
        6. United flight from DEN @ 9:23 PM to LAX @ 11:00 PM (Nonstop) in First for $305\
        7. United flight from LAX @ 11:05 PM to ATL @ 9:50 PM+1 (1 stop in ORD) in Economy for $434"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}

TEST_CASE("naive top5 d=2", "[naive],[top5],[d]") {
    flight_constraints constrs = {
        .airlines = std::nullopt,
        .cabin = std::nullopt,
        .origin = std::nullopt,
        .start_ts = std::nullopt,
        .div_n = 2
    };
    std::vector<flight> flights = parse_flights_from_directory(data_dir_top5, constrs);
    flight_finder ff(std::move(flights), constrs.origin);
    const std::string result = ff.search<OptLevel::NAIVE>();

    const std::string expected{
        "1. American flight from DFW @ 6:05 AM to DEN @ 7:14 AM (Nonstop) in First for $325\
        2. American flight from DEN @ 7:44 AM to DFW @ 10:47 AM (Nonstop) in Economy for $299\
        3. Key flight from DFW @ 11:15 AM to DEN @ 1:30 PM (1 stop in CVN) in Economy for $135\
        4. United flight from DEN @ 1:30 PM to DFW @ 4:35 PM (Nonstop) in Business for $339\
        5. United flight from DFW @ 5:00 PM to DEN @ 6:18 PM (Nonstop) in Business for $629\
        6. American flight from DEN @ 6:46 PM to LAX @ 10:03 PM (1 stop in PHX) in Business for $274\
        7. Spirit flight from LAX @ 10:39 PM to ATL @ 8:38 PM+1 (2 stops in DTW, FLL) in Economy for $260"
    };

    REQUIRE(remove_whitespace(result) == remove_whitespace(expected));
}
