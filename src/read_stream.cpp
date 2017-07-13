#include "read_stream.hpp"

#include "data_member.hpp"
#include "errors.hpp"

#include "Snippets/defer.hpp"

#include <fstream>
#include <sstream>
#include <string>

void create_variable(std::vector<data_member>& data_member_list,
					 std::istream& ss) {
	data_member temp;

	std::string type;
	if (!(ss >> type))
		throw errors::types::incomplete_command;
	temp.str_to_type_id(type);

	std::string name;
	if (!(ss >> name))
		throw errors::types::incomplete_command;
	temp.name = name;

	size_t offset;
	while ((ss >> std::hex >> offset)) {
		temp.offsets.push_back(std::move(offset));
	}

	data_member_list.push_back(std::move(temp));
}

void set_variable(std::vector<data_member>& data_member_list,
				  std::istream& ss) {
	std::string name;
	if (!(ss >> name))
		throw errors::types::incomplete_command;

	std::string to;
	if (!(ss >> to))
		throw errors::types::incomplete_command;

	bool found = false;
	for (auto& member : data_member_list)
		if (member.name == name) {
			member.set_data(to);
			found = true;
			break;
		}

	if (!found)
		throw errors::types::not_a_variable;
}

void get_variable(std::vector<data_member>& data_member_list, std::istream& ss,
				  std::ostream& out) {
	std::string name;
	if (!(ss >> name))
		throw errors::types::incomplete_command;

	bool found = false;
	for (auto& member : data_member_list)
		if (member.name == name) {
			out << member.get_data() << std::endl;
			found = true;
			break;
		}
	if (!found)
		throw errors::types::not_a_variable;
}

void export_file(std::vector<data_member>& data_member_list, std::istream& in) {
	std::string file_format;
	if (!(in >> file_format))
		throw errors::types::incomplete_command;

	std::string filename;
	if (!(in >> file_format))
		throw errors::types::incomplete_command;

	if (file_format != "binary" && file_format != "file") {
		throw errors::types::invalid_file_type;
	}

	std::ofstream file(
		filename, file_format == "binary" ? std::ios::binary : std::ios::trunc);

	defer { file.close(); }; // Ensure we close the file regardless of errors

	for (auto& member : data_member_list) {
		file << "create " << member.get_type() << " " << member.name;
		file << std::endl;
		file << "set " << member.name << " " << member.get_data() << std::endl;
	}
}

void import_file(std::vector<data_member>& data_member_list, std::istream& in,
				 std::ostream& out, std::ostream& err) {
	std::string mode;
	if (!(in >> mode))
		throw errors::types::incomplete_command;

	std::string filename;
	if (!(in >> filename))
		throw errors::types::incomplete_command;

	if (mode != "binary" && mode != "file") {
		throw errors::types::invalid_file_type;
	}
	std::ifstream file(filename,
					   mode == "binary" ? std::ios::binary : std::ios::in);
	defer { file.close(); };
	read_stream(data_member_list, file, out, err);
}

void read_stream(std::vector<data_member>& data_member_list, std::istream& in,
				 std::ostream& out, std::ostream& err) {
	std::string line;
	while (true) {
		std::getline(in, line);
		std::istringstream ss(line);
		std::string command;
		if (!(ss >> command)) // Empty line, just continue.
			continue;
		if (command == "create") {
			try {
				create_variable(data_member_list, ss);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == "set") {
			try {
				set_variable(data_member_list, ss);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == "get") {
			try {
				get_variable(data_member_list, ss, out);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == "export") {
			try {
				export_file(data_member_list, ss);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == "import") {
			try {
				import_file(data_member_list, ss, out, err);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == "exit")
			break;
		try {
			throw errors::types::not_a_command;
		} catch (...) {
			errors::dispatcher(err);
		}
	}
	return;
}
