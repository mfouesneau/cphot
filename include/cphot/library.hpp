/**
 * @defgroup LIBRARY Library of filters
 * @brief Storage of filters.
 *
 * Currently we implement only the HDF5 library format.
 * This is the format used by <a link=https://github.com/mfouesneau/pyphot> pyphot </a>
 * and thus one can use its file directly.
 * (https://github.com/mfouesneau/pyphot)
 *
 */
#pragma once
#include <H5Cpp.h>
#include <cphot/filter.hpp>
#include <cphot/rquantities.hpp>
#include <highfive/H5Easy.hpp>
#include <highfive/H5File.hpp>
#include <prettyprint.hpp>
#include <xtensor/xadapt.hpp>
#include <xtensor/xarray.hpp>
#include <helpers.hpp>

namespace cphot {

    using DMatrix = xt::xarray<double, xt::layout_type::row_major>;

    /**
     * @brief Get a string attribute from a dataset
     *
     * @param ds              dataset object
     * @param attribute_name  attribute name
     * @return std::string    content of the attribute
     */
    std::string get_str_attribute(const H5::DataSet& ds,
                                const std::string & attribute_name){

        if (!ds.attrExists("DETECTOR"))
            throw std::runtime_error("Attribute " + attribute_name + " does not exist");

        std::string attr_str;
        H5::Attribute attr = ds.openAttribute(attribute_name);
        H5::StrType stype = attr.getStrType();
        attr.read(stype, attr_str);
        return attr_str;
    }

    /**
     * @brief Get a string attribute from a dataset
     *
     * @param filename        file name
     * @param path            path to the dataset
     * @param attribute_name  attribute name
     * @return std::string    content of the attribute
     */
    std::string get_str_attribute(const std::string & filename,
                                const std::string & path,
                                const std::string & attribute_name){
        H5::H5File file(filename, H5F_ACC_RDONLY);
        H5::DataSet ds = file.openDataSet(path);
        return get_str_attribute(ds, attribute_name);
    }

    /**
     * @brief Structure of the HDF5 filter record
     *
     * definition of records in HDF5 library datasets (wavelength, transmission)
     * This structure is needed to read-in the dataset as a compound data type
     *
     * this needs HIGHFIVE_REGISTER_TYPE(filter_t, create_compound_filter_t);
     */
    typedef struct {
        double wavelength;
        double transmission;
    } filter_t;

    /**
     * @brief Create a compound filter t object
     * @return HighFive::CompoundType
     *
     * this needs HIGHFIVE_REGISTER_TYPE(filter_t, create_compound_filter_t);
     */
    HighFive::CompoundType create_compound_filter_t(){
        return {{"WAVELENGTH", HighFive::AtomicType<double>{} },
                {"THROUGHPUT", HighFive::AtomicType<double>{}} };
    }

    /**
     * @brief Get the filter from hdf5 library object
     *
     * @param library_filename  where to find the hdf5 library
     * @param filter_name       filter normalized name (dataset group key)
     * @return Filter           filter object
     */
    Filter get_filter_from_hdf5_library(const std::string& library_filename,
                                        const std::string& filter_name){

        std::string dataset_path ("/filters/" + filter_name);
        H5::H5File file(library_filename, H5F_ACC_RDONLY);
        H5::DataSet ds = file.openDataSet(dataset_path);
        std::string name = get_str_attribute(ds, "NAME");
        std::string detector_type = get_str_attribute(ds, "DETECTOR");
        std::string wavelength_unit = get_str_attribute(ds, "WAVELENGTH_UNIT");
        file.close();

        H5Easy::File hf_file(library_filename, H5Easy::File::ReadOnly);
        HighFive::DataSet hf_ds = hf_file.getDataSet(dataset_path);
        std::vector<filter_t> data;
        hf_ds.read(data);

        std::vector<double> wavelength;
        std::vector<double> transmission;
        for (auto & d : data) {
            wavelength.push_back(d.wavelength);
            transmission.push_back(d.transmission);
        }

        std::vector<std::size_t> shape = { wavelength.size() };
        DMatrix xt_wave = xt::adapt(wavelength, shape);
        DMatrix xt_transmit = xt::adapt(transmission, shape);

        cphot::Filter tmp(xt_wave, xt_transmit,
                        units::parse_length(wavelength_unit),
                        detector_type,
                        name);
        return tmp;
    }


    /**
     * @ingroup LIBRARY
     * @brief  Storage of filters in HDF5 format
     *
     * This is the format used by <a link=https://github.com/mfouesneau/pyphot> pyphot </a>
     * and thus one can use its file directly.
     */
    class HDF5Library {

        private:
            std::string source;                 ///< source of the library
            std::vector<std::string> content;   ///< content of the library

        public:
            HDF5Library(const std::string & filename);
            std::vector<std::string> get_content();
            Filter load_filter(const std::string & filter_name);
            std::vector<std::string> find (const std::string & name,
                                           bool case_sensitive=true);
            std::string get_source();


    };

    /**
     * @brief Construct a new HDF5Library object
     *
     * @param filename source filename of the HDF5 library
     */
    HDF5Library::HDF5Library(const std::string & filename){
        this->source = filename;

        // record content of the file (datasets in /filters/)
        H5Easy::File hf_file(filename, H5Easy::File::ReadOnly);
        this->content = hf_file.getGroup("filters").listObjectNames();
    }

    /**
     * @brief return the list of filters in the library
     *
     * @return std::vector<std::string>
     */
    std::vector<std::string> HDF5Library::get_content(){
        return this->content;
    }

    /**
     * @brief Load a given filter from the library
     *
     * @param filter_name   normalized names according to the library
     * @return Filter object
     */
    Filter HDF5Library::load_filter(const std::string& filter_name){
        return get_filter_from_hdf5_library(this->source, filter_name);
    }

    /**
     * @brief Look for filter names
     *
     * @param name                      name to look for
     * @param case_sensitive            set if the case needs to be respected (default true)
     * @return std::vector<std::string> list of potential candidates
     */
    std::vector<std::string> HDF5Library::find (
        const std::string & name, bool case_sensitive){

        std::vector<std::string> result;

        if (case_sensitive) {
            for (const auto & lib_name: this->get_content()) {
                if (lib_name.find(name) != std::string::npos)
                    result.push_back(lib_name);
            }
        } else {
            std::string name_lower = tolower(name);
            for (const auto & lib_name: this->get_content()) {
                if (tolower(lib_name).find(name) != std::string::npos)
                    result.push_back(lib_name);
            }
        }
        return result;
    }

    /**
     * @brief Get the source filename of the library
     *
     * @return std::string filename
     */
    std::string HDF5Library::get_source(){
        return this->source;
    }

    /**
     * @brief Nice representation of Filter objects
     *
     * @param os   stream to output the representation
     * @param F    Filter object
     * @return std::ostream&  same as os
     */
    std::ostream & operator<<(std::ostream &os,
                              HDF5Library &l){
        os << "HDF5Library: " << l.get_source() << "\n"
           << "   Contains " << l.get_content().size() << " registered filters."
           << "\n";
        return os;
    }
};

HIGHFIVE_REGISTER_TYPE(cphot::filter_t, cphot::create_compound_filter_t);