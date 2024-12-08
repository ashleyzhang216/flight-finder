#ifndef PARSER_H
#define PARSER_H

#include "common_data_types.h"
#include "utils.h"

std::vector<flight> parse_flights_from_directory(const std::string &dir_path, const flight_constraints constraints);

#endif // PARSER_H