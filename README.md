# cphot -- A C++ tool for computing photometry from spectra

This project is a portage from [pyphot](http://mfouesneau.github.io/pyphot/), a
Python tool for computing photometry from spectra.

This package is a header-only library to compute synthetic photometry in a
simple way, ideal to integrate into larger projects.

Complete documentation at: http://mfouesneau.github.io/cphot/

The inputs are photonic or energetic response functions for the desired
photometric bands and stellar spectra. The modules are flexible to handle (some) units
in the wavelength and flux definitions through a simplified code.

Filters are represented individually by a `cphot::Filter` object.

One can use a collection of filters handled with a `cphot::HDF5Library`.
For example, the internal library of [pyphot](http://mfouesneau.github.io/pyphot/) contains a significant amount of standard filters.
We also provide a single function interface to the [SVO Filter Profile Service](http://svo2.cab.inta-csic.es/theory/fps/) `cphot::download_svo_filter`

Each filter is minimally defined by a `wavelength` and `throughput`. Many
properties such as central or pivot wavelength are computed internally. Zero
points in multiple units are also accessible (AB, Vega magnitude, Jy,
erg/s/cm2/AA). The default detector type is assumed to be photonic, but
energetic detectors are also handled transparently for the computations.

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/mfouesneau/cphot)

What's new?
-----------

* [Dec 15, 2021] Added `cphot::download_pyphot_hdf5library` for convenience.
* [Dec 14, 2021] Added `cphot::HDF5Library` interface and revised documentation.
* [Dec 10, 2021] First portage of the pyphot library to C++. (no internal library)

Contributors
------------

Author:

- Morgan Fouesneau