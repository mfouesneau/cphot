/**
 * @file blackbody.hpp
 *
 * @brief Blackbody radiation model
 * @version 0.1
 * @date 2021-12-01
 *
 * References:
 *
 *  - Rybicki, G. B.; Lightman, A. P. (1979).
 *    _Radiative Processes in Astrophysics._ John Wiley & Sons.
 *    ISBN 0-471-82759-2.
 *
 * Planck's law of black-body radiation states that
 * \f{eqnarray*}{
 *      f_{\lambda }(T) &=& a\,\cdot\,{\frac {2h\nu ^{3}}{c^{2}}}{\frac {1}{e^{h\nu /kT}-1}}\\
 *                      &=& a\,\cdot\,\frac{2hc^2}{\lambda^5}{\frac {1}{e^{hc /\lambda kT}-1}},
 * \f}
 * where
 * - \f$f_\lambda(T)\f$ is the spectral flux density at thermal equilibrium of temperature \f$T\f$.
 * - \f$h\f$ is the Planck constant (\f$6.6260693\cdot10^{-34}\f$  J/s);
 * - \f$c\f$ is the speed of light in a vacuum (\f$299792458\cdot 10^6\mu m/s\f$);
 * - \f$k\f$ is the Boltzmann constant (\f$1.3806505\cdot 10^{-23}\f$ J/K);
 * - \f$\nu\f$ is the frequency of the electromagnetic radiation;
 * - \f$\lambda\f$ is the wavelength of the electromagnetic radiation;
 * - \f$T\f$ is the absolute temperature of the body;
 * - and \f$a\f$ is a dimensionless normalization factor.
 *
 * Note that $a$ can alternatively links to the angular size $\theta$
 * \f[
 *      \theta = \sqrt{\frac{a}{\pi}} = \frac{R}{d}
 * \f]
 * where \f$R\f$ is the radius, and \f$d\f$ is the distance to the star.
 *
 */
#pragma once
#include "cphot/rquantities.hpp"

/**
 * Blackbody as a flux distribution as function
 *  of wavelength, temperature and amplitude.
 *
 * This function deals with unit conversions
 *
 * @param lam:  wavelength
 * @param amp:  dimensionless normalization factor
 * @param teff: temperature
 * @return evaluation of the blackbody radiation in flam units (erg/s/cm2/AA)
 *
 * Note that amp is alternatively represented as the angular size θ = R/d = sqrt(amp/pi)
 */
QSpectralFluxDensity bb_flux_function(QLength lam,
                                      Number amp,
                                      QTemperature teff){
    // Natural constants.
    double kB = (1.380649e-23 * joule / kelvin).getValue();
    double c = speed_of_light.Convert(meter / second);
    double h = (6.62607015e-34 * metre2 * kg / second).getValue();
    double lam_nm = lam.Convert(nanometre);
    double teff_K = teff.Convert(kelvin);
    double amp_ = amp.getValue();

    double v = (amp_ * 2 * h * std::pow(c, 2) / (std::pow(lam_nm, 5) *
            (std::exp(h * c / (lam_nm * 1e-9 * kB * teff_K)) - 1)));

    return v * 1e+38 * flam;
}


/**
 * default units blackbody as a flux distribution as function of wavelength,
 * temperature and amplitude.
 *
 * @param lam:   wavelength in nm
 * @param amp:   dimensionless normalization factor
 * @param teff: temperature in Kelvins
 * @return evaluation of the blackbody radiation in flam units (erg/s/cm2/AA)
 *
 */
double bb_flux_function(double lam_nm, double amp, double teff_K){
    // Natural constants.
    double kB = 1.380649e-23;   // Unit("J/K")
    double c = 299792458.0;     // Unit("m/s")
    double h = 6.62607015e-34;  // Unit('m**2 * kg / s')
    double v = (amp * 2 * h * std::pow(c, 2) / (std::pow(lam_nm, 5) *
            (std::exp(h * c / (lam_nm * 1e-9 * kB * teff_K)) - 1)));
    std::cout << lam_nm << " " << amp << " " << teff_K << "\n";
    return v * 1e+38;  // flam = erg/s/cm2/AA
}