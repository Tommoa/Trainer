#include "interpreter.hpp"

#include "data_member.hpp"
#include "errors.hpp"
#include "variables.hpp"
#include "porter.hpp"

#include "Snippets/defer.hpp"

#include <sstream>

void interpreter::read_stream(std::vector<data_member>& data_member_list,
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
		if (command == "create") {
			try {
				variables::create(data_member_list, ss);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == "set") {
			try {
				variables::set(data_member_list, ss);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == "get") {
			try {
				out << variables::get(data_member_list, ss) << std::endl;
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == "export") {
			try {
				porter::stream_export(data_member_list, ss);
			} catch (...) {
				errors::dispatcher(err);
			}
			continue;
		}
		if (command == "import") {
			try {
				porter::stream_import(data_member_list, ss, out, err);
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
