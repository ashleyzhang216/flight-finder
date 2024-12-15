import json
import os
from collections import defaultdict

def process_flight_data(input_dir, output_dir):
    arrivals_by_airport = defaultdict(list)

    # Counters for the total number of flights
    total_flights_read = 0
    total_flights_written = 0

    # Validate input directory
    if not os.path.exists(input_dir) or not os.path.isdir(input_dir):
        print(f"Error: Input directory '{input_dir}' does not exist or is not a directory.")
        return

    # Get a list of all JSON files in the input directory
    input_files = [os.path.join(input_dir, f) for f in os.listdir(input_dir) if f.endswith('.json')]
    print(f"Found {len(input_files)} files in {input_dir}.")

    # Read each input file and process its data
    for file in input_files:
        try:
            with open(file, 'r') as f:
                data = json.load(f)

            if "flights_data" not in data:
                print(f"Warning: 'flights_data' key not found in file {file}. Skipping...")
                continue

            for flight_entry in data["flights_data"]:
                metadata = flight_entry.get("metadata", {})
                search_parameters = flight_entry.get("search_parameters", {})
                destination_airport_iota = search_parameters.get("destination_iota")

                if not destination_airport_iota:
                    print(f"Warning: Missing 'destination_iota' in file {file}. Skipping entry...")
                    continue

                for flight in flight_entry.get("flights", []):
                    total_flights_read += 1

                    # Consolidate flight data
                    flight_data = {
                        "metadata": metadata,
                        "search_parameters": search_parameters,
                        "flight": flight
                    }

                    # Append to the corresponding arrival airport
                    arrivals_by_airport[destination_airport_iota].append(flight_data)

        except json.JSONDecodeError:
            print(f"Error: Failed to parse JSON in file {file}. Skipping...")
        except Exception as e:
            print(f"Error processing file {file}: {e}")

    # Remove duplicate flights for each airport
    for airport_iota in arrivals_by_airport:
        unique_flights = []
        seen = set()
        for flight_data in arrivals_by_airport[airport_iota]:
            # Serialize the flight data as a tuple for hashing
            flight_tuple = (
                json.dumps(flight_data["metadata"], sort_keys=True),
                json.dumps(flight_data["search_parameters"], sort_keys=True),
                json.dumps(flight_data["flight"], sort_keys=True),
            )
            if flight_tuple not in seen:
                seen.add(flight_tuple)
                unique_flights.append(flight_data)
        
        arrivals_by_airport[airport_iota] = unique_flights  # Update with unique flights
        total_flights_written += len(unique_flights)

    # Save data for each airport
    os.makedirs(output_dir, exist_ok=True)
    for airport_iota, flights in arrivals_by_airport.items():
        # Prepare output JSON structure
        output_data = {"flights_data": flights}

        # Write to output JSON file
        output_file = os.path.join(output_dir, f"{airport_iota}_arrival.json")
        with open(output_file, 'w') as f:
            json.dump(output_data, f, indent=4)

    # Print the flight counts
    print(f"Total flights read: {total_flights_read}")
    print(f"Total flights written: {total_flights_written}")
    if total_flights_read == total_flights_written:
        print("All flights successfully transferred.")
    else:
        print(f"Processed {total_flights_written} unique flights out of {total_flights_read} read flights.")

# Example usage:
input_dir = "flight_departure_results"
output_dir = "flight_arrival_results"

process_flight_data(input_dir, output_dir)
