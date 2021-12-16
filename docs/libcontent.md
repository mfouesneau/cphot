# Using pyphot filter library

This page shows the content of the [pyphot](https://mfouesneau.github.io/pyphot/) library with the respective
properties of the passband filters. The code to generate the table is also
provided in `hdf5test.cpp`.

You can download the pyphot library using `cphot::download_pyphot_hdf5library` and use the `cphot::HDF5Library` interface.

A short example below shows how to use that library.
```cpp
#include <cphot/io.hpp>
#include <cphot/library.hpp>

// where to save the file
std::string filename = "pyphot_library.hdf5";
cphot::download_pyphot_hdf5library(filename);
// use the library (like in pyphot)
cphot::HDF5Library lib = cphot::HDF5Library(filename);
std::cout << lib << "\n";
auto f = lib.load_filter("HST_WFC3_F160W");
f.info();
```

## Library content

* `name`:  `cphot::Filter.get_name()`
* `detector type`: energy or photon counter `cphot::Filter.is_photon_type()`
* `wavelength units`:  filter are internally defined in `nm`
* `central wavelength`: `cphot::Filter.get_cl()`
* `pivot wavelength`: `cphot::Filter.get_lpivot()`
* `effective wavelength`: `cphot::Filter.get_leff()`

Zeropoints are defined for Vega, AB, and ST systems, while also accounting for the detector type.
* `<X> mag`: magnitude in the `X` system; `cphot::Filter.get_Vega_zero_mag()`, `cphot::Filter.get_AB_zero_mag()`, `cphot::Filter.get_ST_zero_mag()`
* `<X> flux`: flux in \f$flam = erg/s/cm^2/\unicode{x212B}\f$ in the `X` system; `cphot::Filter.get_Vega_zero_flux()`, `cphot::Filter.get_AB_zero_flux()`, `cphot::Filter.get_ST_zero_flux()`
* `<X> Jy`: flux in Jansky (\f$Jy\f$) in the `X` system; `cphot::Filter.get_Vega_zero_Jy()`, `cphot::Filter.get_AB_zero_Jy()`, `cphot::Filter.get_ST_zero_Jy()`

@htmlonly
<div id="filterTable">Pyphot Filter Library</div>
<script src="https://d3js.org/d3.v3.min.js"></script>
<script type="text/javascript"charset="utf-8">
    d3.text("filters_properties.csv", function(data) {
        var parsedCSV = d3.csv.parseRows(data);

        var container = d3.select("#filterTable")
            .append("table")

            .selectAll("tr")
                .data(parsedCSV).enter()
                .append("tr")

            .selectAll("td")
                .data(function(d) { return d; }).enter()
                .append("td")
                .text(function(d) { return d; });
    });
</script>
@endhtmlonly
