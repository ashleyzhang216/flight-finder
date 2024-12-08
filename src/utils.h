#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <exception>

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

/**
 * @brief helpers to check if struct has 'id' field of a certain type
*/
template <typename T, typename F>
struct has_id : std::false_type {};

template <typename T>
struct has_id<T, std::decay_t<decltype(std::declval<T>().id)> >
    : std::true_type {};

/**
 * @brief wrapper for std::vector
 */
template <class K, class V>
requires has_id<K, size_t>::value
struct id_vec {
    id_vec() = default;
    id_vec(std::vector<V>&& data)
        : vec(data) {
        // do nothing else
    }

    // for id placeholders
    static constexpr size_t INVALID_ID = ULONG_MAX;

    // access size
    size_t size() const {
        return vec.size();
    }

    // access vector element
    V& operator[](K n) {
        assert(n.id < size());
        return vec[n.id];
    }
    const V& operator[](K n) const {
        return const_cast<const V&>(const_cast<id_vec*>(this)->operator[](n));
    }

    // underlying data
    std::vector<V> vec;
};

#endif // UTILS_H