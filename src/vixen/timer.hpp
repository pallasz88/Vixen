#ifndef VIXEN_TIME_HPP_INCLUDED
#define VIXEN_TIME_HPP_INCLUDED

#include <chrono>
#include <iostream>
#include <variant>

template <class T> struct Timer
{
    std::chrono::high_resolution_clock::time_point start;

    std::string name;

    explicit Timer(std::string &&nm) : start(std::chrono::high_resolution_clock::now()), name(std::move(nm))
    {
    }

    ~Timer()
    {
        namespace stc = std::chrono;
        auto end = stc::high_resolution_clock::now();

        T duration = stc::duration_cast<T>(end - start);
        std::cout << "Execution of " << name << " took " << duration.count();
        std::is_same_v<T, stc::milliseconds> ? std::cout << " ms." << std::endl
                                             : std::is_same_v<T, stc::microseconds> ? std::cout << " us." << std::endl
                                                                                    : std::cout << " ns." << std::endl;
    }

    Timer(const Timer &) = delete;

    Timer &operator=(const Timer &) = delete;
};

#endif // VIXEN_TIME_HPP_INCLUDED