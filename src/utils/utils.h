#ifndef UTILS_H
#define UTILS_H

#include "../common/common_data_types.h"

using namespace common_data_types_ns;

struct assertion_failure_exception : public std::exception {
    public:
        explicit assertion_failure_exception(const std::string& msg) : message(msg) {}
        
        const char* what() const noexcept override {
            return message.c_str();
        }

    private:
        std::string message;
};

#undef assert
#define assert(...) do {                                                         \
    if (!(__VA_ARGS__)) [[unlikely]] {                                           \
        std::stringstream ss;                                                    \
        ss << "\nAssertion failed\n"                                             \
           << "File: " << __FILE__ << ": " << std::dec << __LINE__ << std::endl; \
        throw assertion_failure_exception(ss.str());                             \
    }                                                                            \
} while(0)

#undef assert_m
#define assert_m(cond, ...) do {                                                                             \
    if (!(cond)) [[unlikely]] {                                                                              \
        std::stringstream ss;                                                                                \
        ss << "\nAssertion failed\n"                                                                         \
           << "File: " << __FILE__ << ": " << std::dec << __LINE__ << ", Msg: " << __VA_ARGS__ << std::endl; \
        throw assertion_failure_exception(ss.str());                                                         \
    }                                                                                                        \
} while(0)

#endif // UTILS_H