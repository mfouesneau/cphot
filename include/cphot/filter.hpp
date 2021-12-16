/**
 * @defgroup FILTER Filter
 * @brief Handle Filter definitions and manipulation.
 *
 * Define a filter by its name, wavelength and transmission The type of
 * detector (energy or photon counter) can be specified for adapting
 * calculations. (default: photon)
 *
 * Note the usual astronomical (non SI) units of flux definitions:
 *
 * * flam     = \f$erg/s/cm^2/AA\f$
 * * fnu      = \f$erg/s/cm^2/Hz\f$
 * * photflam = \f$photon/s/cm^2/AA\f$
 * * photnu   = \f$photon/s/cm^2/Hz\f$
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
#include "vega.hpp"

/** \ingroup FILTER
 */
namespace cphot {

using DMatrix = xt::xarray<double, xt::layout_type::row_major>;

/**
 * @ingroup FILTER
 * @brief Unit Aware Filter.
 * input spectra and output values have units to avoid mis-interpretation.
 *
 * Define a filter by its name, wavelength and transmission The type of
 * detector (energy or photon counter) can be specified for adapting
 * calculations. (default: photon)
 */
class Filter {
    private:
        static constexpr double c = speed_of_light.to(angstrom / second);
        static constexpr double h = 6.62607015e-27;  // erg/s

        //! wavelength of the filter stored in nm
        DMatrix wavelength_nm;
        //! transmission of the passband
        DMatrix transmission;
        //! name of the filter
        std::string name = "";
        //! type, either photon or energy
        std::string dtype = "photon";
        //! units of the wavelength (nm by construction)
        QLength wavelength_unit = nm;
        //! Central wavelength in nm
        double cl;
        //! pivot wavelength in nm
        double lpivot;
        //! minimum wavelength in nm
        double lmin;
        //! maximum wavelength in nm
        double lmax;
        //! norm of the passband
        double norm;
        //! effective width in nm
        double width;
        //! full width at half maximum in nm
        double fwhm;
        //! Photon distribution based effective wavelength.
        double lphot = 0;
        //! Effective wavelength
        double leff = 0;
        //! Internal int λ * transmission * dλ
        double lT;

        void calculate_sed_independent_properties();

    public:
        Filter(const DMatrix& wavelength,
               const DMatrix& transmission,
               const QLength& wavelength_unit,
               const std::string dtype,
               const std::string name);
        void info();

        std::string get_name(){ return this->name;}
        double get_norm();
        QLength get_leff();
        QLength get_lphot();
        QLength get_fwhm();
        QLength get_width();
        QLength get_lmax();
        QLength get_lmin();
        QLength get_lpivot();
        QLength get_cl();

        double get_AB_zero_mag();
        QSpectralFluxDensity get_AB_zero_flux();
        QSpectralFluxDensity get_AB_zero_Jy();

        double get_ST_zero_mag();
        QSpectralFluxDensity get_ST_zero_flux();
        QSpectralFluxDensity get_ST_zero_Jy();

        double get_Vega_zero_mag();
        QSpectralFluxDensity get_Vega_zero_flux();
        QSpectralFluxDensity get_Vega_zero_Jy();

        DMatrix get_wavelength();
        DMatrix get_wavelength(const QLength& in);
        DMatrix get_transmission();

        bool is_photon_type();

        QSpectralFluxDensity get_flux(const DMatrix& wavelength,
                                      const DMatrix& flux,
                                      const QLength& wavelength_unit,
                                      const QSpectralFluxDensity& flux_unit);

        Filter reinterp(const DMatrix& new_wavelength_nm);
        Filter reinterp(const DMatrix& new_wavelength,
                        const QLength& new_wavelength_unit);
};

/**
 * @brief Construct a new Filter:: Filter object
 *
 * @param wavelength       wavelength definition
 * @param transmission     transmission on the wavelength
 * @param wavelength_unit  units of the wavelength definition
 * @param dtype            "photon" or "energy"
 * @param name             name of the passband
 * @throw std::runtime_error if detector type is invalid
 */
Filter::Filter(const DMatrix& wavelength,
               const DMatrix& transmission,
               const QLength& wavelength_unit,
               const std::string dtype,
               const std::string name){
    double convfac = wavelength_unit.to(nanometre);
    this->wavelength_nm = convfac * wavelength;
    this->wavelength_unit = nm;
    this->name = name;
    this->transmission = transmission;

    if ((dtype.compare("photon") == 0) ||
        (dtype.compare("energy") == 0)){
        this->dtype = dtype;
    } else {
        throw std::runtime_error("only photon and energy allowed");
    }
    this->calculate_sed_independent_properties();
}

/**
 * @brief Nice representation of Filter objects
 *
 * @param os   stream to output the representation
 * @param F    Filter object
 * @return std::ostream&  same as os
 */
std::ostream & operator<<(std::ostream &os,
                          Filter &F){
    os << "Filter: " << F.get_name()
       << "\n";
    return os;
}

/**
 * @brief Calculate the various standard properties of a given filter.
 *
 * These properties are e.g., fwhm, pivot wavelength.
 * Those that do not require to consider an SED such as Vega.
 */
void Filter::calculate_sed_independent_properties(){
    // Calculate Filter properties
    const auto& wavelength_nm = this->wavelength_nm;
    const auto& transmission = this->transmission;

    size_t n_points = transmission.size();
    double transmission_max = xt::amax(transmission)[0];
    double transmission_max_100th = transmission_max / 100.;

    auto norm = xt::trapz(transmission, wavelength_nm)[0];
    auto _lT = xt::trapz(wavelength_nm * transmission, wavelength_nm)[0];
    auto _cl = norm > 0 ? _lT / norm : 0.;
    this->cl = _cl;
    this->norm = norm;
    this->lT = _lT;
    double lpivot2 = 0.;
    if (this->dtype.compare("photon") == 0){
        lpivot2 = _lT / trapz(transmission / wavelength_nm, wavelength_nm)[0];
    } else {
        lpivot2 = norm / trapz(transmission / xt::square(wavelength_nm), wavelength_nm)[0];
    }
    this->lpivot = std::sqrt(lpivot2);

    // the last value with a transmission at least 1% of maximum transmission
    double lmax = wavelength_nm[0];
    // the first value with a transmission at least 1% of maximum transmission
    double lmin = wavelength_nm[n_points - 1];
    for (size_t i=0; i < transmission.size(); ++i){
        if (transmission[i] > transmission_max_100th){
            lmax = std::max(lmax, wavelength_nm[i]);
            lmin = std::min(lmin, wavelength_nm[i]);
        }
    }
    this->lmin = lmin;
    this->lmax = lmax;

    // Effective width
    // Equivalent to the horizontal size of a rectangle with height equal
    // to maximum transmission and with the same area that the one covered by
    // the filter transmission curve.
    // W = int(T dlamb) / max(T)
    this->width = (norm / xt::amax(transmission)[0]);

    // FWHM
    // the difference between the two wavelengths for which filter transmission is
    // half maximum
    //
    // ..note::
    //      This calculation is not exact but rounded to the nearest passband data
    //      points
    double first = wavelength_nm[0];
    double last = wavelength_nm[-1];
    double thresh = transmission_max * 0.5;
    for (size_t i=0; i < wavelength_nm.size() - 1; ++i){
        if((transmission[i+1] > thresh) and (transmission[i] <= thresh)){
            first = wavelength_nm[i];
            break;
        }
    }
    for (size_t i=wavelength_nm.size(); i > 1; --i){
        if((transmission[i-1] > thresh) and (transmission[i] <= thresh)){
            last = wavelength_nm[i];
            break;
        }
    }
    this->fwhm = last - first;

    // leff = int (lamb * T * Vega dlamb) / int(T * Vega dlamb)
    auto vega = Vega();
    const DMatrix& vega_wavelength = vega.get_wavelength(nm);
    const DMatrix& vega_flux = vega.get_flux(flam);
    auto vega_T = xt::interp(vega_wavelength, this->get_wavelength(nm), this->get_transmission(), 0., 0.);
    this->leff =  xt::trapz(vega_wavelength * vega_T * vega_flux, vega_wavelength)[0] /
                  xt::trapz(vega_T * vega_flux, vega_wavelength)[0];

    // lphot = int(lamb ** 2 * T * Vega dlamb) / int(lamb * T * Vega dlamb)
    this->lphot = xt::trapz(xt::square(vega_wavelength) * vega_T * vega_flux, vega_wavelength)[0] /
                  xt::trapz(vega_wavelength * vega_T * vega_flux, vega_wavelength)[0];
}

/**
 * @brief AB magnitude zero point
 *
 * \f[ mag_{AB} = -2.5 * log10(f_nu) - 48.60
 *          = -2.5 \log_{10}(f_\lambda) - 2.5 * \log_{10}(\lambda_{pivot}^2 / c) - 48.60 \f]
 *
 * @return AB magnitude zero point
 */
double Filter::get_AB_zero_mag(){
    double C1 = (this->wavelength_unit).to(angstrom);
    C1 = C1 * C1 / speed_of_light.to(angstrom / second);
    C1 = this->lpivot * this->lpivot * C1;
    return 2.5 * std::log10(C1) + 48.60;
}

/**
 * @brief  AB flux zero point
 *
 * Calculated as
 * \f[ flux_{AB} = 10^{-0.4 * (mag_{AB})} \f]
 *
 * @return AB flux zero point
 */
QSpectralFluxDensity Filter::get_AB_zero_flux(){
    return std::pow(10, -0.4 * this->get_AB_zero_mag()) * flam;
}

/**
 * @brief AB flux zero point in Jansky (Jy)
 *
 * Calculated as
 * \f[ flux_{Jy} = \frac{10^5}{10^{-8} c } \lambda_{pivot}^2 flux_{AB} \f]
 * where \f$c\f$ is speed of light in m/s, and \f$\lambda_{pivot}\f$ is the pivot wavelength in angstrom,
 * and \f$flux_{AB}\f$ is the AB flux in flam.
 *
 * @return AB flux zero point in Jansky (Jy)
 */
QSpectralFluxDensity Filter::get_AB_zero_Jy(){
        double c = 1e-8 * speed_of_light.to(meter / second);
        double f = 1e5 / c * std::pow(this->get_lpivot().to(angstrom), 2) * this->get_AB_zero_flux().to(flam);
        return f * Jy;
}

/**
 * @brief ST magnitude zero point
 *
 * Set by definition to 21.1 mag
 *
 * @return ST magnitude zero point
 */
double Filter::get_ST_zero_mag(){
    return 21.1;    // definition
}

/**
 * @brief  ST flux zero point
 *
 * Calculated as
 * \f[ flux_{ST} = 10^{-0.4 * (mag_{ST})} \f]
 *
 * @return ST flux in flam
 */
QSpectralFluxDensity Filter::get_ST_zero_flux(){
    return std::pow(10, -0.4 * this->get_ST_zero_mag()) * flam;
}

/**
 * @brief ST flux zero point in Jansky (Jy)
 *
 * Calculated as
 * \f[ flux_{Jy} = \frac{10^5}{10^{-8} c } \lambda_{pivot}^2 flux_{ST} \f]
 * where \f$c\f$ is speed of light in m/s, and \f$\lambda_{pivot}\f$ is the pivot wavelength in angstrom,
 * and \f$flux_{ST}\f$ is the ST flux in flam.
 *
 * @return ST flux in Jy
 */
QSpectralFluxDensity Filter::get_ST_zero_Jy(){
        double c = 1e-8 * speed_of_light.to(meter / second);
        double f = 1e5 / c * std::pow(this->get_lpivot().to(angstrom), 2) * this->get_ST_zero_flux().to(flam);
        return f * Jy;
}

/**
 * @brief Vega magnitude zero point
 *
 * @return Vega magnitude zero point
 */
double Filter::get_Vega_zero_mag(){
    return -2.5 * std::log10(this->get_Vega_zero_flux().to(flam));
}

/**
 * @brief  Vega flux zero point
 *
 * @return flux of Vega in flam (erg/s/cm^2/Angstrom)
 */
QSpectralFluxDensity Filter::get_Vega_zero_flux(){
    Vega v = Vega();
    double flux_flam = this->get_flux(
        v.get_wavelength(nm),
        v.get_flux(flam),
        nm, flam).to(flam);
    return flux_flam * flam;
}

/**
 * @brief Vega flux zero point in Jansky (Jy)
 *
 * Calculated as
 * \f[ flux_{Jy} = \frac{10^5}{10^{-8} c } \lambda_{pivot}^2 flux_{Vega} \f]
 * where \f$c\f$ is speed of light in m/s, and \f$\lambda_{pivot}\f$ is the pivot wavelength in angstrom,
 * and \f$flux_{Vega}\f$ is the Vega flux in flam.
 *
 * @return flux of Vega in Jy
 */
QSpectralFluxDensity Filter::get_Vega_zero_Jy(){
        double c = 1e-8 * speed_of_light.to(meter / second);
        double f = 1e5 / c * std::pow(this->get_lpivot().to(angstrom), 2) * this->get_Vega_zero_flux().to(flam);
        return f * Jy;
}

/**
 * @brief Integrate the flux within the filter and return the integrated energy/flux
 *
 * The filter is first interpolated on the spectrum wavelength definition.
 * The flux is then calculated as the integral of the flux within the filter depending on the detector type as:
 *
 * - for photon detectors:
 * \f[
 * f_\lambda = \frac{\int \lambda T(\lambda) f_\lambda d\lambda}{\int \lambda T(\lambda) d\lambda}
 * \f]
 *
 * - for energy detectors:
 * \f[
 * f_\lambda = \frac{\int T(\lambda) f_\lambda d\lambda}{\int T(\lambda) d\lambda}
 * \f]
 *
 * @param wavelength        wavelength array
 * @param flux              flux array
 * @param wavelength_unit   wavelength unit
 * @param flux_unit         flux unit
 * @return integrated flux through the filter
 *
 */
QSpectralFluxDensity Filter::get_flux(
    const DMatrix& wavelength,
    const DMatrix& flux,
    const QLength& wavelength_unit,
    const QSpectralFluxDensity& flux_unit) {
    //filter on wavelength units
    const DMatrix& filt_wave = this->get_wavelength(wavelength_unit);
    const DMatrix& filt_trans = this->get_transmission();

    // Check overlaps
    if ((xt::amin(filt_wave)[0] > xt::amax(wavelength)[0])
        || (xt::amax(filt_wave)[0] < xt::amin(wavelength)[0])) {
        return 0. * flux_unit;
    }

    // reinterpolate the transmission to the spectrum wavelength
    auto new_trans = xt::interp(wavelength, filt_wave, filt_trans, 0., 0.);

    // check transmission is not null everywhere
    if (! xt::any(new_trans > 0)){
        return 0. * flux_unit;
    }
    // Calculate the flux depending on detector type
    if (this->is_photon_type()){
        double a = xt::trapz(wavelength * new_trans * flux, wavelength)[0];
        double b = xt::trapz(wavelength * new_trans, wavelength)[0];
        return a / b * flux_unit;
    } else {
        double a = xt::trapz(new_trans * flux, wavelength)[0];
        double b = xt::trapz(new_trans, wavelength)[0];
        return a / b * flux_unit;
    }
}

/**
 * @brief New filter interpolated to match a wavelegnth definition
 *
 * @param new_wavelength_nm    wavelength definition in nm
 * @return new filter interpolated to match the new wavelength definition
 */
Filter Filter::reinterp(const DMatrix& new_wavelength_nm){
    const DMatrix& filt_wave = this->get_wavelength();
    const DMatrix& filt_trans = this->get_transmission();
    auto new_trans = xt::interp(new_wavelength_nm, filt_wave, filt_trans, 0., 0.);
    return Filter(new_wavelength_nm, new_trans, nm, this->dtype, this->name);
}

/**
 * @brief New filter interpolated to match a wavelegnth definition
 *
 * @param new_wavelength    wavelength definition
 * @param new_wavelength_unit  wavelength unit
 * @return new filter interpolated to match the new wavelength definition
 */
Filter Filter::reinterp(const DMatrix& new_wavelength, const QLength& new_wavelength_unit){
    const DMatrix& filt_wave = this->get_wavelength(new_wavelength_unit);
    const DMatrix& filt_trans = this->get_transmission();
    auto new_trans = xt::interp(new_wavelength, filt_wave, filt_trans, 0., 0.);
    return Filter(new_wavelength, new_trans, new_wavelength_unit,
                  this->dtype, this->name);
}

/**
 * @brief Display some information on cout
 */
void Filter::info(){
    size_t n_points = this->transmission.size();
    std::cout << "Filter Object information:\n"
            << "    name:                 " << this->name << "\n"
            << "    detector type:        " << this->dtype << "\n"
            << "    wavelength units:     " << "nm  (internally set)" << "\n"
            << "    central wavelength:   " << this->cl  << " nm" << "\n"
            << "    pivot wavelength:     " << this->lpivot << " nm" << "\n"
            << "    effective wavelength: " << this->leff << " nm" << "\n"
            << "    photon wavelength:    " << this->lphot << " nm" << "\n"
            << "    minimum wavelength:   " << this->lmin << " nm" << "\n"
            << "    maximum wavelength:   " << this->lmax << " nm" << "\n"
            << "    norm:                 " << this->norm << "\n"
            << "    effective width:      " << this->width << " nm" << "\n"
            << "    fullwidth half-max:   " << this->fwhm  << " nm" << "\n"
            << "    definition contains " << n_points << " points" << "\n"
            << " \n"
            << "  Zeropoints \n"
            << "      Vega: " << this->get_Vega_zero_mag() << " mag" << "\n"
            << "            " << this->get_Vega_zero_flux().to(flam) << " erg/s/cm^2/AA" << "\n"
            << "            " << this->get_Vega_zero_Jy().to(Jy) << " Jy" << "\n"
            << "        AB: " << this->get_AB_zero_mag() << " mag" << "\n"
            << "            " << this->get_AB_zero_flux().to(flam) << " erg/s/cm^2/AA" << "\n"
            << "            " << this->get_AB_zero_Jy().to(Jy) << " Jy" << "\n"
            << "        ST: " << this->get_ST_zero_mag() << " mag" << "\n"
            << "            " << this->get_ST_zero_flux().to(flam) << " erg/s/cm^2/AA" << "\n"
            << "            " << this->get_ST_zero_Jy().to(Jy) << " Jy" << "\n"
            << "\n";
}

/**
 * @brief  Central wavelength
 *
 * \f[ \lambda_{cl} = \frac{\int \lambda T(\lambda) d\lambda}{\int T(\lambda) d\lambda}\f]
 *
 * @return central wavelength in nm
 */
QLength Filter::get_cl(){ return this->cl * this->wavelength_unit;}

/**
 * @brief  Pivot wavelength in nm
 *
 * if photon detector:
 * \f[ \lambda_p^2 = \frac{\int \lambda T(\lambda) d\lambda}{\int T(\lambda) d\lambda / \lambda} \f]
 *
 * if energy:
 * \f[ \lambda_p^2 = \frac{\int T(\lambda) d\lambda}{\int T(\lambda) d\lambda / \lambda^2} \f]
 *
 * @return pivot wavelength in nm
 */
QLength Filter::get_lpivot(){ return this->lpivot * this->wavelength_unit;}

/**
 * @brief the first λ value with a transmission at least 1% of maximum transmission
 *
 * @return min wavelength in nm
 */
QLength Filter::get_lmin(){ return this->lmin * this->wavelength_unit;}

/**
 * @brief the last λ value with a transmission at least 1% of maximum transmission
 *
 * @return max wavelength in nm
 */
QLength Filter::get_lmax(){ return this->lmax * this->wavelength_unit;}

/**
 * @brief the norm of the passband
 *
 * \f[ norm = \int T(\lambda) d\lambda \f]
 *
 * @return norm
 */
double Filter::get_norm(){ return this->norm; }

/**
 * @brief  Effective width
 *
 * \f[ width = \frac{\int T(\lambda) d\lambda}{\max(T(\lambda))} \f]
 *
 * @return width in nm
 */
QLength Filter::get_width(){ return this->width * this->wavelength_unit;}

/**
 * @brief the difference between the two wavelengths for which filter
 * transmission is half maximum.
 *
 * ..note::
 *      This calculation is not exact but rounded to the nearest passband
 *      data points
 *
 * @return fwhm in nm
 */
QLength Filter::get_fwhm(){ return this->fwhm * this->wavelength_unit;}

/**
 * @brief Photon distribution based effective wavelength.
 *
 * Defined as
 * \f[ \lambda_{phot} = \frac{\int\lambda^2 T(\lambda) Vega(\lambda) d\lambda }{\int\lambda T(\lambda) Vega(\lambda) d\lambda} \f]
 *
 * @return QLength
 */
QLength Filter::get_lphot(){ return this->lphot * this->wavelength_unit;}

/**
 * @brief Effective wavelength
 *
 * \f[ \lambda_{eff} = \frac{\int \lambda T(\lambda) Vega(\lambda) d\lambda)}{\int T(\lambda) Vega(\lambda) d\lambda)} \f]
 *
 * @return Effective wavelenth
 */
QLength Filter::get_leff(){ return this->leff * this->wavelength_unit;}

/**
 * @brief Get the wavelength in nm
 *
 * @return wavelegnth in nm
 */
DMatrix Filter::get_wavelength() {
    return this->wavelength_nm;
}

/**
 * @brief Get the wavelength in requested units
 *
 * @param in  units to convert to
 * @return  wavelegnth in requested units
 */
DMatrix Filter::get_wavelength(const QLength& in) {
    return this->wavelength_nm * nm.to(in);
}

/**
 * @brief Get the transmission
 *
 * @return Transmission (unitless)
 */
DMatrix Filter::get_transmission() {
    return this->transmission;
}

/**
 * @brief Check the type of the detector
 *
 * @return true   photon
 * @return false  energy
 */
bool Filter::is_photon_type() {
    return (this->dtype.compare("photon") == 0);
}

}; // namespace cphot