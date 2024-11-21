#include "finder.h"

int main(int argc, char** argv) {
    cxxopts::Options options("finder", "Find different flight routes");

    options.add_options()
        ("o,origin",  "Origin airport code",                   cxxopts::value<std::string>())
        ("d,dest",    "Destination airport code",              cxxopts::value<std::string>())
        ("b,begin",   "Earliest departure time, default: any", cxxopts::value<std::string>()) // TODO: enforce format
        ("e,end",     "Latest arrival time, default: any",     cxxopts::value<std::string>()) // TODO: enforce format
        ("p,price",   "Maximum itinerary price, in USD",       cxxopts::value<uint>())
        ("s,stops",   "Maximum number of stops, default: any", cxxopts::value<uint>())
        ("a,airline", "Allowed airlines, default: all",        cxxopts::value<std::vector<std::string> >())
        // TODO: add goal to optimize for
        ("h,help",    "Print usage")
        ;

    auto result = options.parse(argc, argv);

    std::cout << "Hello, world!" << std::endl;

    return 0;
}