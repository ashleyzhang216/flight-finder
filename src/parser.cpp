#include "parser.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include "lib/src/json.hpp"

using json = nlohmann::json;

airline parse_airline(const std::string &airline_str)
{
    int earliestFoundIndex = -1;
    airline result = INVALID_AIRLINE;
    for (const auto &entry : airline_of_str)
    {
        int foundIndex = airline_str.find(entry.first);
        if (foundIndex != -1)
        {
            if (result == INVALID_AIRLINE || foundIndex < earliestFoundIndex)
            {
                result = entry.second;
                earliestFoundIndex = foundIndex;
            }
        }
    }
    return result;
}

airport parse_airport(const std::string &airport_str)
{
    auto it = airport_of_str.find(airport_str);
    if (it != airport_of_str.end())
    {
        return it->second;
    }
    return airport::INVALID_AIRPORT;
}

cabin parse_cabin(const std::string &cabin_str)
{
    auto it = cabin_of_str.find(cabin_str);
    if (it != cabin_of_str.end())
    {
        return it->second;
    }
    return cabin::ECONOMY;
}

// Function to parse a single flight from JSON
flight parse_flight(const json &flight_data, size_t flight_id)
{
    flight f;
    f.id = flight_id;

    // Parse airline
    f.airline = parse_airline(flight_data["flight"]["airline"].get<std::string>());

    // Parse departure and arrival airports using IATA codes
    f.from = parse_airport(flight_data["search_parameters"]["departure_iota"].get<std::string>());
    f.to = parse_airport(flight_data["search_parameters"]["destination_iota"].get<std::string>());

    // Parse departure and arrival times (in Unix epoch format)
    f.depart_ts = flight_data["flight"]["unix_departure_time"].get<int64_t>();
    f.arrive_ts = flight_data["flight"]["unix_arrival_time"].get<int64_t>();

    // Parse human-readable departure and arrival times
    f.depart_time = flight_data["flight"]["departure_time"].get<std::string>();
    f.arrive_time = flight_data["flight"]["arrival_time"].get<std::string>();

    // Parse stops and number of stops
    f.stops = flight_data["flight"]["stops"].get<std::string>();
    f.num_stops = (f.stops == "Nonstop") ? 0 : 1;

    // Parse cabin class
    f.cabin = parse_cabin(flight_data["search_parameters"]["travel_class"].get<std::string>());

    // Parse price (remove dollar sign and convert to integer)
    std::string price_str = flight_data["flight"]["price"].get<std::string>();
    if (price_str.size() > 1 && price_str[0] == '$')
    {
        // Create a copy of the string excluding the '$'
        std::string clean_str = price_str.substr(1);

        // Remove commas from the string
        clean_str.erase(std::remove(clean_str.begin(), clean_str.end(), ','), clean_str.end());

        // Convert the cleaned-up string to an integer
        int price = std::stoi(clean_str);

        f.price = price;
    }

    return f;
}

// TODO: Add filtering based on constraints
// Function to parse all flights from JSON files in a directory
std::vector<flight> parse_flights_from_directory(const std::string &dir_path, const flight_constraints constraints)
{
    std::vector<flight> flights;
    size_t flight_id = 0;

    // Iterate over all files in the directory
    for (const auto &entry : std::filesystem::directory_iterator(dir_path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".json")
        {
            std::ifstream file(entry.path());
            json root = json::parse(file);

            // Parse each flight in the flights_data array
            for (const auto &flight_data : root["flights_data"])
            {
                flights.push_back(parse_flight(flight_data, flight_id++));
            }
        }
    }

    return flights;
}
