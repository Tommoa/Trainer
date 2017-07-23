#include "interpreter.hpp"

#include "data_member.hpp"
#include "errors.hpp"
#include "porter.hpp"
#include "variables.hpp"

#include "Snippets/defer.hpp"

#include <sstream>

#ifdef _MSC_VER
const std::wstring new_executable_command = L"[";
const std::wstring window_command = L"[[";
const std::wstring create_variable_command = L"create";
const std::wstring set_variable_command = L"set";
const std::wstring get_variable_command = L"get";
const std::wstring export_variables_command = L"export";
const std::wstring import_variables_command = L"import";
const std::wstring exit_command = L"exit";
const std::wstring help_command = L"help";
const std::wstring list_variables_command = L"list";
#else
const std::string new_executable_command = "[";
const std::string window_command = "[[";
const std::string create_variable_command = "create";
const std::string set_variable_command = "set";
const std::string get_variable_command = "get";
const std::string export_variables_command = "export";
const std::string import_variables_command = "import";
const std::string exit_command = "exit"; 
const std::string help_command = "help";
const std::string list_variables_command = "list";
#endif

bool interpreter::read_stream(std::vector<data_member>& data_member_list,
#ifdef _MSC_VER
							  std::wistream& in, std::wostream& out,
							  std::wostream& err) {
	std::wstring line;
#else
							  std::istream& in, std::ostream& out,
							  std::ostream& err) {
	std::string line;
#endif
	while (true) {
		out << " > ";
		std::getline(in, line);
#ifdef _MSC_VER
		std::wistringstream ss(line);
		std::wstring command;
#else
		std::istringstream ss(line);
		std::string command;
#endif
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
		if (command.substr(0, 2) == window_command) {
			try {
				variables::set_window(
					ss.str().substr(2, ss.str().length() - 4));
			}
			catch (...) {
				errors::dispatcher(err);
			}
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
