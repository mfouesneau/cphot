/**
 * @file test_cphot.cpp
 * @brief Unittest on CPHOT
 * @version 0.1
 *
 */
#include "testlib.hpp"
#include <cphot/rquantities.hpp>
#include <cphot/filter.hpp>
#include <cphot/io.hpp>

/**
 * @brief Testing unit conversions
 */
void test_units(){
    EXPECT_NEAR((45_km).to(parsec), 1.45835e-12, 1e-15);
    EXPECT_NEAR((1. * lsun).to(watt), 3.839e+26, 1.);
}

void test_svo_photon_dtype(){
    const std::string filt_name = "GAIA/GAIA3.G";
    cphot::Filter filt = cphot::download_svo_filter(filt_name);

    std::cout << "  Characteristic calculations" << std::endl;
    std::cout << "    get_norm()" << std::endl;
    EXPECT_NEAR(filt.get_norm(), 317.323944, 1e-5);
    std::cout << "    get_cl()" << std::endl;
    EXPECT_NEAR(filt.get_cl().to(nm), 639.022034, 1e-5);
    std::cout << "    get_lpivot()" << std::endl;
    EXPECT_NEAR(filt.get_lpivot().to(nm), 621.759036, 1e-5);
    std::cout << "    get_lmin()" << std::endl;
    EXPECT_NEAR(filt.get_lmin().to(nm), 330., 1e-5);
    std::cout << "    get_lmax()" << std::endl;
    EXPECT_NEAR(filt.get_lmax().to(nm), 1030., 1e-5);
    std::cout << "    get_width()" << std::endl;
    EXPECT_NEAR(filt.get_width().to(nm), 441.591713, 1e-5);
    std::cout << "    get_fwhm()" << std::endl;
    EXPECT_NEAR(filt.get_fwhm().to(nm), 456., 1e-5);

    std::cout << "  AB zero points" << std::endl;
    EXPECT_NEAR(filt.get_AB_zero_mag(), 21.376059, 1e-5);
    EXPECT_NEAR(filt.get_AB_zero_flux().to(flam), 2.81564e-09, 1e-14);
    EXPECT_NEAR(filt.get_AB_zero_Jy().to(Jy), 3630.780547, 1e-5);

    std::cout << "  ST zero points" << std::endl;
    EXPECT_NEAR(filt.get_ST_zero_mag(), 21.1, 1e-15);
    EXPECT_NEAR(filt.get_ST_zero_flux().to(flam), 3.63078e-09, 1e-14);
    EXPECT_NEAR(filt.get_ST_zero_Jy().to(Jy), 4681.914826, 1e-5);

    std::cout << "  Vega zero points" << std::endl;
    EXPECT_NEAR(filt.get_Vega_zero_mag(), 21.50233318269791, 1e-5);
    EXPECT_NEAR(filt.get_Vega_zero_flux().to(flam), 2.506494332299632e-09, 1e-14);
    EXPECT_NEAR(filt.get_Vega_zero_Jy().to(Jy), 3232.140533127787, 1e-5);

}

void test_svo_energy_dtype(){
    const std::string filt_name = "2MASS/2MASS.H";
    cphot::Filter filt = cphot::download_svo_filter(filt_name);

    std::cout << "  Characteristic calculations" << std::endl;
    std::cout << "    get_norm()" << std::endl;
    EXPECT_NEAR(filt.get_norm(), 250.940449, 1e-5);
    std::cout << "    get_cl()" << std::endl;
    EXPECT_NEAR(filt.get_cl().to(nm), 1651.36646, 1e-5);
    std::cout << "    get_lpivot()" << std::endl;
    EXPECT_NEAR(filt.get_lpivot().to(nm), 1645.750389, 1e-5);
    std::cout << "    get_lmin()" << std::endl;
    EXPECT_NEAR(filt.get_lmin().to(nm), 1486., 1e-5);
    std::cout << "    get_lmax()" << std::endl;
    EXPECT_NEAR(filt.get_lmax().to(nm), 1818., 1e-5);
    std::cout << "    get_width()" << std::endl;
    EXPECT_NEAR(filt.get_width().to(nm), 250.940449, 1e-5);
    std::cout << "    get_fwhm()" << std::endl;
    EXPECT_NEAR(filt.get_fwhm().to(nm), 270., 1e-5);


    std::cout << "  AB zero points" << std::endl;
    EXPECT_NEAR(filt.get_AB_zero_mag(), 23.489768, 1e-5);
    EXPECT_NEAR(filt.get_AB_zero_flux().to(flam), 4.01877e-10, 1e-14);
    EXPECT_NEAR(filt.get_AB_zero_Jy().to(Jy), 3630.780548, 1e-5);

    std::cout << "  ST zero points" << std::endl;
    EXPECT_NEAR(filt.get_ST_zero_mag(), 21.1, 1e-15);
    EXPECT_NEAR(filt.get_ST_zero_flux().to(flam), 3.63078e-09, 1e-14);
    EXPECT_NEAR(filt.get_ST_zero_Jy().to(Jy), 32802.521598, 1e-5);

    std::cout << "  Vega zero points" << std::endl;
    EXPECT_NEAR(filt.get_Vega_zero_mag(), 24.85379094542596, 1e-5);
    EXPECT_NEAR(filt.get_Vega_zero_flux().to(flam), 1.14415173060345e-10, 1e-14);
    EXPECT_NEAR(filt.get_Vega_zero_Jy().to(Jy), 1033.691278249937, 1e-5);
}




int main() {
    std::cout << "Testing units..." << std::endl;
    test_units();
    std::cout << "Testing SVO energy filter..." << std::endl;
    test_svo_energy_dtype();
    std::cout << "Testing SVO photon filter..." << std::endl;
    test_svo_photon_dtype();
    return 0;
}
