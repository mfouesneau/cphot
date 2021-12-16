/**
 * @file cphot_dev.cpp
 * @brief  developping cphot
 * @version 0.1
 *
 */
#include <iostream>
#include <cphot/io.hpp>
#include <cphot/rquantities.hpp>
#include <cphot/votable.hpp>
#include <cphot/vega.hpp>
#include <cphot/sun.hpp>
#include <cphot/licks.hpp>



int main(){
    // std::string filter_id = "2MASS/2MASS.H";
    std::string filter_id = "Gaia/Gaia3.G";
    cphot::Filter filt = cphot::download_svo_filter(filter_id);
    filt.info();

    cphot::Vega v2 = cphot::Vega(
        cphot_vega::wavelength_nm,
        cphot_vega::flux_flam,
        nm, flam);

    double flux_flam_v2 = filt.get_flux(v2.get_wavelength(nm), v2.get_flux(flam), nm, flam).to(flam);
    std::cout << "Vega zero points for filter: " << filter_id << "\n"
              <<  flux_flam_v2 << " flam\n"
              << -2.5 * std::log10(flux_flam_v2) << " mag\n"
              << -2.5 * std::log10(flux_flam_v2) - filt.get_Vega_zero_mag() << " vega mag\n";

    auto s = cphot::Sun();
    std::cout << s.get_wavelength(angstrom) << " angstrom\n"
              << s.get_flux() << " flam\n";

    s = cphot::Sun(10 * parsec);
    std::cout << s.get_wavelength(angstrom) << " angstrom\n"
              << s.get_flux(flam) << " flam\n";

    cphot::LickLibrary lib;
    std::cout << lib.get_content() << "\n";

}