/**
 * @defgroup SUN Sun
 * @brief Handle the Sun Spectrum
 *
 *  Observed solar spectrum comes from:
 *  ftp://ftp.stsci.edu/cdbs/current_calspec/sun_reference_stis_001.fits
 *
 *  and theoretical spectrum comes from:
 *  ftp://ftp.stsci.edu/cdbs/grid/k93models/standards/sun_kurucz93.fits
 *
 *  The theoretical spectrum is scaled to match the observed spectrum from 1.5 -
 *  2.5 microns, and then it is used where the observed spectrum ends.
 *  The theoretical model of the Sun from Kurucz'93 atlas using the following
 *  parameters when the Sun is at 1 au.
 *
 *  log_Z  |  T_eff  |  log_g |  V_{Johnson}
 * ------- | ------- | ------ | --------------
 *  +0.0   |  5777 K |  +4.44 |   -26.75 mag
 */
#pragma once
#include "rquantities.hpp"
#include <cmath>
#include <exception>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <xtensor/xadapt.hpp>
#include <xtensor/xarray.hpp>
#include <cphot/hardcoded_data/sun_data.hpp>



namespace cphot {

using DMatrix = xt::xarray<double, xt::layout_type::row_major>;

/**
 * @ingroup SUN
 * @brief Class that handles the Sun's spectrum and references.
 *
 */
class Sun{
    private:
        DMatrix wavelength_nm;       ///< Wavelength in nm
        DMatrix flux_flam;           ///< flux in flam
        QLength distance;            ///< distance to the sun
        double distance_conversion;  ///< conversion factor for distance

    public:
        Sun(const QLength & distance=1 * au,
            const std::string & flavor="theoretical");

        DMatrix get_wavelength();
        DMatrix get_wavelength(const QLength& in);
        DMatrix get_flux();
        DMatrix get_flux(const QSpectralFluxDensity& in);

};

/**
 * @brief Construct a new Sun object
 *
 * @param distance to the Sun (default 1 au)
 * @param flavor  theoretical or observed (default theoretical)
 */
Sun::Sun(const QLength & distance,
         const std::string & flavor){

    this->distance = distance;
    if (flavor.compare("theoretical") == 0){
        // read the theoretical spectrum
        this->wavelength_nm = xt::adapt(cphot_sun_theoretical::wavelength) * cphot_sun_theoretical::wavelength_unit.to(nm);
        this->flux_flam = xt::adapt(cphot_sun_theoretical::flux) * cphot_sun_theoretical::flux_unit.to(flam);
        // distance_conversion = (default distance / distance) ** 2
        this->distance_conversion = std::pow((cphot_sun_theoretical::distance/distance).value, 2);
    } else {
        // read the theoretical spectrum
        this->wavelength_nm = xt::adapt(cphot_sun_observed::wavelength) * cphot_sun_observed::wavelength_unit.to(nm);
        this->flux_flam = xt::adapt(cphot_sun_observed::flux) * cphot_sun_observed::flux_unit.to(flam);
        this->distance_conversion = std::pow((cphot_sun_theoretical::distance/distance).value, 2);
    }
}

/**
 * @brief Get the internal wavelength in nm
 *
 * @return Sun wavelength in nm
 */
DMatrix Sun::get_wavelength() {
    return this->wavelength_nm;
}

/**
 * @brief Get the internal wavelength in requested units
 *
 * @param in  requested units for the wavelength
 * @return Sun wavelength units of in
 */
DMatrix Sun::get_wavelength(const QLength& in) {
    return this->wavelength_nm * nm.to(in);
}

/**
 * @brief Get the internal flux in flam
 *
 * @return Sun flux in flam
 */
DMatrix Sun::get_flux() {
    return this->flux_flam * this->distance_conversion;
}

/**
 * @brief Get the internal flux in requested units
 *
 * @param in  requested units for the flux
 * @return Sun flux units of in
 */
DMatrix Sun::get_flux(const QSpectralFluxDensity& in) {
    return this->flux_flam * flam.to(in) * this->distance_conversion;;
}


} // namespace    cphot
