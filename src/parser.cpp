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
flight parse_flight(const json &flight_data)
{
    flight f;

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

// Function to parse all flights from JSON files in a directory
std::vector<flight> parse_flights_from_directory(const std::string &dir_path, const flight_constraints constraints)
{
    std::vector<flight> flights;
    size_t flight_id = 0;
    size_t div_id = 0;
    uint div_n = constraints.div_n.value_or(1);
    size_t total_flights = 0;
    size_t removed_flights = 0;
    size_t included_flights = 0;
    size_t valid_flights = 0;

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
                flight f = parse_flight(flight_data);
                f.id = flight_id;
                total_flights++;

                bool should_add_flight = true;

                // Check airlines
                if (constraints.airlines.has_value())
                {
                    const auto &valid_airlines = constraints.airlines.value();
                    if (std::find(valid_airlines.begin(), valid_airlines.end(), f.airline) == valid_airlines.end())
                    {
                        should_add_flight = false;
                    }
                }

                // Check if the flight matches the cabin constraint
                if (constraints.cabin.has_value() && f.cabin != constraints.cabin.value())
                {
                    should_add_flight = false;
                }

                // Check if the flight matches the start timestamp constraint
                if (constraints.start_ts.has_value() && f.depart_ts < constraints.start_ts.value())
                {
                    should_add_flight = false;
                }

                // Check if the flight matches the end timestamp constraint
                if (constraints.end_ts.has_value() && f.arrive_ts > constraints.end_ts.value())
                {
                    should_add_flight = false;
                }

                if (should_add_flight)
                {
                    valid_flights++;

                    // Check div_n constraint only if we should add the flight
                    if (div_n <= 1 || div_id % div_n == 0)
                    {
                        flights.push_back(f);
                        flight_id++; // Only incr after adding

                        included_flights++;
                    }
                    else
                        removed_flights++;

                    div_id++;
                }
                else
                    removed_flights++;
            }
        }
    }

    // Print flight parse metadata
    std::cout << "Total flights: " << total_flights << std::endl;
    std::cout << "Flights included: " << included_flights << std::endl;
    std::cout << "Flights removed: " << removed_flights << std::endl;
    if (total_flights > 0)
    {
        double ratio_included = static_cast<double>(included_flights) / total_flights;
        std::cout << "Ratio of included flights: " << ratio_included << std::endl;

        double ratio_valid = static_cast<double>(valid_flights) / total_flights;
        std::cout << "Ratio of valid flights: " << ratio_valid << std::endl;

        double ratio_included_valid = static_cast<double>(included_flights) / valid_flights;
        std::cout << "Ratio of included to valid (should correspond to div_n): " << ratio_included_valid << std::endl;
    }
    else
    {
        std::cout << "No flights found in the directory." << std::endl;
    }

    return flights;
}
