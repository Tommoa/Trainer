#include "read_stream.hpp"

#include "data_member.hpp"
#include "errors.hpp"

#include <fstream>
#include <string>
#include <sstream>

void read_stream(std::vector<data_member>& data_member_list, std::istream& in,
				 std::ostream& out, std::ostream& err) {
	std::string line;
	while (true) {
		std::getline(in, line);
		int last = 0;
		int current = line.find(' ', last);
		if (current == 0)
			break;
		if (line.substr(last, current - last) == "create") {
			last = current + 1;
			try {
				current = line.find(' ', last);
				data_member temp;
				temp.str_to_type_id(line.substr(last, current - last));
				last = current + 1;
				current = line.find(' ', last);
				temp.name = line.substr(last, current - last);
				last = current + 1;
				std::istringstream ss(line.substr(last, line.size()));
				size_t offset;
				while ((ss >> std::hex >> offset)) {
					temp.offsets.push_back(offset);
				}
				data_member_list.push_back(temp);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (line.substr(last, current - last) == "set") {
			last = current + 1;
			try {
				current = line.find(' ', last);
				auto n = line.substr(last, current - last);
				last = current + 1;
				bool found = false;
				for (auto &member : data_member_list) {
					if (member.name == n) {
						member.set_data(line.substr(last, line.size())); 
						found = true;
						break;
					}
				}
				if (!found)
					throw errors::types::not_a_variable;
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (line.substr(last, current - last) == "get") {
			last = current + 1;
			try {
				current = line.find(' ', last);
				auto n = line.substr(last, current - last);
				last = current + 1;
				bool found = false;
				for (auto &member : data_member_list) {
					if (member.name == n) {
						out << member.get_data() << std::endl; 
						found = true;
						break;
					}
				}
				if (!found)
					throw errors::types::not_a_variable;
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (line.substr(last, current) == "export") {
			try {
				last = current + 1;
				current = line.find(' ', last);
				auto type = line.substr(last, current - last);
				last = current + 1;
				if (type != "binary" && type != "file") {
					throw errors::types::invalid_file_type;
				}
				auto filename = line.substr(last, line.size());
				std::ofstream file(filename,
								   type == "binary" ? std::ios::binary
													: std::ios::trunc);
				for (auto member : data_member_list) {
					file << "create " << member.get_type() << " "
						 << member.name;
					file << std::endl;
					file << "set " << member.name << " " << member.get_data()
						 << std::endl;
				}
				file.close();
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (line.substr(last, current) == "import") {
			try {
				last = current + 1;
				current = line.find(" ", last);
				auto mode = line.substr(last, current - last);
				if (mode != "binary" && mode != "file") {
					throw errors::types::invalid_file_type;
				}
				last = line.find(" ", current + 1) + 1;
				auto filename = line.substr(last, line.size());
				std::ifstream file(filename,
								   mode == "binary" ? std::ios::binary
													: std::ios::in);
				read_stream(data_member_list, file, out, err);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (line.substr(last, current) == "exit")
			break;
	}
	return;
}
