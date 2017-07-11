#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <iostream>
#include <exception>

namespace errors {
	enum class types {
		invalid_set,
		invalid_file_type
	};
	void dispatcher(std::ostream& out) {
		try {
			throw;
		}
		catch (types T) {
			if (T == types::invalid_set)
				out << "Invalid entry" << std::endl;
			if (T == types::invalid_file_type)
				out << "Invalid file type. Allowed types are 'file' and 'binary'" << std::endl;
		}
		catch (std::exception e) {
			out << "Error!\n\t" << e.what() << std::endl;
		}
	}
}

class data_member {
	union {
		char* str;
		int integer;
		long long_int;
		float floating;
		double prec;
		bool boolean;
	} data;
	public:
	std::string name; 	// The name of the data_member
	std::vector<unsigned long> offsets; 	// The memory offsets to get to the data member
	std::type_info* type;
	void str_to_type_id(std::string str) {
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
	std::string get_type() {
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
	}
	std::string get_data() { 
		if (*type == typeid(std::string))
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
	}
	void set_data(std::string data) {
		if (*type == typeid(std::string)) {
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
		}
	}
}; 

void read_stream(std::vector<data_member*>& data_member_list, std::istream& in, std::ostream& out, std::ostream& err) {
	std::string line;
	while (true) {
		std::getline(in, line); 
		int last = 0;
		int current = line.find(' ', last);
		if (current == 0)
			break; 
		if (line.substr(last, current-last) == "create") {
			last = current + 1;
			try {
					current = line.find(' ', last);
					data_member temp;
					temp.str_to_type_id(line.substr(last, current - last));
					last = current + 1;
					line.find(' ', last);
					temp.name = line.substr(last, current-last); 
					data_member_list.push_back(&temp);
			}
			catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (line.substr(last, current-last) == "set") { 
			last = current+1;
			try { 
				current = line.find(' ', last);
				auto n = line.substr(last, current-last);
				last = current + 1;
				for (auto member : data_member_list) {
					if (member->name == n) { 
						member->set_data(line.substr(last, line.size()));
					}
				}
			}
			catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (line.substr(last, current-last) == "get") {
			last = current + 1;
			try {
				current = line.find(' ', last);
				auto n = line.substr(last, current-last);
				last = current + 1;
				for (auto member : data_member_list) {
					if (member->name == n) {
						out << member->get_data() << std::endl;
					}
				}
			}
			catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (line.substr(last, current) == "export") {
			try {
				last = current+1;
				current = line.find(' ', last);
				auto type = line.substr(last, current-last);
				last = current +1;
				if (type != "binary" && type != "file") {
					throw errors::types::invalid_file_type;
				}
				auto filename = line.substr(last, line.size());
				std::ofstream file(filename, type == "binary" ? std::ios::binary : std::ios::trunc);
				for (auto member : data_member_list) {
					file << "create " << member->get_type() << " " << member->name;
					file << std::endl;
					file << "set " << member->name << " " << member->get_data() << std::endl;
				}
				file.close();
			}
			catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (line.substr(last, current) == "import") {
			try {
				last = current + 1;
				current = line.find(" ", last);
				auto mode = line.substr(last, current-last);
				if (mode != "binary" && mode != "file") {
					throw errors::types::invalid_file_type;
				}
				last = line.find(" ", current + 1) + 1;
				auto filename = line.substr(last, line.size());
				std::ifstream file(filename, mode == "binary" ? std::ios::binary : std::ios::in);
				read_stream(data_member_list, file, out, err);
			}
			catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (line.substr(last, current) == "exit")
			break;
	}
	return;
}

int main() {
	std::vector<data_member*> data_member_list;
	read_stream(data_member_list, std::cin, std::cout, std::cerr);
}