/**
 * @brief Lick indices definitions
 *
 * A Lick index is defined by 3 wavelength intervals:
 * - an index interval, which defines the measurement region,
 * - a blue and a red wavelength intervals, which define the continuum reference values (by interpolation).
 *
 *
 * This list was compiled from private communication with Ariane Lancon (AL),
 * Scott Trager (SCT), and Damien Leborgne (DL).
 * The CaH_1, CaH_2, and TiO_3 are based on Gizis (1997) with modifications by SCT.
 * TiO_4 also (mostly?) contains K
 *
 * \note taken from pyphot
 *
 */
#pragma once
#include <string>
#include <cphot/rquantities.hpp>

namespace cphot_licks {

/**
 * @brief Stores the definition of a Lick index
 */
typedef struct {
    std::string name;             ///< name of the index
    double index_band_min;        ///< minimal wavelength of index interval
    double index_band_max;        ///< maximal wavelength of index interval
    double blue_continuum_min;    ///< minimal wavelength of blue continuum
    double blue_continuum_max;    ///< maximal wavelength of blue continuum
    double red_continuum_min;     ///< minimal wavelength of red continuum
    double red_continuum_max;     ///< maximal wavelength of red continuum
    QLength wavelength_unit;      ///< wavelength unit (usually Angstrom or nm)
    bool is_mag;                  ///< true if the index is in magnitudes (else equivalent width)
    // std::string description;   ///< description/notes
} lickdata;

static const std::vector<lickdata> lickdefs({
//Index                    band          blue continuum         red continuum       Units   Mag?
{"CN_1"           , 4142.125, 4177.125,  4080.125, 4117.625,  4244.125, 4284.125, angstrom, true},
{"CN_2"           , 4142.125, 4177.125,  4083.875, 4096.375,  4244.125, 4284.125, angstrom, true},
{"Ca4227"         , 4222.250, 4234.750,  4211.000, 4219.750,  4241.000, 4251.000, angstrom, false},
{"G4300"          , 4281.375, 4316.375,  4266.375, 4282.625,  4318.875, 4335.125, angstrom, false},
{"Fe4383"         , 4369.125, 4420.375,  4359.125, 4370.375,  4442.875, 4455.375, angstrom, false},
{"Ca4455"         , 4452.125, 4474.625,  4445.875, 4454.625,  4477.125, 4492.125, angstrom, false},
{"Fe4531"         , 4514.250, 4559.250,  4504.250, 4514.250,  4560.500, 4579.250, angstrom, false},
{"Fe4668"         , 4634.000, 4720.250,  4611.500, 4630.250,  4742.750, 4756.500, angstrom, false},
{"H_beta"         , 4847.875, 4876.625,  4827.875, 4847.875,  4876.625, 4891.625, angstrom, false},
{"Fe5015"         , 4977.750, 5054.000,  4946.500, 4977.750,  5054.000, 5065.250, angstrom, false},
{"Mg_1"           , 5069.125, 5134.125,  4895.125, 4957.625,  5301.125, 5366.125, angstrom, true},
{"Mg_2"           , 5154.125, 5196.625,  4895.125, 4957.625,  5301.125, 5366.125, angstrom, true},
{"Mg_b"           , 5160.125, 5192.625,  5142.625, 5161.375,  5191.375, 5206.375, angstrom, false},
{"Fe5270"         , 5245.650, 5285.650,  5233.150, 5248.150,  5285.650, 5318.150, angstrom, false},
{"Fe5335"         , 5312.125, 5352.125,  5304.625, 5315.875,  5353.375, 5363.375, angstrom, false},
{"Fe5406"         , 5387.500, 5415.000,  5376.250, 5387.500,  5415.000, 5425.000, angstrom, false},
{"Fe5709"         , 5696.625, 5720.375,  5672.875, 5696.625,  5722.875, 5736.625, angstrom, false},
{"Fe5782"         , 5776.625, 5796.625,  5765.375, 5775.375,  5797.875, 5811.625, angstrom, false},
{"Na_D"           , 5876.875, 5909.375,  5860.625, 5875.625,  5922.125, 5948.125, angstrom, false},
{"TiO_1"          , 5936.625, 5994.125,  5816.625, 5849.125,  6038.625, 6103.625, angstrom, true},
{"TiO_2"          , 6189.625, 6272.125,  6066.625, 6141.625,  6372.625, 6415.125, angstrom, true},
{"Hdelta_A"       , 4083.500, 4122.250,  4041.600, 4079.750,  4128.500, 4161.000, angstrom, false},
{"Hgamma_A"       , 4319.750, 4363.500,  4283.500, 4319.750,  4367.250, 4419.750, angstrom, false},
{"Hdelta_F"       , 4091.000, 4112.250,  4057.250, 4088.500,  4114.750, 4137.250, angstrom, false},
{"Hgamma_F"       , 4331.250, 4352.250,  4283.500, 4319.750,  4354.750, 4384.750, angstrom, false},
{"OIIEW"          , 3716.3  , 3738.3  ,  3696.3  , 3716.3  ,  3738.3  , 3758.3  , angstrom, false},
{"bTiO"           , 4758.500, 4800.000,  4742.750, 4756.500,  4827.875, 4847.875, angstrom, false},
{"HdeltaEW"       , 4083.5  , 4122.250,  4017.0  , 4057.0  ,  4153.0  , 4193.0  , angstrom, false},
{"NaI"            , 8163.500, 8229.125,  8140.000, 8163.500,  8230.250, 8250.000, angstrom, false},
{"HgammaEW"       , 4319.750, 4363.5  ,  4242.0  , 4282.0  ,  4404.0  , 4444.0  , angstrom, false},
{"CaH_1"          , 6357.500, 6401.750,  6342.125, 6356.500,  6408.500, 6429.750, angstrom, true},
{"HbetaEW"        , 4847.875, 4876.625,  4799.0  , 4839.0  ,  4886.0  , 4926.0  , angstrom, false},
{"CaH_2"          , 6775.000, 6900.000,  6510.000, 6539.250,  7017.000, 7064.000, angstrom, true},
{"TiO_3"          , 7123.750, 7162.500,  7017.000, 7064.000,  7234.000, 7269.000, angstrom, true},
{"TiO_4"          , 7643.250, 7717.250,  7527.000, 7577.750,  7735.500, 7782.750, angstrom, true},
{"TiOCaH"         , 6600.000, 6817.000,  6520.000, 6545.000,  7035.000, 7050.000, angstrom, false},
{"TiO3"           , 6600.000, 6723.000,  6520.000, 6545.000,  7035.000, 7050.000, angstrom, false},
{"CaH"            , 6775.000, 6817.000,  6520.000, 6545.000,  7035.000, 7050.000, angstrom, false},
{"aTiO"           , 5445.000, 5600.000,  5420.000, 5442.000,  5630.000, 5655.000, angstrom, true},
{"NaI_V12"        , 8180.000, 8200.000,  8164.000, 8173.000,  8233.000, 8244.000, angstrom, false},
{"NaI_F13"        , 8180.000, 8200.000,  8137.000, 8147.000,  8233.000, 8244.000, angstrom, false},
{"CaHK_LB13"      , 3899.47 , 4003.47 ,  3806.50 , 3833.82 ,  4020.69 , 4052.36 , angstrom, false},
{"Mg4780"         , 4760.78 , 4798.80 ,  4738.91 , 4757.31 ,  4819.78 , 4835.51 , angstrom, false},
{"TiO2SDSS_LB13"  , 6189.625, 6272.125,  6066.625, 6141.625,  6442.0  , 6455.0  , angstrom, true},
{"NaI_LB13"       , 8180.000, 8200.000,  8143.000, 8153.000,  8233.000, 8244.000, angstrom, false},
{"Ca1_LB13"       , 8484.0  , 8513.0  ,  8474.0  , 8484.0  ,  8563.0  , 8577.0  , angstrom, false},
{"Ca2_LB13"       , 8522.0  , 8562.0  ,  8474.0  , 8484.0  ,  8563.0  , 8577.0  , angstrom, false},
{"Ca3_LB13"       , 8642.0  , 8682.0  ,  8619.0  , 8642.0  ,  8700.0  , 8725.0  , angstrom, false},
{"Hbeta0"         , 4839.275, 4877.097,  4821.175, 4838.404,  4897.445, 4915.845, angstrom, false}
});

}; // namespace cphot_licks