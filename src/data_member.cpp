#include "data_member.hpp"

#include "errors.hpp"

void data_member::str_to_type_id(std::string str) {
	if (str == "string")
		type = const_cast<std::type_info*>(&typeid(char*));
	if (str == "int" || str == "integer")
		type = const_cast<std::type_info*>(&typeid(int));
	if (str == "long")
		type = const_cast<std::type_info*>(&typeid(long));
	if (str == "float")
		type = const_cast<std::type_info*>(&typeid(float));
	if (str == "double")
		type = const_cast<std::type_info*>(&typeid(double));
	if (str == "bool" || str == "boolean")
		type = const_cast<std::type_info*>(&typeid(bool));
}

std::string data_member::get_type() {
	if (*type == typeid(char*))
		return "string";
	if (*type == typeid(int))
		return "int";
	if (*type == typeid(long))
		return "long";
	if (*type == typeid(float))
		return "float";
	if (*type == typeid(double))
		return "double";
	if (*type == typeid(bool))
		return "bool";
	throw errors::types::type_not_found;
}

std::string data_member::get_data() {
	if (*type == typeid(char*))
		return (data.str);
	if (*type == typeid(int))
		return std::to_string(data.integer);
	if (*type == typeid(long))
		return std::to_string(data.long_int);
	if (*type == typeid(float))
		return std::to_string(data.floating);
	if (*type == typeid(double))
		return std::to_string(data.prec);
	if (*type == typeid(bool))
		return std::to_string(data.boolean);
	throw errors::types::type_not_found;
}

void data_member::set_data(std::string data) {
	if (*type == typeid(char*)) {
		this->data.str = const_cast<char*>(data.c_str());
		return;
	}
	if (*type == typeid(int)) {
		this->data.integer = std::stoi(data);
		return;
	}
	if (*type == typeid(long)) {
		this->data.long_int = std::stol(data);
		return;
	}
	if (*type == typeid(float)) {
		this->data.floating = std::stof(data);
		return;
	}
	if (*type == typeid(double)) {
		this->data.prec = std::stod(data);
		return;
	}
	if (*type == typeid(bool)) {
		this->data.boolean = data == "true";
		return;
	}
	throw errors::types::type_not_found;
}

// TODO: Make this function work without injection.
void data_member::update_data() {
	size_t* offset = 0;
	for (auto a : offsets) { 
		offset = offset + a;
		offset = reinterpret_cast<size_t*>(*offset);
	}
	if (*type == typeid(char*)) {
		this->data.str = reinterpret_cast<char*>(offset);
	}
	if (*type == typeid(int)) {
		this->data.integer = *reinterpret_cast<int*>(offset);
	}
	if (*type == typeid(long)) {
		this->data.long_int = *reinterpret_cast<long*>(offset);
	}
	if (*type == typeid(float)) { 
		this->data.floating = *reinterpret_cast<float*>(offset);
	}
	if (*type == typeid(double)) {
		this->data.prec = *reinterpret_cast<double*>(offset);
	}
	if (*type == typeid(bool)) {
		this->data.boolean = *reinterpret_cast<bool*>(offset);
	}
}
