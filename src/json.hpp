/*! @file */

#ifndef JSON_HPP
#define JSON_HPP

#include <string>
#include <vector>

#include "../extras/json/json.h"

//!Typedef'd in-case JSON library is ever changed.
typedef Json::Value json_t;

/**Converts a json_t to a std::string (same as the std::to_string function).
\param json			JSON object to convert.
\return				String representation of JSON object.
*/
// std::string JSON_serialize(const json_t& json);

/**Converts a std::string represented JSON object into a json_t (throws on bad JSON string).
\param stringified	String representation of JSON object.
\return				JSON object of the passed string.
*/
json_t JSON_parse(const std::string& stringified);

/**Converts a json_t to a std::string (same as the std::string JSON_serialize function).
\param json			JSON object to convert.
\return				String representation of JSON object.
*/
std::string to_string(const json_t& json);

/**Converts a JSON array object containing doubles or ints to a C++ vector of doubles (throws if json array contains values other than doubles or ints).
\param json			JSON object to convert.
\return				Vector of doubles.
*/
std::vector<double> to_double_array(const json_t& json);

/**Converts a JSON array object containing unsigned ints to a C++ vector of size_t (throws if json array contains values other than ints >= 0).
\param json			JSON object to convert.
\return				Vector of size_t.
*/
std::vector<size_t> to_size_array(const json_t& json);

/**Converts a JSON array object containing JSON array objects of doubles or ints to a C++ vector of vector of doubles (throws if json array contains values that contain values other than doubles or ints).
\param json			JSON object to convert.
\return				Vector of vector of doubles.
*/
std::vector<std::vector<double>> to_array_double_array(const json_t& json);

#endif
