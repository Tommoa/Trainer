#include "variables.hpp"

#include "Snippets/defer.hpp"
#include "errors.hpp"
#include "data_member.hpp"

#include <vector>
#include <istream>

void variables::create_variable(std::vector<data_member>& data_member_list,
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

void variables::set_variable(std::vector<data_member>& data_member_list,
				  std::istream& ss) {
	std::string name;
	if (!(ss >> name))
		throw errors::types::incomplete_command;

	std::string to;
	if (!(ss >> to))
		throw errors::types::incomplete_command;

	for (auto& member : data_member_list)
		if (member.name == name) {
			member.set_data(to);
			return;
		}

	throw errors::types::not_a_variable;
}

std::string variables::get_variable(std::vector<data_member>& data_member_list, std::istream& ss) {
	std::string name;
	if (!(ss >> name))
		throw errors::types::incomplete_command;

	for (auto& member : data_member_list)
		if (member.name == name) {
			return member.get_data();
		}
	throw errors::types::not_a_variable;
}
