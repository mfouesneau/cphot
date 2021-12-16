/**
 * @file helpers.hpp
 * @brief Some random tools to ease the project
 * @version 0.1
 * @date 2021-12-01
 *
 */
#ifndef Helpers_HH
#define Helpers_HH
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <dirent.h>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>


// for windows mkdir
#ifdef _WIN32
#include <direct.h>
#endif

/**
 * Display an `std::vector` object
 */
template <typename T>
std::ostream & operator<<(std::ostream &os,
                          const std::vector<T> &v)
{
    os << "[ ";
    for (const T &p : v){ os << p << " ";}
    os << "]\n";
    return os;
}


/**
 * Check if a vector contains an item
 */
bool contains(const std::vector<std::string>& v, const std::string& other){
    for (const auto& entry: v){
        if (entry.compare(other) == 0){
            return true;
        }
    }
    return false;
}


namespace utils
{

    /**
     * Check if a file exists regardless of types.
     * @param name    filename and path to check
     * @return whether the file exists
     */
    inline bool file_exists (const std::string& name) {
      struct stat buffer;   
      return (stat (name.c_str(), &buffer) == 0); 
    }

    /**
     * Checks if a folder exists
     * @param foldername path to the folder to check.
     * @return true if the folder exists, false otherwise.
     */
    bool folder_exists(std::string foldername)
    {
        struct stat st;
        stat(foldername.c_str(), &st);
        return st.st_mode & S_IFDIR;
    }

    /**
     * Portable wrapper for mkdir. Internally used by mkdir()
     * @param[in] path the full path of the directory to create.
     * @return zero on success, otherwise -1.
     */
    int _mkdir(const char *path)
    {
    #ifdef _WIN32
        return ::_mkdir(path);
    #else
    #if _POSIX_C_SOURCE
        return ::mkdir(path, 0755);
    #else
        return ::mkdir(path, 0755); // not sure if this works on mac
    #endif
    #endif
    }

    /**
     * Recursive, portable wrapper for mkdir.
     * @param[in] path the full path of the directory to create.
     * @return zero on success, otherwise -1.
     */
    int mkdir(const char *path)
    {
        std::string current_level = "";
        std::string level;
        std::stringstream ss(path);

        // split path using slash as a separator
        while (std::getline(ss, level, '/'))
        {
            current_level += level; // append folder to the current level

            // create current level
            if (!folder_exists(current_level) && _mkdir(current_level.c_str()) != 0)
                return -1;

            current_level += "/"; // don't forget to append a slash
        }

        return 0;
    }
}


/**
 * Return the list of files matching a pattern from a given directory
 *
 * @param path  	where to look for files
 * @param pattern   which pattern to match (regex needed)
 */
std::vector<std::string> list_dir(const std::string& path, 
								  const std::string& pattern = ".*") {

	struct dirent *entry;
	DIR *dir = opendir(path.c_str());
	const std::regex txt_regex(pattern);

	std::vector<std::string> results;
	if (dir == NULL) {
	    return results;
	}
	while ((entry = readdir(dir)) != NULL) {
		if (std::regex_match(entry->d_name, txt_regex)){
				results.push_back(path + "/" + entry->d_name);
		}
	}
	closedir(dir);
	return results;
}


/**
 * Split string into bits according to a given delimiter.
 *
 * @param text       original string
 * @param delimiter  string to cut on
 *
 * @return a vector of strings
 * */
std::vector<std::string> split_string(const std::string & text, 
                                      const std::string & delimiter){
    size_t pos;
    std::string token;
    std::string s(text);
    std::vector<std::string> tokens;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);
    return tokens;
}


/**
 * Returns the upper case of a string 
 */
std::string toupper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), 
        [](unsigned char c){ return std::toupper(c); } 
    );
    return s;
}

/**
 * Returns the lower case of a string 
 */
std::string tolower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), 
        [](unsigned char c){ return std::tolower(c); } 
    );
    return s;
}

/**
 * Helper to make a string with formatting options.
 * Note that c++20 has std::format in the <format> library
 *
 * @param format    formatting string similar to printf
 * @param args      formatting arguments
 *
 * @return the formatted string
 */
template< typename... Args >
std::string string_sprintf( const char* format, Args... args ) {
  int length = std::snprintf( nullptr, 0, format, args... );
  assert( length >= 0 );

  char* buf = new char[length + 1];
  std::snprintf( buf, length + 1, format, args... );

  std::string str( buf );
  delete[] buf;
  return str;
}

/**
* @brief Parse a char* into a numeric value.
* 
* @tparam T   the output type 
* @param txt  the string to parse
* @return T   the value as a T type
*/
template <typename T>
T parseString(const char * txt){
    T d  = std::atof(txt);
    return d;
}

/**
* @brief Parse a std::string into a numeric value.
* 
* @tparam T   the output type 
* @param txt  the string to parse
* @return T   the value as a T type
*/
template <typename T>
T parseString(const std::string& txt){
    T d  = std::atof(txt.c_str());
    return d;
}

#endif // Helpers_HH
// vim: expandtab:ts=4:softtabstop=4:shiftwidth=4