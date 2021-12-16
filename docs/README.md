# Perfect Blackbody stars

> Under development - Use at your own risks

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/mfouesneau/blackbodystars)

The literature reports the discovery of stars that show spectral energy distribution very close to blackbody radiation.
We combine the photometry from Gaia DR2, SDSS, PanSTARRs... to search for any star consistent with a blackbody radiation spectrum at XXX mmag.

Preliminary inspection shows that these stars have, on average, an effective temperature compatible with being White Dwarfs (WDs) of type DB(?)

This project analyzes these objects (and more) using a blackbody model of their SEDs to derive their temperatures and distances.
We further use the Gaia's proper motions and parallaxes to derive their orbital properties.

We discuss the value of this sample of objects for the precise calibration of photometry and, in particular, the BP/RP spectra of Gaia expected with Gaia data release 3 in 2022. Regardless of the physical nature of these objects represent standard candles for the calibration of instruments.


## TODOs

- [ ] Update to (e)DR3
   - [ ] xmatch of MWDD to eDR3
   - [ ] xmatch of Suzuki2018 to eDR3


## The model

[Planck's Law](https://en.wikipedia.org/wiki/Planck%27s_law) of blackbody radiation states[^Rybicki1979] that

\f[
   {\displaystyle f_{\lambda }(T)=a\,\cdot\,{\frac {2h\nu ^{3}}{c^{2}}}{\frac {1}{e^{h\nu /kT}-1}}=a\,\cdot\,\frac{2hc^2}{\lambda^5}{\frac {1}{e^{hc /\lambda kT}-1}},}
\f]

where

* \f$f_\lambda(T)\f$ is the spectral flux density at thermal equilibrium of temperature \f$T\f$.
* \f$h\f$ is the Planck constant (\f$6.6260693\cdot10^{-34}\f$  J/s);
* \f$c\f$ is the speed of light in a vacuum (\f$299792458\cdot 10^6\mu m/s\f$);
* \f$k\f$ is the Boltzmann constant (\f$1.3806505\cdot 10^{-23}\f$ J/K);
* \f$\nu\f$ is the frequency of the electromagnetic radiation;
* \f$\lambda\f$ is the wavelength of the electromagnetic radiation;
* \f$T\f$ is the absolute temperature of the body;
* and \f$a\f$ is a dimensionless normalization factor.

Note that $a$ can alternatively links to the angular size $\theta$
$$ \theta = \sqrt{\frac{a}{\pi}} = \frac{R}{d}$$
where $R$ is the
radius, and $d$ is the distance to the star.


[^Rybicki1979]:  Rybicki, G. B.; Lightman, A. P. (1979). Radiative Processes in Astrophysics. John Wiley & Sons. ISBN 0-471-82759-2.


## Some known objects

<!-- ADS custom
[^%1H%Y]: %l (%Y), "%T", %J, %V, %p, [%R](%u)
-->

Suzuki & Fukugita (2018)[^Suzuki2018] provides us with ~20 objects with spectroscopic follow-ups.

We also compare these objects against the Montreal White Dwarf Database[^Dufour2017] ([MWDD](http://www.montrealwhitedwarfdatabase.org/tables-and-charts.html)).
MWDD contains only the information found in the Tables (and sometimes in the text) of literature publications (References on the website). Since a star can be published under many different names, each star is assigned a unique identifier via Simbad. If a star is entered in different papers under different names, the entries will be merged together in the database.

The table and the charts show the most recent value of each variable of each stars (`MWDD-GDR2.csv`)




[^Suzuki2018]: Suzuki, N., & Fukugita, M. (2018), "Blackbody Stars", The Astronomical Journal, 156, 219, [2018AJ....156..219S](https://ui.adsabs.harvard.edu/abs/2018AJ....156..219S)

[^Dufour2017]: Dufour, P., Blouin, S., Coutu, S., Fortin-Archambault, M., Thibeault, C., Bergeron, P., & Fontaine, G. (2017), "The Montreal White Dwarf Database: A Tool for the Community", 20th European White Dwarf Workshop, 509, 3, [2017ASPC..509....3D](https://ui.adsabs.harvard.edu/abs/2017ASPC..509....3D)