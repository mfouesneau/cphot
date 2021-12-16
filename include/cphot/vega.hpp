/**
 * @defgroup VEGA Vega
 * @brief Handle vega spec/mags/fluxes manipulations
 *
 * Class that handles vega spectrum and references.  This class knows where to
 * find the Vega synthetic spectrum taken from <a
 * href="https://iopscience.iop.org/article/10.1086/420715"> Bohlin 2007 </a> in
 * order to compute fluxes and magnitudes in given filters.
 *
 * \note the Vega zeropoints are defined by the magnitude of Vega being exactly zero in all filters.
 */
#pragma once
#include "rquantities.hpp"
#include "votable.hpp"
#include <xtensor/xadapt.hpp>
#include <xtensor/xarray.hpp>
#include <cphot/hardcoded_data/vega_data.hpp>

namespace cphot {

using DMatrix = xt::xarray<double, xt::layout_type::row_major>;

/**
 * @brief Interface to Vega reference data
 *
 * Class that handles vega spectrum and references.  This class knows where to
 * find the Vega synthetic spectrum (Bohlin 2007) in order to compute fluxes and
 * magnitudes in given filters
 *
 * @ingroup VEGA
 **/
class Vega {
    public:
        Vega(const DMatrix& wavelength,
             const DMatrix& flux,
             const QLength& wavelength_unit,
             const QSpectralFluxDensity& flux_unit);
        Vega(const std::vector<double>& wavelength,
             const std::vector<double>& flux,
             const QLength& wavelength_unit,
             const QSpectralFluxDensity& flux_unit);
        Vega();

        DMatrix get_wavelength();
        DMatrix get_wavelength(const QLength& in);
        DMatrix get_flux();
        DMatrix get_flux(const QSpectralFluxDensity& in);

    private:
        DMatrix wavelength_nm;    ///< Wavelength in nm
        DMatrix flux_flam;        ///< flux in flam

};

/**
 * @brief Construct a new Vega object from hardcoded data
 */
Vega::Vega() {
    std::vector<std::size_t> shape = { cphot_vega::wavelength_nm.size() };
    DMatrix xt_wave = xt::adapt(cphot_vega::wavelength_nm, shape);
    DMatrix xt_flux = xt::adapt(cphot_vega::flux_flam, shape);
    this->wavelength_nm = xt_wave;
    this->flux_flam = xt_flux;
}

/**
 * @brief Construct a new Vega object from data
 *
 * @param wavelength       wavelength array
 * @param flux             flux array of vega spectrum
 * @param wavelength_unit  wavelength unit
 * @param flux_unit        flux unit of the spectrum
 */
Vega::Vega(const DMatrix& wavelength,
           const DMatrix& flux,
           const QLength& wavelength_unit,
           const QSpectralFluxDensity& flux_unit) {
    this->wavelength_nm = wavelength * wavelength_unit.to(nm);
    this->flux_flam = flux * flux_unit.to(flam);
}

/**
 * @brief Construct a new Vega object from data
 *
 * @param wavelength       wavelength array
 * @param flux             flux array of vega spectrum
 * @param wavelength_unit  wavelength unit
 * @param flux_unit        flux unit of the spectrum
 */
Vega::Vega(const std::vector<double>& wavelength,
           const std::vector<double>& flux,
           const QLength& wavelength_unit,
           const QSpectralFluxDensity& flux_unit) {

    std::vector<std::size_t> shape = { wavelength.size() };
    DMatrix xt_wave = xt::adapt(wavelength, shape);
    DMatrix xt_flux = xt::adapt(flux, shape);
    this->wavelength_nm = xt_wave * wavelength_unit.to(nm);
    this->flux_flam = xt_flux * flux_unit.to(flam);
}

/**
 * @brief Construct a new Vega object from a votable
 *
 * @param infile    votable containing the vega spectrum
 * @return Vega object
 *
 * \ingroup VEGA
 */
Vega vega_from_votable(const std::string& infile){

    votable::VOTable vega(infile);
    const auto & wave = vega.get<double>("WAVELENGTH");
    const auto & flux = vega.get<double>("FLUX");
    const QLength wavelength_unit = units::parse_length(wave.unit);
    const QSpectralFluxDensity flux_unit = units::parse_spectralflux(flux.unit);

    Vega v(wave.data, flux.data, wavelength_unit, flux_unit);
    return v;
}

/**
 * @brief Get the internal wavelength in nm
 *
 * @return Vega wavelength in nm
 */
DMatrix Vega::get_wavelength() {
    return this->wavelength_nm;
}

/**
 * @brief Get the internal wavelength in requested units
 *
 * @param in  requested units for the wavelength
 * @return Vega wavelength units of in
 */
DMatrix Vega::get_wavelength(const QLength& in) {
    return this->wavelength_nm * nm.to(in);
}

/**
 * @brief Get the internal flux in flam
 *
 * @return Vega flux in flam
 */
DMatrix Vega::get_flux() {
    return this->flux_flam;
}

/**
 * @brief Get the internal flux in requested units
 *
 * @param in  requested units for the flux
 * @return Vega flux units of in
 */
DMatrix Vega::get_flux(const QSpectralFluxDensity& in) {
    return this->flux_flam * flam.to(in);
}

} // namespace cphot