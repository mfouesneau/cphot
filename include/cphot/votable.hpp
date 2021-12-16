/**
 * @defgroup VOTABLE VOTable reader
 * @brief Reading VOTABLEs
 *
 * This package is a very first draft of a VOTABLE reader. It was intended to
 * read small "simple" tables rapidly. It  does not make specific verifications
 *
 * VOTable supports a number of different serialization formats.
 *
 * * TABLEDATA stores the data in pure XML, where the numerical values are written
 * as human-readable strings.
 *
 * * BINARY is a binary representation of the data, stored in the XML as an opaque
 * base64-encoded blob.
 *
 * * BINARY2 was added in VOTable 1.3, and is identical to “BINARY”, except that
 * it explicitly records the position of missing values rather than identifying
 * them by a special value.
 *
 * * FITS stores the data in an external FITS file. This serialization is not
 * supported
 *
 * \warning Currently only the TABLEDATA is supported by this package
 *
 * version: 0.1a
 */
#pragma once
#include "helpers.hpp"
#include "prettyprint.hpp"
#include "rapidjson/document.h"
#include "xml2json.hpp"
#include <map>
#include <string>


/** \ingroup VOTABLE
 */
namespace votable {

    /**
    * @brief Store Table Parameter Attributes
    *
    * @ingroup VOTABLE
    */
    struct Param {
        std::string name;
        std::string datatype;
        std::string value;
        std::string ucd;
        std::string utype;
        std::string unit;
        std::string description;
    };

    /**
    * @brief Store Table Field and data
    *
    * @ingroup VOTABLE
    */
    struct Field {
        std::string name;
        std::string datatype;
        std::string ucd;
        std::string utype;
        std::string unit;
        std::string description;
        std::vector<std::string> data;
    };


    /**
    * @brief Store parsed data from VOTable with a non-string format
    * @ingroup VOTABLE
    *
    * Preserves the information from a field, but parses the data into T type
    * to be easily usable.
    *
    * @tparam T   data type
    */
    template <typename T>
    struct VOField {
        std::string name;
        std::string datatype;
        std::string ucd;
        std::string utype;
        std::string unit;
        std::string description;
        std::vector<T> data;
    };



    /**
    * Display an `std::vector` object
    */
    std::ostream & operator<<(std::ostream &os,
                            const Param &v)
    {
        os << "Param: " << v.name << "= "
           << v.value  << " [" << v.unit << "]\n"
           << "     DTYPE=" << v.datatype << "\n"
           << "     UCD=" << v.ucd << "\n"
           << "     DESCRIPTION=" << v.description << "\n";
        return os;
    }


    /**
    * Display a `Field` structure
    */
    std::ostream & operator<<(std::ostream &os,
                            const Field &v)
    {
        os << "Field: " << v.name << "length="
           << v.data.size()  << " [" << v.unit << "]\n"
           << "     DTYPE=" << v.datatype << "\n"
           << "     UCD=" << v.ucd << "\n"
           << "     DESCRIPTION=" << v.description << "\n";
        if (v.data.size() > 0){
            os << "     [ ";
            for (size_t idx=0; idx < 5; ++idx) {os << v.data[idx] << ", ";}
            os << "..., ";
            for (size_t idx=v.data.size() - 5; idx < v.data.size() - 1; ++idx) {os << v.data[idx] << ", ";}
            os << v.data[v.data.size() - 1] << "]\n";
        }
        return os;
    }


    /**
    * Display a `VOField` structure
    */
    template <typename T>
    std::ostream & operator<<(std::ostream &os,
                            const VOField<T> &v)
    {
        os << "Field: " << v.name << "length="
           << v.data.size()  << " [" << v.unit << "]\n"
           << "     DTYPE=" << v.datatype << "\n"
           << "     UCD=" << v.ucd << "\n"
           << "     DESCRIPTION=" << v.description << "\n";
        if (v.data.size() > 0){
            os << "     [ ";
            for (size_t idx=0; idx < 5; ++idx) {os << v.data[idx] << ", ";}
            os << "..., ";
            for (size_t idx=v.data.size() - 5; idx < v.data.size() - 1; ++idx) {os << v.data[idx] << ", ";}
            os << v.data[v.data.size() - 1] << "]\n";
        }
        return os;
    }

    /**
     * @brief Class defining a very simple parser of XML VOTable
     *
     * @ingroup VOTABLE
     *
     * This class reads in the XML content assuming a single table was defined.
     * It stores the descriptions into fields and params (attributes of the
     * table) the `get` methods allows one to rapidly access the data into a
     * non-string type.
     */
    class VOTable {

        public:
            std::map<std::string, Param> params;  ///< Table parameters
            std::vector<Field> fields;            ///< Table Fields

            VOTable();
            VOTable(const std::string & input_filename);
            void from_content(const std::string & content);
            size_t size();
            size_t n_columns();

            template <typename T> VOField<T> get(std::string field_name);
            template <typename T> VOField<T> get(size_t field_index);

        private:
            rapidjson::Document document;  /** Storing the JSON document */
            size_t n_data = 0;  ///< length of the data
            void parse(const std::string & input_filename);
            void setup_params();
            void setup_fields();
    };


/**
 * Dummy Constructor
 *
 */
VOTable::VOTable(){}

/**
 * Constructor
 *
 * @param input_filename: XML file to parse
 */
VOTable::VOTable(const std::string & input_filename){
    this->parse(input_filename);
    this->setup_params();
    this->setup_fields();
}

/**
 * @brief Constructor
 *
 * @param content: XML file content to parse
 */
void VOTable::from_content(const std::string & content){
    std::string json_str = xml2json(content.c_str());
    this->document.Parse(json_str.c_str());
    this->setup_params();
    this->setup_fields();
}

/**
 * @brief Parse the xml file into a json document
 *
 * @param input_filename file to parse
 */
void VOTable::parse(const std::string & input_filename){
    std::ostringstream oss;
    std::ifstream infile;
    infile.open(input_filename);
    oss.str("");
    oss << infile.rdbuf();
    infile.close();
    std::string json_str = xml2json(oss.str().data());
    this->document.Parse(json_str.c_str());
}

/**
 * @brief Read the document PARAM elements
 *
 * PARAM elements contain attributes of the VOTABLE
 * These are stored into `VOTable::params`
 */
void VOTable::setup_params(){

    if (!this->document["VOTABLE"]["RESOURCE"]["TABLE"].HasMember("PARAM")){
        std::cerr << "No PARAM elements found in VOTABLE\n";
        return;
    }
        // read in the document PARAMs
        const rapidjson::Value& where = this->document["VOTABLE"]["RESOURCE"]["TABLE"]["PARAM"];
        for (const auto& v : where.GetArray()){
            Param p;
            auto name = v["@name"].GetString();
            p.name = name;
            p.datatype = v["@datatype"].GetString();
            p.value = v["@value"].GetString();
            if (v.HasMember("@ucd")) {p.ucd = v["@ucd"].GetString();}
            if (v.HasMember("@ytype")) {p.utype = v["@utype"].GetString();}
            if (v.HasMember("@unit")) {p.unit = v["@unit"].GetString();}
            if (v.HasMember("@DESCRIPTION")) {p.description = v["@DESCRIPTION"].GetString(); }
            this->params[name] = p;
        }
}

/**
 * @brief Read the document FIELD and DATA elements
 *
 * FIELD elements contain column description of the VOTABLE
 * These are stored into `VOTable::fields`
 *
 * The DATA element contain the table(s).
 * These are stored as strings in the `VOTable::fields::data` vectors
 * [Note: I do not know what happens with multi tables]
 */
void VOTable::setup_fields(){

    // Read in FIELD information
    const rapidjson::Value& where = this->document["VOTABLE"]["RESOURCE"]["TABLE"]["FIELD"];
    for (const auto& v : where.GetArray()){
        Field p;
        auto name = v["@name"].GetString();
        p.name = name;
        p.datatype = v["@datatype"].GetString();
        if (v.HasMember("@ucd")) {p.ucd = v["@ucd"].GetString();}
        if (v.HasMember("@ytype")) {p.utype = v["@utype"].GetString();}
        if (v.HasMember("@unit")) {p.unit = v["@unit"].GetString();}
        if (v.HasMember("@DESCRIPTION")) {p.description = v["@DESCRIPTION"].GetString(); }
        this->fields.push_back(p);
    }

    // setup data arrays
    const rapidjson::Value& data = this->document["VOTABLE"]["RESOURCE"]["TABLE"]["DATA"]["TABLEDATA"]["TR"];
    size_t n_entries = data.GetArray().Size();
    size_t n_fields = this->fields.size();
    for (auto & field: this->fields){
        field.data.resize(n_entries);
    }
    this->n_data = n_entries;

    // Store data
    const auto& tr_array = data.GetArray();
    for (size_t k_entry=0; k_entry < n_entries; ++k_entry){
        const auto& td = tr_array[k_entry]["TD"].GetArray();
        for( size_t k_field=0; k_field < n_fields; ++k_field){
            this->fields[k_field].data[k_entry] = td[k_field].GetString();
        }
    }
}

/**
 * @brief Size of the data table
 *
 * @return size_t length of the data
 */
size_t VOTable::size(){
    return this->n_data;
}

/**
 * @brief number of fields
 *
 * @return size_t length of the fields
 */
size_t VOTable::n_columns(){
    return this->fields.size();
}

/**
 * @brief Retrieve field data
 *
 * @tparam T              type to parse the data into
 * @param field_name       name of the field
 * @return std::vector<T>  data vector
 */
template <typename T>
VOField<T> VOTable::get(std::string field_name){
    size_t match = -1;
    for (size_t idx=0; idx < this->fields.size(); ++idx){
        if (field_name.compare(this->fields[idx].name) == 0){
            match = idx;
            break;
        }
    }
    if (match == (size_t(-1))){
        throw std::runtime_error("Field: " + field_name + " not found.");
    }
    return this->get<T>(match);
};

/**
 * @brief Retrieve field data
 *
 * @tparam T              type to parse the data into
 * @param field_index      index of the field
 * @return std::vector<T>  data vector
 */
template <typename T>
VOField<T> VOTable::get(size_t field_index){
    size_t n = this->size();
    VOField<T> newfield;
    newfield.data.resize(n);
    const auto & field = this->fields[field_index];
    std::transform(
        field.data.begin(),
        field.data.end(),
        newfield.data.begin(), [](std::string val) { return parseString<T>(val);});
    newfield.name = field.name;
    newfield.datatype = field.datatype;
    newfield.ucd = field.ucd;
    newfield.utype = field.utype;
    newfield.unit = field.unit;
    newfield.description = field.description;
    return newfield;
};

/**
* Display a `VOTable` object
*/
std::ostream & operator<<(std::ostream &os,
                            const VOTable &vot)
{
    os << "VOTable(\n";
    for (const auto & p: vot.params){
        os << "  - " << p.second;
    }
    os << "\n";
    for (const auto & f: vot.fields){
        os << "  - " << f;
    }
    os << ")\n";
    return os;
}

} // namespace votable