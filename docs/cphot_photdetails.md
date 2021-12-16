# Details on predicting photometry

It is sometimes not evident that there are essential differences between photometric systems. But even less known, the difference between detector count types (energy or photons) requires special care.

This section reviews the essential details for computing the luminosity and magnitude of a star through a photometric passband filter. We do not discuss calibration, which instrument documentations cover, in principle.

\tableofcontents

Let's consider a filter throughput (a.k.a, transmission curve, or response function) defined in wavelength by the dimensionless function \f$T(\lambda)\f$. This function tells you what fraction of the arriving photons at wavelength \f$\lambda\f$ get through the instrument.  Therefore, the total number of photons, per unit time per unit area, received in this filter is

$$N_{tot} = \frac{1}{hc} \int_\lambda f_\lambda\,\lambda\,T(\lambda)\,d\lambda,$$

where \f$f_\lambda\f$ is the wavelength-dependent flux density of an object given in energy per unit time per unit area per unit wavelength.

Consequently, interpreting \f$\lambda T(\lambda)\f$ as a distribution leads to the statistical mean of the flux density, \f$\overline{f_\lambda}\f$:

$$\overline{f_\lambda}(T) = \frac{\int_\lambda \lambda f_\lambda T(\lambda) d\lambda}{\int_\lambda \lambda T(\lambda) d\lambda}.$$

Note that this is not the mean flux density because of the \f$\lambda\f$ factor in the integrals. It is the mean photon rate density in this filter commonly expressed in stellar physics literature as \f$erg.s^{-1}.cm^{-2}.Å^{-1}\f$ or \f$W.m^{-2}.nm^{-1}\f$.

Example: (the code links to the API documentation)
```cpp
#include <cphot/filter.hpp>
#include <cphot/io.hpp>           // svo webservice
#include <cphot/rquantities.hpp>  // units
cphot::Filter filt = cphot::download_svo_filter("Gaia/Gaia3.G");
// compute the mean flux density in this filter
// note the units as arguments
auto flux = filt.get_flux(lamb, spec, nm, flam);
```

Finally, at least for instruments using CCD or CCD-like cameras, i.e., counting photons, we obtain the usual definition of a magnitude

$$mag_\lambda(T) = -2.5\,\log_{10}\left(\overline{f_\lambda}\right) - ZP\left(\overline{f_\lambda}\right),$$

where \f$ZP(\overline{f_\lambda})\f$ gives the pass-band reference value (zeropoint) for a given photometric/magnitude system.

However, the zeropoints themselves depend on the adopted photometric system used to report the measurements. They may vary fundamentally from one to another.  Below we briefly describe the primary systems used in large surveys.


## Vega magnitude system

This system is defined such that the star Alpha Lyr (Vega) has magnitude 0 in any passband filter. In other words, the zeropoints are set by the magnitude of vega, \f$-2.5 \log_{10} \overline{f_\lambda}(Vega)\f$, or

$$mag_{Vega}(T) = -2.5\,\log_{10}\left(\overline{f_\lambda} / \overline{f_\lambda}(Vega)\right).$$

Example:
```cpp
#include <cphot/filter.hpp>
#include <cphot/io.hpp>           // svo webservice
#include <cphot/rquantities.hpp>  // units
#include <cmath>
cphot::Filter filt = cphot::download_svo_filter("HST/WFC3_IR.F110W");
double flux = filt.get_flux(lamb, spec, nm, flam).to(flam);
double mags_a = -2.5 * std::log10(flux) - filt.get_Vega_zero_mag();
// or similarly (note the use of the units)
double mags_b = -2.5 * std::log10(flux / filt.get_Vega_zero_flux().to(flam));
```

## Johnson system

The Johson system is defined such that the star Alpha Lyr (Vega) has
\f$V=0.03\f$ and all colors equal to zero. It is very similar to the Vega
magnitude system, but **using mean flux definition** (instead of photon counts),
i.e., **energy counter** detectors

$$\widetilde{f_\lambda}(T) = \frac{\int_\lambda f_\lambda T(\lambda) d\lambda}{\int_\lambda T(\lambda) d\lambda},$$

(the true definition of mean flux throughout a given transmission filter.)

> **Note**: Table A2 of [Bessell et al. (1998)](https://ui.adsabs.harvard.edu/abs/1998A%26A...333..231B) gives zero points for the UBVRIJHKL(+Kp and L’) filters in the
Cousins-Glass-Johnson system.

If one defines the **effective wavelength** \f$\lambda_{\rm eff}\f$ as the photon weighted mean wavelength:

$$\lambda_{\rm eff} = \frac{\int \lambda f_\lambda T(\lambda) d\lambda}{\int f_\lambda T(\lambda) d\lambda},$$

```cpp
//the effective wavelength for vega is given by
cphot::download_svo_filter("Generic/Johnson.U").get_leff();
```

then the difference between the Johnson and Vega systems within the same filter is given by

$$\widetilde{mag}_\lambda - \overline{mag}_\lambda = 0.03 - 2.5 \log_{10} \frac{\lambda_{\rm eff}(Vega)}{\lambda_{\rm eff}(star)},$$
where we explicit which equation was used to compute magnitudes.

## AB magnitude system

This system is defined such that, when monochromatic flux \f$f_\nu\f$ is measured in \f$erg\,s^{-1}\,cm^{-2} Hz^{-1}\f$,

$$mag_{AB}(T) = -2.5\, \log_{10}(\overline{f_\nu}) - 48.60,$$
where the value of the constant is selected to define \f$m_{AB}=0\f$ for a
flat-spectrum source. In this system, an object with constant flux per unit frequency interval has zero color.

Koornneef et al. gives the respective definition of \f$\overline{f_\nu}(T)\f$:

$$\overline{f_\nu}(T) = \frac{\int_\nu f_\nu T(\nu) d\nu / \nu}{\int_\nu T(\nu) d\nu / \nu}
= \frac{\int_\lambda f_\nu T(\lambda) d\lambda / \lambda}{\int_\lambda T(\lambda) d\lambda / \lambda}$$

To go back to wavelength units, we have \f$d\nu = (c/\lambda^2) d\lambda\f$.

If one defines the **pivot wavelength** \f$\lambda_p\f$ to convert between \f$\overline{f_\nu}\f$ and \f$\overline{f_\lambda}\f$ as

$$\overline{f_\nu} = \frac{\lambda_p^2}{c} \overline{f_\lambda},$$

one can easily show that

$$\lambda_p^2 = \frac{\int_\lambda T(\lambda)\,\lambda\,d\lambda}{\int_\lambda T(\lambda)\,d\lambda /\lambda}.$$

```cpp
//the pivot wavelength for a filter is given by
cphot::download_svo_filter("Generic/Johnson.U").get_lpivot();
```

Therefore for filters with AB magnitudes, one can compute

$$mag_{AB}(T) = -2.5\, \log_{10}(\overline{f_\lambda}) - 2.5\log_{10}\left(\lambda_p^2/c\right) - 48.6,$$
where one must care to use the speed of light \f$c\f$ and \f$\lambda_p\f$ in matching units.

```cpp
#include <cphot/filter.hpp>
#include <cphot/io.hpp>
#include <cphot/rquantities.hpp>
#include <cmath>
cphot::Filter filt = cphot::download_svo_filter("HST/WFC3_IR.F110W");
double flux = filt.get_flux(lamb, spec, nm, flam).to(flam);
// Simlarly to before ("Vega" -> "AB")
double mags_a = -2.5 * std::log10(flux) - filt.get_AB_zero_mag();
// or similarly (note the use of the units)
double mags_b = -2.5 * std::log10(flux / filt.get_AB_zero_flux().to(flam));
```



## ST magnitude system

This system is defined such as a source with flat \f$f_\lambda\f$ will have the same magnitude in every filter.

Koornneef et al. (1986; same as above) defines
$$mag_{ST}(T) = -2.5\, \log_{10}(\overline{f_\lambda}) - 21.1,$$

```cpp
#include <cphot/filter.hpp>
#include <cphot/io.hpp>
#include <cphot/rquantities.hpp>
#include <cmath>
cphot::Filter filt = cphot::download_svo_filter("HST/WFC3_IR.F110W");
double flux = filt.get_flux(lamb, spec, nm, flam).to(flam);
// Simlarly to before ("Vega" -> "ST")
double mags_a = -2.5 * std::log10(flux) - filt.get_ST_zero_mag();
// or similarly (note the use of the units)
double mags_b = -2.5 * std::log10(flux / filt.get_ST_zero_flux().to(flam));
```


## Jansky definition

The jansky (symbol Jy) is a non-SI unit of spectral flux density, it is equivalent to
\f$10^{−26}.W.m^{-2}.Hz^{-1}\f$ or
\f$10^{-23}.erg.s^{-1}.cm^{-2}.Å^{-1}\f$.

$${f_{Jy}} = \frac{10^5}{10^{-8}c} {\lambda_p^2} {f_\lambda},$$
where \f$c\f$ is the speed of light in \f$m/s\f$,  \f$\lambda_p\f$ is the pivot wavelength in \f$Å\f$, and \f${f_\lambda}\f$ the flux (Vega, AB, or ST) in flam (\f$erg.s^{-1}.cm^{-2}.Å^{-1}\f$).

```cpp
#include <iostream>
#include <cphot/filter.hpp>
#include <cphot/io.hpp>
cphot::Filter filt = cphot::download_svo_filter("HST/WFC3_IR.F110W");
std::cout << filt.get_AB_zero_Jy() << " "
          << filt.get_Vega_zero_Jy() << " "
          << filt.get_ST_zero_Jy() << std::endl;
```

---
## References

* Bessel, M. S. 1990, PASP, 91, 589 - _"UBVRI passbands."_
  [[1990PASP..102.1181B](https://ui.adsabs.harvard.edu/abs/1990PASP..102.1181B)];
* Bessel, M. S. 1998, A&A, 333, 231 - _"Model atmospheres broad-band colors, bolometric corrections and temperature calibrations for O - M stars."_
[[1998A%26A...333..231B](https://ui.adsabs.harvard.edu/abs/1998A%26A...333..231B)]

* Koornneef, Bohlin, Buser, Horne, Turnshek, 1986, Highlights of astronomy 7 - _"Synthetic photometry and the calibration of HST"._ [[1986HiA.....7..833K](https://ui.adsabs.harvard.edu/abs/1986HiA.....7..833K)]


* Bessel, M. S. 1983, PASP, 95, 480;

* Bessel, M. S. 1990, PASP, 102, 1181;

* Hayes, D. S., \& Latham, D. W. 1975, ApJ, 197, 593;

* Johnson, H. L. \& Morgan, W. W. 1953, ApJ, 117, 313

* Oke, J.B. 1974, ApJS, 27, 21;
