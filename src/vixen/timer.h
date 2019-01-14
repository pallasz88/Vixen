#pragma once

#include <boost/chrono.hpp>
#include <variant>
#include <string>
#include <iostream>

template<class T>
struct Timer
{
    boost::chrono::high_resolution_clock::time_point start;

    std::string name;

    explicit Timer(std::string &&name) : start(boost::chrono::high_resolution_clock::now()),
                                         name(std::move(name))
    {
    }

    ~Timer()
    {
        namespace btc = boost::chrono;
        auto end = btc::high_resolution_clock::now();

        T duration = btc::duration_cast<T>(end - start);
        std::cout << "Execution of " << name << " took " << duration.count();
        std::is_same_v<T, btc::milliseconds> ? std::cout << " ms." << std::endl :
        std::is_same_v<T, btc::microseconds> ? std::cout << " us." << std::endl :
        std::cout << " ns." << std::endl;
    }
};
