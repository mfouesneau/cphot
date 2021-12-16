/**
 * @file hdf5lib_test.cpp
 * @brief Testing the hdf5 library interface and the file content
 *
 * In this module, we create a HDF5Library from the pyphot library file and
 * extract the characteristics of all the filters to create a csv table.
 * The pyphot library is downloaded from the pyphot website using
 * `cphot::download_pyphot_hdf5library`.
 */
#include <cphot/filter.hpp>
#include <cphot/library.hpp>
#include <cphot/rquantities.hpp>
#include <iostream>
#include <fstream>
#include <cphot/io.hpp>


int main(){
    std::string filename = "pyphot_library.hdf5";
    cphot::download_pyphot_hdf5library(filename);

    auto lib = cphot::HDF5Library(filename);
    std::cout << lib << "\n";

    std::ofstream out("filters_properties.csv");

    out << "name" << ", "
        << "detector type" << ", "
        << "wavelength units" << ", "
        << "number of def points" << ", "
        << "central wavelength (nm)" << ", "
        << "pivot wavelength (nm)" << ", "
        << "effective wavelength (nm)" << ", "
        << "Vega mag (mag)" << ", "
        << "Vega flux (flam)" << ", "
        << "Vega flux (Jy)" << ", "
        << "AB mag (mag)" << ", "
        << "AB flux (flam)" << ", "
        << "AB flux (Jy)" << ", "
        << "ST mag (mag)" << ", "
        << "ST flux (flam)" << ", "
        << "ST flux (Jy)"
        << "\n";

    for (auto & c : lib.get_content()) {
       auto current = lib.load_filter(c);
       out << current.get_name() << ", "
           << (current.is_photon_type() ? "photon" : "energy") << ", "
           << "nm" << ", "    // coherence with pyphot table
           << current.get_wavelength().size() << ", "
           << current.get_cl().to(nm) << ", "
           << current.get_lpivot().to(nm) << ", "
           << current.get_leff().to(nm) << ", "
           << current.get_Vega_zero_mag() << ", "
           << current.get_Vega_zero_flux().to(flam) << ", "
           << current.get_Vega_zero_Jy().to(Jy) << ", "
           << current.get_AB_zero_mag() << ", "
           << current.get_AB_zero_flux().to(flam) << ", "
           << current.get_AB_zero_Jy().to(Jy) << ", "
           << current.get_ST_zero_mag() << ", "
           << current.get_ST_zero_flux().to(flam) << ", "
           << current.get_ST_zero_Jy().to(Jy)
           << "\n";

    }
    std::cout << lib.find("gaia", false) << "\n";
    out.close();

    return 0;
}