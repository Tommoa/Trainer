#include "interpreter.hpp"

#include "data_member.hpp"
#include "errors.hpp"
#include "porter.hpp"
#include "variables.hpp"

#include "Snippets/defer.hpp"

#include <sstream>

const std::string new_executable_command = "[";
const std::string create_variable_command = "create";
const std::string set_variable_command = "set";
const std::string get_variable_command = "get";
const std::string export_variables_command = "export";
const std::string import_variables_command = "import";
const std::string exit_command = "exit"; 
const std::string help_command = "help";
const std::string list_variables_command = "list";

bool interpreter::read_stream(std::vector<data_member>& data_member_list,
							  std::istream& in, std::ostream& out, 
							  std::ostream& err) {
	std::string line;
	while (true) {
		out << " > ";
		std::getline(in, line);
		std::istringstream ss(line);
		std::string command;
		if (!(ss >> command)) // Empty line, just continue.
			continue;
		if (command == list_variables_command) {
			try {
				for (auto member : data_member_list) {
					out << member.name << ": " << member.get_type()
						<< std::endl;
				}
			}
			catch (...) {
				errors::dispatcher(err);
			}
			continue;
		} 
		if (command == help_command) {
			out << "\nCommands:\n" <<
				"list\ncreate\nset\nget\nexport\nimport\nexit\n" << 
				std::endl;
			continue;
		}
		if (command.substr(0, 1) == new_executable_command) {
			try {
				variables::set_executable(
					ss.str().substr(1, ss.str().length() - 2));
			}
			catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == create_variable_command) {
			try {
				variables::create(data_member_list, ss);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == set_variable_command) {
			try {
				variables::set(data_member_list, ss);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == get_variable_command) {
			try {
				out << variables::get(data_member_list, ss) << std::endl;
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == export_variables_command) {
			try {
				porter::stream_export(data_member_list, ss);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == import_variables_command) {
			try {
				porter::stream_import(data_member_list, ss, out, err);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == exit_command) {
			if (ss >> command)
				return false;
			break;
		}
		try {
			throw errors::types::not_a_command;
		} catch (...) {
			errors::dispatcher(err);
		}
	}
	return true;
}
