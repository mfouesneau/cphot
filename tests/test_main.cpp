/**
 * @file test_main.cpp
 * @brief Unittest on Main example file
 * @version 0.1
 * @date 2021-11-23
 *
 */
#include "testlib.hpp"
#include <cmath>
#include <ratio>
#include <xtensor/xarray.hpp>
#include <xtensor/xbuilder.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>
#include "cphot/rquantities.hpp"
#include "prettyprint.hpp"
#include "blackbody.hpp"


/**
 * @brief Testing simple array creation and cout
 *
 */
void test_array(){
    int A[3] {3, 5, 7};
    std::cout << "manual cout: ";
    for (auto val : A){
        std::cout << val << " ";
    }
    std::cout << "\n";
    std::cout << "prettyprint's cout: ";
    std::cout << A << "\n";
}

void test_xtensor(){
    xt::xarray<double> arr1
    {{1.0, 2.0, 3.0},
    {2.0, 5.0, 7.0},
    {2.0, 5.0, 7.0}};

    xt::xarray<double> arr2
    {5.0, 6.0, 7.0};

    xt::xarray<double> res = xt::view(arr1, 1) + arr2;

    std::cout << res << std::endl;
}

/**
 * @brief Testing unit conversions
 */
void test_units(){
    EXPECT_NEAR((45_km).to(parsec), 1.45835e-12, 1e-15);
    EXPECT_NEAR((1. * lsun).to(watt), 3.839e+26, 1.);
}

/**
 * @brief testing blackbody function with units
 *
 */
void test_blackbody(){
    const auto val = bb_flux_function(500e-9 * metre, 1., 5000 * kelvin);
    EXPECT_NEAR(val.Convert(flam), 1.21072e+06, 1e-15);
    EXPECT_NEAR((500._nm * val).to(watt/metre2), 6.0536e+06, 1e-15);
}

int main() {
    test_array();
    test_xtensor();
    test_units();
    // std::cout << bb_flux_function(500, 1., 5000) << std::endl;
    // std::cout << bb_flux_function(500e-9 * metre, 1., 5000 * kelvin).Convert(flam) << std::endl;
    return 0;
}
