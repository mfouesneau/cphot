/**
 * @defgroup RQUANTITY unit module
 * @brief This module defines usual units and convenient ways to convert between them.
 *
 * a `Quantity` is defined as a `double` with a `Unit` attached.
 * A unit combines the following 8 dimensions to some powers:
 * -# MassDim      mass
 * -# LengthDim    length
 * -# TimeDim      time
 * -# AngleDim     angular
 * -# CurrentDim   current (electric)
 * -# LumDim       luninosity
 * -# SubDim       Substance (chemical)
 * -# TempDim      temperature
 *
 * for example,
 * * a frequency is defined as `QUANTITY_TYPE(0, 0, -1, 0, 0, 0, 0, 0, QFrequency)`
 * * an energy as `QUANTITY_TYPE(1, 2, -2, 0, 0, 0, 0, 0, QEnergy)`
 *
 * Below are listed the base definitions:
 *
 * * `QUANTITY_TYPE(0, 0, 0, 0, 0, 0, 0, 0, Number)`
 * * `QUANTITY_TYPE(1, 0, 0, 0, 0, 0, 0, 0, QMass)`
 * * `QUANTITY_TYPE(0, 1, 0, 0, 0, 0, 0, 0, QLength)`
 * * `QUANTITY_TYPE(0, 2, 0, 0, 0, 0, 0, 0, QArea)`
 * * `QUANTITY_TYPE(0, 3, 0, 0, 0, 0, 0, 0, QVolume)`
 * * `QUANTITY_TYPE(0, 0, 1, 0, 0, 0, 0, 0, QTime)`
 * * `QUANTITY_TYPE(0, 1, -1, 0, 0, 0, 0, 0, QSpeed)`
 * * `QUANTITY_TYPE(0, 1, -2, 0, 0, 0, 0, 0, QAcceleration)`
 * * `QUANTITY_TYPE(0, 1, -3, 0, 0, 0, 0, 0, QJerk)`
 * * `QUANTITY_TYPE(0, 0, -1, 0, 0, 0, 0, 0, QFrequency)`
 * * `QUANTITY_TYPE(1, 1, -2, 0, 0, 0, 0, 0, QForce)`
 * * `QUANTITY_TYPE(1, -1, -2, 0, 0, 0, 0, 0, QPressure)`
 * * `QUANTITY_TYPE(0, 0, 0, 0, 1, 0, 0, 0, QCurrent)`
 * * `QUANTITY_TYPE(0, 0, 0, 0, 0, 1, 0, 0, QLuminosity)`
 * * `QUANTITY_TYPE(0, 0, 0, 0, 0, 0, 1, 0, QSubstance)`
 * * `QUANTITY_TYPE(0, 0, 0, 0, 0, 0, 0, 1, QTemperature)`
 * * `QUANTITY_TYPE(1, 2, -2, 0, 0, 0, 0, 0, QEnergy)`
 * * `QUANTITY_TYPE(1, 2, -3, 0, 0, 0, 0, 0, QFlux)`
 * * `QUANTITY_TYPE(1, -1, -3, 0, 0, 0, 0, 0, QSpectralFluxDensity)`
 * * `QUANTITY_TYPE(0, 0, 0, 1, 0, 0, 0, 0, Angle)`
 * * `QUANTITY_TYPE(0, 0, 0, 2, 0, 0, 0, 0, AngleArea)`
 *
 * We predefined units as `constexpr` which are optimized at compilation time.
 *
 * \note All units only apply to double values. (eventually will extend to vectors etc)
 */
#pragma once
#include <ratio>
#include <ostream>
#include <string.h>
#include <string_view>
#include <map>

// The class SHOULD NOT BE INSTANTIATED directly by itself, rather use the quantity types defined below.
/**
 * @ingroup RQUANTITY
 * @brief prototype template container class, that just holds a double value.
 *
 * \warning This class is not intended to be used directly, but rather as a base class for the quantity types.
 */
template<typename MassDim, typename LengthDim, typename TimeDim, typename AngleDim, typename CurrentDim, typename LumDim, typename SubDim, typename TempDim>
class RQuantity
{
public:
    double value;

public:
    constexpr RQuantity() : value(0.0) {}
    constexpr RQuantity(double val) : value(val) {}
    constexpr RQuantity(long double val) : value(static_cast<double>(val)) {}

    // The intrinsic operations for a quantity with a unit is addition and subtraction
    constexpr RQuantity const& operator+=(const RQuantity& rhs)
    {
        value += rhs.value;
        return *this;
    }
    constexpr RQuantity const& operator-=(const RQuantity& rhs)
    {
        value -= rhs.value;
        return *this;
    }

    // Returns the value of the quantity in multiples of the specified unit
    constexpr double Convert(const RQuantity& rhs) const
    {
        return value / rhs.value;
    }

    constexpr double to(const RQuantity& rhs) const {
        return value / rhs.value;
    }

    // returns the raw value of the quantity (should not be used)
    constexpr double getValue() const
    {
        return value;
    }
};



// Predefined (physical unit) quantity types:
// ------------------------------------------
#define QUANTITY_TYPE(_Mdim, _Ldim, _Tdim, _Adim, _CDim, _LumDim, _SubDim, TempDim, name) \
    typedef RQuantity<std::ratio<_Mdim>, std::ratio<_Ldim>, std::ratio<_Tdim>, std::ratio<_Adim>, std::ratio<_CDim>, std::ratio<_LumDim>, std::ratio<_SubDim>, std::ratio<TempDim>> name;

//MassDim, LengthDim, TimeDim, AngleDim, CurrentDim, LumDim, SubDim, TempDim
// Replacement of "double" type

QUANTITY_TYPE(0, 0, 0, 0, 0, 0, 0, 0, Number);

// Physical quantity types
QUANTITY_TYPE(1, 0, 0, 0, 0, 0, 0, 0, QMass);
QUANTITY_TYPE(0, 1, 0, 0, 0, 0, 0, 0, QLength);
QUANTITY_TYPE(0, 2, 0, 0, 0, 0, 0, 0, QArea);
QUANTITY_TYPE(0, 3, 0, 0, 0, 0, 0, 0, QVolume);
QUANTITY_TYPE(0, 0, 1, 0, 0, 0, 0, 0, QTime);
QUANTITY_TYPE(0, 1, -1, 0, 0, 0, 0, 0, QSpeed);
QUANTITY_TYPE(0, 1, -2, 0, 0, 0, 0, 0, QAcceleration);
QUANTITY_TYPE(0, 1, -3, 0, 0, 0, 0, 0, QJerk);
QUANTITY_TYPE(0, 0, -1, 0, 0, 0, 0, 0, QFrequency);
QUANTITY_TYPE(1, 1, -2, 0, 0, 0, 0, 0, QForce);
QUANTITY_TYPE(1, -1, -2, 0, 0, 0, 0, 0, QPressure);
QUANTITY_TYPE(0, 0, 0, 0, 1, 0, 0, 0, QCurrent);
QUANTITY_TYPE(0, 0, 0, 0, 0, 1, 0, 0, QLuminosity);
QUANTITY_TYPE(0, 0, 0, 0, 0, 0, 1, 0, QSubstance);
QUANTITY_TYPE(0, 0, 0, 0, 0, 0, 0, 1, QTemperature);
QUANTITY_TYPE(1, 2, -2, 0, 0, 0, 0, 0, QEnergy);
QUANTITY_TYPE(1, 2, -3, 0, 0, 0, 0, 0, QFlux);
QUANTITY_TYPE(1, -1, -3, 0, 0, 0, 0, 0, QSpectralFluxDensity);

// Angle type:
QUANTITY_TYPE(0, 0, 0, 1, 0, 0, 0, 0, Angle);
QUANTITY_TYPE(0, 0, 0, 2, 0, 0, 0, 0, AngleArea);


// Standard arithmetic operators:
// ------------------------------
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr RQuantity<M, l, T, A, C, L, S, D>
    operator+(const RQuantity<M, l, T, A, C, L, S, D>& lhs, const RQuantity<M, l, T, A, C, L, S, D>& rhs)
{
    return RQuantity<M, l, T, A, C, L, S, D>(lhs.getValue() + rhs.getValue());
}
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr RQuantity<M, l, T, A, C, L, S, D>
    operator-(const RQuantity<M, l, T, A, C, L, S, D>& lhs, const RQuantity<M, l, T, A, C, L, S, D>& rhs)
{
    return RQuantity<M, l, T, A, C, L, S, D>(lhs.getValue() - rhs.getValue());
}
template <typename M1, typename l1, typename T1, typename A1, typename C1, typename L1, typename S1, typename D1,
          typename M2, typename l2, typename T2, typename A2, typename C2, typename L2, typename S2, typename D2>
constexpr RQuantity<std::ratio_add<M1, M2>, std::ratio_add<l1, l2>,
                    std::ratio_add<T1, T2>, std::ratio_add<A1, A2>,
                    std::ratio_add<C1, C2>, std::ratio_add<L1, L2>,
                    std::ratio_add<S1, S2>, std::ratio_add<D1, D2>
                    >
    operator*(const RQuantity<M1, l1, T1, A1, C1, L1, S1, D1>& lhs,
              const RQuantity<M2, l2, T2, A2, C2, L2, S2, D2>& rhs)
{
    return RQuantity<std::ratio_add<M1, M2>, std::ratio_add<l1, l2>,
                     std::ratio_add<T1, T2>, std::ratio_add<A1, A2>,
                     std::ratio_add<C1, C2>, std::ratio_add<L1, L2>,
                     std::ratio_add<S1, S2>, std::ratio_add<D1, D2>>
                    (lhs.getValue()*rhs.getValue());
}
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr RQuantity<M, l, T, A, C, L, S, D>
    operator*(const double& lhs, const RQuantity<M, l, T, A, C, L, S, D>& rhs)
{
    return RQuantity<M, l, T, A, C, L, S, D>(lhs*rhs.getValue());
}
template <typename M1, typename l1, typename T1, typename A1, typename C1, typename L1, typename S1, typename D1,
          typename M2, typename l2, typename T2, typename A2, typename C2, typename L2, typename S2, typename D2>
constexpr RQuantity<std::ratio_subtract<M1, M2>, std::ratio_subtract<l1, l2>,
                    std::ratio_subtract<T1, T2>, std::ratio_subtract<A1, A2>,
                    std::ratio_subtract<C1, C2>, std::ratio_subtract<L1, L2>,
                    std::ratio_subtract<S1, S2>, std::ratio_subtract<D1, D2>>
    operator/(const RQuantity<M1, l1, T1, A1, C1, L1, S1, D1>& lhs, const RQuantity<M2, l2, T2, A2, C2, L2, S2, D2>& rhs)
{
    return RQuantity<std::ratio_subtract<M1, M2>, std::ratio_subtract<l1, l2>,
                     std::ratio_subtract<T1, T2>, std::ratio_subtract<A1, A2>,
                     std::ratio_subtract<C1, C2>, std::ratio_subtract<L1, L2>,
                     std::ratio_subtract<S1, S2>, std::ratio_subtract<D1, D2>>
                    (lhs.getValue() / rhs.getValue());
}
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr RQuantity<std::ratio_subtract<std::ratio<0>, M>,
                    std::ratio_subtract<std::ratio<0>, l>,
                    std::ratio_subtract<std::ratio<0>, T>,
                    std::ratio_subtract<std::ratio<0>, A>,
                    std::ratio_subtract<std::ratio<0>, C>,
                    std::ratio_subtract<std::ratio<0>, L>,
                    std::ratio_subtract<std::ratio<0>, S>,
                    std::ratio_subtract<std::ratio<0>, D>>
    operator/(double x, const RQuantity<M, l, T, A, C, L, S, D>& rhs)
{
    return RQuantity<std::ratio_subtract<std::ratio<0>, M>,
                    std::ratio_subtract<std::ratio<0>, l>,
                    std::ratio_subtract<std::ratio<0>, T>,
                    std::ratio_subtract<std::ratio<0>, A>,
                    std::ratio_subtract<std::ratio<0>, C>,
                    std::ratio_subtract<std::ratio<0>, L>,
                    std::ratio_subtract<std::ratio<0>, S>,
                    std::ratio_subtract<std::ratio<0>, D>>
                    (x / rhs.getValue());
}
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr RQuantity<M, l, T, A, C, L, S, D>
    operator/(const RQuantity<M, l, T, A, C, L, S, D>& rhs, double x)
{
    return RQuantity<M, l, T, A, C, L, S, D>(rhs.getValue() / x);
}


// Comparison operators for quantities:
// ------------------------------------
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr bool operator==(const RQuantity<M, l, T, A, C, L, S, D>& lhs, const RQuantity<M, l, T, A, C, L, S, D>& rhs)
{
    return (lhs.getValue() == rhs.getValue());
}
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr bool operator!=(const RQuantity<M, l, T, A, C, L, S, D>& lhs, const RQuantity<M, l, T, A, C, L, S, D>& rhs)
{
    return (lhs.getValue() != rhs.getValue());
}
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr bool operator<=(const RQuantity<M, l, T, A, C, L, S, D>& lhs, const RQuantity<M, l, T, A, C, L, S, D>& rhs)
{
    return (lhs.getValue() <= rhs.getValue());
}
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr bool operator>=(const RQuantity<M, l, T, A, C, L, S, D>& lhs, const RQuantity<M, l, T, A, C, L, S, D>& rhs)
{
    return (lhs.getValue() >= rhs.getValue());
}
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr bool operator< (const RQuantity<M, l, T, A, C, L, S, D>& lhs, const RQuantity<M, l, T, A, C, L, S, D>& rhs)
{
    return (lhs.getValue()<rhs.getValue());
}
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr bool operator> (const RQuantity<M, l, T, A, C, L, S, D>& lhs, const RQuantity<M, l, T, A, C, L, S, D>& rhs)
{
    return (lhs.getValue()>rhs.getValue());
}


// Predefined units:
// -----------------

// Predefined mass units:
constexpr QMass kg(1.0);                            // SI base unit
constexpr QMass gram = 0.001 * kg;
constexpr QMass tonne = 1000 * kg;
constexpr QMass ounce = 0.028349523125 * kg;
constexpr QMass pound = 16 * ounce;
constexpr QMass stone = 14 * pound;
constexpr QMass carat = 200 * 1e-3 * gram;

// Predefined length-derived units
constexpr QLength metre(1.0);                   // SI base unit
constexpr QLength meter = metre;
constexpr QLength decimetre = metre / 10;
constexpr QLength centimetre = metre / 100;
constexpr QLength millimetre = metre / 1000;
constexpr QLength micrometre = millimetre / 1000;
constexpr QLength nanometre = micrometre / 1000;
constexpr QLength nanometer = nanometre;
constexpr QLength nm = nanometer;
constexpr QLength angstrom = micrometre / 10000;
constexpr QLength kilometre = 1000 * metre;
constexpr QLength inch = 2.54 * centimetre;
constexpr QLength foot = 12 * inch;
constexpr QLength yard = 3 * foot;
constexpr QLength mile = 5280 * foot;

constexpr QArea kilometre2 = kilometre*kilometre;
constexpr QArea metre2 = metre*metre;
constexpr QArea decimetre2 = decimetre*decimetre;
constexpr QArea centimetre2 = centimetre*centimetre;
constexpr QArea millimetre2 = millimetre * millimetre;
constexpr QArea inch2 = inch*inch;
constexpr QArea foot2 = foot*foot;
constexpr QArea mile2 = mile*mile;

constexpr QVolume kilometre3 = kilometre2*kilometre;
constexpr QVolume metre3 = metre2*metre;
constexpr QVolume decimetre3 = decimetre2*decimetre;
constexpr QVolume litre = decimetre3;
constexpr QVolume centimetre3 = centimetre2*centimetre;
constexpr QVolume millimetre3 = millimetre2 * millimetre;
constexpr QVolume inch3 = inch2*inch;
constexpr QVolume foot3 = foot2*foot;
constexpr QVolume mile3 = mile2*mile;

// Predefined time-derived units:
constexpr QTime second(1.0);                        // SI base unit
constexpr QTime minute = 60 * second;
constexpr QTime hour = 60 * minute;
constexpr QTime day = 24 * hour;

constexpr QFrequency Hz(1.0);

// Predefined mixed units:
constexpr QAcceleration G = 9.80665 *  metre / (second*second);

constexpr QForce newton(1.0);
constexpr QForce poundforce = pound*G;
constexpr QForce kilopond = kg*G;

constexpr QPressure Pascal(1.0);
constexpr QPressure bar = 100000 * Pascal;
constexpr QPressure psi = pound*G / inch2;


// current units
constexpr QCurrent ampere(1.0);                 // SI Base

// constexpr QEnergy coulomb = ampere * second;

// energy
constexpr QEnergy joule = newton * metre;
constexpr QEnergy electron_volt = 1.60217653e-19 * joule;
constexpr QEnergy eV = electron_volt;
constexpr QEnergy thermochemical_calorie = 4.184 * joule;
constexpr QEnergy cal = thermochemical_calorie;

constexpr QFlux watt = joule / second;

// luminosity units
constexpr QLuminosity candela(1.0);         // SI Base

//substance units
constexpr QSubstance mole(1.0);             // SI Base

// temperature units
constexpr QTemperature degK(1.0);           // SI Base
constexpr QTemperature kelvin = 1. * degK;


// Astronomy units
constexpr QTime julian_year = 365.25 * day;
constexpr QLength parsec = 30856775814914 * kilometre;
constexpr QLength astronomical_unit = 149597870691 * meter;
constexpr QLength au = astronomical_unit;
constexpr QSpeed speed_of_light = 299792458 * meter / second;
constexpr QSpeed c = speed_of_light;
constexpr QLength light_year = speed_of_light * julian_year;
constexpr QLength ly = light_year;
constexpr QMass msun =  1.98892e30 * kg;
constexpr QMass Msun = msun;
constexpr QLength rsun = 6.955e8 * meter;
constexpr QLength Rsun = rsun;
constexpr QEnergy erg = 1e-7 * joule;
constexpr QFlux lsun = 3.839e26 * watt;

constexpr QSpectralFluxDensity flam = erg / second / centimetre2 / angstrom;
// constexpr QSpectralFluxDensity jansky = 1e-26 * watt / (meter * meter) / Hz;
constexpr QSpectralFluxDensity jansky = 1e-23 * erg / second/ centimetre2 / angstrom;
constexpr QSpectralFluxDensity Jy = jansky;



constexpr QLength operator"" _pc(long double x) { return static_cast<double>(x)*parsec; }
constexpr QLength operator"" _pc(unsigned long long int  x) { return static_cast<double>(x)*parsec; }

// Physical unit literals:
// -----------------------

// literals for length units
constexpr QLength operator"" _AA(long double x) { return static_cast<double>(x)*angstrom; }
constexpr QLength operator"" _nm(long double x) { return static_cast<double>(x)*nanometre; }
constexpr QLength operator"" _mm(long double x) { return static_cast<double>(x)*millimetre; }
constexpr QLength operator"" _cm(long double x) { return static_cast<double>(x)*centimetre; }
constexpr QLength operator"" _m(long double x) { return static_cast<double>(x)*metre; }
constexpr QLength operator"" _km(long double x) { return static_cast<double>(x)*kilometre; }
constexpr QLength operator"" _mi(long double x) { return static_cast<double>(x)*mile; }
constexpr QLength operator"" _yd(long double x) { return static_cast<double>(x)*yard; }
constexpr QLength operator"" _ft(long double x) { return static_cast<double>(x)*foot; }
constexpr QLength operator"" _in(long double x) { return static_cast<double>(x)*inch; }
constexpr QLength operator"" _mm(unsigned long long int x) { return static_cast<double>(x)*millimetre; }
constexpr QLength operator"" _cm(unsigned long long int  x) { return static_cast<double>(x)*centimetre; }
constexpr QLength operator"" _m(unsigned long long int  x) { return static_cast<double>(x)*metre; }
constexpr QLength operator"" _km(unsigned long long int  x) { return static_cast<double>(x)*kilometre; }
constexpr QLength operator"" _mi(unsigned long long int  x) { return static_cast<double>(x)*mile; }
constexpr QLength operator"" _yd(unsigned long long int  x) { return static_cast<double>(x)*yard; }
constexpr QLength operator"" _ft(unsigned long long int  x) { return static_cast<double>(x)*foot; }
constexpr QLength operator"" _in(unsigned long long int  x) { return static_cast<double>(x)*inch; }

// literals for speed units
constexpr QSpeed operator"" _mps(long double x) { return QSpeed(x); };
constexpr QSpeed operator"" _miph(long double x) { return static_cast<double>(x)*mile / hour; };
constexpr QSpeed operator"" _kmph(long double x) { return static_cast<double>(x)*kilometre / hour; };
constexpr QSpeed operator"" _mps(unsigned long long int x)
                                { return QSpeed(static_cast<long double>(x)); };
constexpr QSpeed operator"" _miph(unsigned long long int x)
                                 { return static_cast<double>(x)*mile / hour; };
constexpr QSpeed operator"" _kmph(unsigned long long int x)
                                 { return static_cast<double>(x)*kilometre / hour; };

// literal for frequency unit
constexpr QFrequency operator"" _Hz(long double x) { return QFrequency(x); };
constexpr QFrequency operator"" _Hz(unsigned long long int x)
                                   { return QFrequency(static_cast<long double>(x)); };

// literals for time units
constexpr QTime operator"" _s(long double x) { return QTime(x); };
constexpr QTime operator"" _min(long double x) { return static_cast<double>(x)*minute; };
constexpr QTime operator"" _h(long double x) { return static_cast<double>(x)*hour; };
constexpr QTime operator"" _day(long double x) { return static_cast<double>(x)*day; };
constexpr QTime operator"" _s(unsigned long long int x) { return QTime(static_cast<double>(x)); };
constexpr QTime operator"" _min(unsigned long long int x) { return static_cast<double>(x)*minute; };
constexpr QTime operator"" _h(unsigned long long int x) { return static_cast<double>(x)*hour; };
constexpr QTime operator"" _day(unsigned long long int x) { return static_cast<double>(x)*day; };

// literals for mass units
constexpr QMass operator"" _kg(long double x) { return QMass(x); };
constexpr QMass operator"" _g(long double x) { return static_cast<double>(x)*gram; };
constexpr QMass operator"" _t(long double x) { return static_cast<double>(x)*tonne; };
constexpr QMass operator"" _oz(long double x) { return static_cast<double>(x)*ounce; };
constexpr QMass operator"" _lb(long double x) { return static_cast<double>(x)*pound; };
constexpr QMass operator"" _st(long double x) { return static_cast<double>(x)*stone; };
constexpr QMass operator"" _kg(unsigned long long int x) { return QMass(static_cast<double>(x)); };
constexpr QMass operator"" _g(unsigned long long int x) { return static_cast<double>(x)*gram; };
constexpr QMass operator"" _t(unsigned long long int x) { return static_cast<double>(x)*tonne; };
constexpr QMass operator"" _oz(unsigned long long int x) { return static_cast<double>(x)*ounce; };
constexpr QMass operator"" _lb(unsigned long long int x) { return static_cast<double>(x)*pound; };
constexpr QMass operator"" _st(unsigned long long int x) { return static_cast<double>(x)*stone; };

// literals for acceleration units
constexpr QAcceleration operator"" _mps2(long double x) { return QAcceleration(x); };
constexpr QAcceleration operator"" _mps2(unsigned long long int x)
                                        { return QAcceleration(static_cast<double>(x)); };
constexpr QAcceleration operator"" _G(long double x) { return static_cast<double>(x)*G; };
constexpr QAcceleration operator"" _G(unsigned long long int x) { return static_cast<double>(x)*G; }

// literals for force units
constexpr QForce operator"" _N(long double x) { return QForce(x); };
constexpr QForce operator"" _N(unsigned long long int x) { return QForce(static_cast<double>(x)); };
constexpr QForce operator"" _lbf(long double x) { return static_cast<double>(x)*poundforce; };
constexpr QForce operator"" _lbf(unsigned long long int x) { return static_cast<double>(x)*poundforce; };
constexpr QForce operator"" _kp(long double x) { return static_cast<double>(x)*kilopond; };
constexpr QForce operator"" _kp(unsigned long long int x) { return static_cast<double>(x)*kilopond; };

// literals for pressure units
constexpr QPressure operator"" _Pa(long double x) { return QPressure(x); };
constexpr QPressure operator"" _Pa(unsigned long long int x)
                                  { return QPressure(static_cast<double>(x)); };
constexpr QPressure operator"" _bar(long double x) { return static_cast<double>(x)*bar; };
constexpr QPressure operator"" _bar(unsigned long long int x) { return static_cast<double>(x)*bar; };
constexpr QPressure operator"" _psi(long double x) { return static_cast<double>(x)*psi; };
constexpr QPressure operator"" _psi(unsigned long long int x) { return static_cast<double>(x)*psi; };


// Angular unit literals:
// ----------------------
constexpr long double operator"" _pi(long double x)
    { return static_cast<double>(x) * 3.1415926535897932384626433832795; }
constexpr long double operator"" _pi(unsigned long long int x)
    { return static_cast<double>(x) * 3.1415926535897932384626433832795; }

// Predefined angle units:
constexpr Angle radian(1.0);
constexpr Angle degree = static_cast<double>(2_pi / 360.0) * radian;
constexpr Angle arcminute = static_cast<double>(1./60.) * degree;
constexpr Angle arcsecond = static_cast<double>(1./60.) * arcminute;
constexpr AngleArea steradian = radian * radian;

// literals for angle units
constexpr Angle operator"" _rad(long double x) { return Angle(x); };
constexpr Angle operator"" _rad(unsigned long long int x) { return Angle(static_cast<double>(x)); };
constexpr Angle operator"" _deg(long double x) { return static_cast<double>(x)*degree; };
constexpr Angle operator"" _deg(unsigned long long int x) { return static_cast<double>(x)*degree; };
constexpr Angle operator"" _arcmin(long double x) { return Angle(x); };
constexpr Angle operator"" _arcmin(unsigned long long int x) { return Angle(static_cast<double>(x)); };
constexpr Angle operator"" _arcsecond(long double x) { return Angle(x); };
constexpr Angle operator"" _arcsecond(unsigned long long int x) { return Angle(static_cast<double>(x)); };
constexpr AngleArea operator"" _steradian(long double x) { return AngleArea(x); };
constexpr AngleArea operator"" _steradian(unsigned long long int x) { return AngleArea(static_cast<double>(x)); };


// Conversion macro, which utilizes the string literals
#define ConvertTo(_x, _y) (_x).Convert(1.0_##_y)



// Typesafe mathematical operations:
// ---------------------------------
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr RQuantity<std::ratio_divide<M, std::ratio<2>>,
                    std::ratio_divide<l, std::ratio<2>>,
                    std::ratio_divide<T, std::ratio<2>>,
                    std::ratio_divide<A, std::ratio<2>>,
                    std::ratio_divide<C, std::ratio<2>>,
                    std::ratio_divide<L, std::ratio<2>>,
                    std::ratio_divide<S, std::ratio<2>>,
                    std::ratio_divide<D, std::ratio<2>>>
    Qsqrt(const RQuantity<M, l, T, A, C, L, S, D>& num)
{
    return RQuantity<std::ratio_divide<M, std::ratio<2>>,
                     std::ratio_divide<l, std::ratio<2>>,
                     std::ratio_divide<T, std::ratio<2>>,
                     std::ratio_divide<A, std::ratio<2>>,
                     std::ratio_divide<C, std::ratio<2>>,
                     std::ratio_divide<L, std::ratio<2>>,
                     std::ratio_divide<S, std::ratio<2>>,
                     std::ratio_divide<D, std::ratio<2>>>
                    (sqrt(num.getValue()));
}
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
constexpr RQuantity<std::ratio_multiply<M, std::ratio<2>>,
                    std::ratio_multiply<l, std::ratio<2>>,
                    std::ratio_multiply<T, std::ratio<2>>,
                    std::ratio_multiply<A, std::ratio<2>>,
                    std::ratio_multiply<C, std::ratio<2>>,
                    std::ratio_multiply<L, std::ratio<2>>,
                    std::ratio_multiply<S, std::ratio<2>>,
                    std::ratio_multiply<D, std::ratio<2>>>
    Qsquare(const RQuantity<M, l, T, A, C, L, S, D>& num)
{
    return RQuantity<std::ratio_multiply<M, std::ratio<2>>,
                     std::ratio_multiply<l, std::ratio<2>>,
                     std::ratio_multiply<T, std::ratio<2>>,
                     std::ratio_multiply<A, std::ratio<2>>,
                     std::ratio_multiply<C, std::ratio<2>>,
                     std::ratio_multiply<L, std::ratio<2>>,
                     std::ratio_multiply<S, std::ratio<2>>,
                     std::ratio_multiply<D, std::ratio<2>>>
                    (pow(num.getValue(), 2));
}

// Typesafe trigonometric operations
inline double sin(const Angle &num)
{
    return sin(num.getValue());
}
inline double cos(const Angle &num)
{
    return cos(num.getValue());
}
inline double tan(const Angle &num)
{
    return tan(num.getValue());
}

/**
 * Nice representation
 */
template <typename M, typename l, typename T, typename A, typename C, typename L, typename S, typename D>
std::ostream &operator<<(std::ostream &os, RQuantity<M, l, T, A, C, L, S, D>& rhs) {
    os << rhs.getValue();
    return os;
}

std::ostream &operator<<(std::ostream &os, QLength rhs){
    os << rhs.getValue();
    return os;
}

std::ostream &operator<<(std::ostream &os, QMass rhs){
    os << rhs.getValue();
    return os;
}
std::ostream &operator<<(std::ostream &os, QLuminosity rhs){
    os << rhs.getValue();
    return os;
}
std::ostream &operator<<(std::ostream &os, QFlux rhs){
    os << rhs.getValue();
    return os;
}


/**
 * @ingroup RQuantity
 * @brief Convenient shortcuts
 *
 */
namespace units {
    /**
     * @ingroup RQuantity
     * @brief str aliases to length units
     */
    const std::map<std::string, QLength> length_mapper {
        {"Angstrom", angstrom},
        {"AA", angstrom},
        {"angstrom", angstrom},
        {"Nanometer", nm},
        {"nanometer", nm},
        {"nm", nm},
        {"meter", meter},
        {"metre", meter},
        {"m", meter},
        {"cm", centimetre},
        {"centimeter", centimetre},
        {"mm", millimetre},
        {"millimeter", millimetre},
        {"km", kilometre},
        {"kilometer", kilometre},
        {"micrometer", micrometre},
        {"micron", micrometre},
        {"um", micrometre},
        {"pc", parsec},
        {"parsec", parsec}
        };

    /**
     * @ingroup RQuantity
     * @brief str aliases to spectral flux density units
     */
    const std::map<std::string, QSpectralFluxDensity> spectralflux_mapper {
        {"flam", flam},
        {"erg/second/centimetre2/angstrom", flam},
        {"erg/second/centimetre**2/angstrom", flam},
        {"erg/second/centimetre^2/angstrom", flam},
        {"Jy", Jy},
        {"jansky", Jy},
    };

    /**
     * @ingroup RQuantity
     * @brief find length units from a string
     * @return QLength unit
     */
    QLength parse_length(const std::string& str) {
        return length_mapper.at(str);
    }

    /**
     * @ingroup RQuantity
     * @brief find length units from a string
     * @return QLength unit
     */
    QSpectralFluxDensity parse_spectralflux(const std::string& str) {
        return spectralflux_mapper.at(str);
    }
} // namespace units