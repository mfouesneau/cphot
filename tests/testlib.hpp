/**
 * @file testlib.hpp
 * @brief Common simple test functions
 *
 */
#pragma once
#include <iostream>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <cmath>

template<typename T>
void EXPECT_NEAR(const T& T1, const T& T2, const T& T3)
{
    if (std::abs(T1 - T2) > T3) {

        const std::string msg (std::string("\nEXPECT_NEAR failed. |T1 - T2| > T3\n")
                               + "with \n"
                               + "    T1: " + std::to_string(T1) + "\n"
                               + "    T2: " + std::to_string(T2) + "\n"
                               + "    T3: " + std::to_string(T3) + "\n"
                               + " (T1-T2) / T3: " + std::to_string(std::abs(T1-T2) / T3) + "\n"
                               );
        throw std::logic_error(msg);
        }
}