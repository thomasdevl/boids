//
// Created by Thomas Devlamminck on 20/11/2025.
//
#ifndef ANT_WAR_DIRECT_RANDOMRANGE_H
#define ANT_WAR_DIRECT_RANDOMRANGE_H

#include <iostream>
#include <random>
#include <type_traits>

template<typename T>
class RandomRange {
    std::random_device rd;
    std::default_random_engine eng;

    typedef typename std::conditional<
        std::is_integral<T>::value,
        std::uniform_int_distribution<T>,
        std::uniform_real_distribution<T>
    >::type Dist;

    Dist dist;
    T min;
    T max;

public:
    RandomRange(T min, T max)
        : eng(rd()), min(min), max(max), dist(min, max) {}

    T get() {
        try {
            return dist(eng);
        } catch (const std::exception &e) {
            std::cerr << "RandomRange exception: " << e.what() << std::endl;
            throw;
        }
    }
};

#endif // ANT_WAR_DIRECT_RANDOMRANGE_H
