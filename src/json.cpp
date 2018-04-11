#include "json.hpp"

#include <sstream>

std::string JSON_serialize(const json_t& json)
{
	Json::FastWriter writer;
	std::string stringified=writer.write(json);

	while(stringified.size()>0&&isspace(stringified[stringified.size()-1])!=0)
		stringified.pop_back();

	return stringified;
}

json_t JSON_parse(const std::string& stringified)
{
	std::istringstream istr(stringified);
	json_t json;
	istr>>json;
	return json;
}

std::string to_string(const json_t& json)
{
	return json.asString();
}

std::vector<double> to_double_array(const json_t& json)
{
	if(!json.isArray())
		throw std::runtime_error("Not a JSON double array.");

	std::vector<double> array;

	for(auto ii:json)
		if(!ii.isDouble()&&!ii.isInt())
			throw std::runtime_error("JSON array contains a non-double.");
		else
			array.push_back(ii.asDouble());

	return array;
}

std::vector<size_t> to_size_array(const json_t& json)
{
	if(!json.isArray())
		throw std::runtime_error("Not a JSON unsigned integer array.");

	std::vector<size_t> array;

	for(auto ii:json)
		if(!ii.isUInt())
			throw std::runtime_error("JSON array contains a non-unsigned integer.");
		else
			array.push_back(ii.asUInt());

	return array;
}

std::vector<std::vector<double>> to_array_double_array(const json_t& json)
{
	if(!json.isArray())
		throw std::runtime_error("Not a JSON array.");

	std::vector<std::vector<double>> array;

	for(auto ii:json)
		array.push_back(to_double_array(ii));

	return array;
}