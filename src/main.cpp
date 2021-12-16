/**
 * @file main.cpp
 * @brief Main example file
 * @version 0.1
 * @date 2021-11-23
 *
 */
/*
#include "rquantities.hpp"
#include "rapidcsv.hpp"
#include "blackbody.hpp"
#include "helpers.hpp"
#include "prettyprint.hpp"
*/
#include <cpr/cpr.h>
#include "cphot/votable.hpp"
#include "cphot/filter.hpp"
#include "cphot/io.hpp"


std::string download_svo_filter(std::string id){
    cpr::Response r = cpr::Get(cpr::Url{"http://svo2.cab.inta-csic.es/theory/fps/getdata.php?format=ascii&id=Generic/Bessell_JHKLM.J"});
                               // cpr::Parameters{{"format", "ascii"}, {"id", "Generic/Bessell_JHKLM.J"}});
    std::cout << "\n URL: " << r.url << std::endl;
    std::cout << "\n Content: \n" << r.text << std::endl;
    return r.text;
}


int main() {

/*
rapidcsv::Document doc("data/blackbody-stars-clean.csv");

std::vector<std::string> ignore = {
    "SDSSName", "R.A.(J2000)", "Decl.(J2000)", "Teff",
    "Teff_error", "amp", "amp_error", "theta", "theta_error",
    "chi^2/dof", "source_id",
    };

std::vector<std::string> columns;
for (const auto& p: doc.GetColumnNames()){
    if (! contains(ignore, p)){ columns.push_back(p); }
}
std::cout << columns << "\n";

std::vector<float> col = doc.GetColumn<float>("GALEX_FUV");
std::cout << col << "\n";
//std::cout << doc.GetColumnNames() << "\n";
*/

// cphot::Filter Bessell_J = cphot::download_svo_filter("Generic/Bessell_JHKLM.J");
// Bessell_J.info();

cphot::Filter gaia_G = cphot::get_filter("data/passbands/GAIA.GAIA3.G.xml");

std::cout << gaia_G;

gaia_G.info();

std::cout << "done.\n";
return 0;

}
