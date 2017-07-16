#include "variables.hpp"

#include "errors.hpp"

void variables::create(std::vector<data_member>& data_member_list,
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

	if (name_of_process.length() > 0) {
		temp.window_name = name_of_process;
#ifdef _MSC_VER
		auto window_handle = FindWindow(NULL, name_of_process.c_str());
		if (window_handle == 0)
			temp.handle = 0;
		else {
			auto pid = GetWindowThreadProcessId(window_handle, NULL);
			temp.handle = reinterpret_cast<size_t>(OpenProcess(PROCESS_ALL_ACCESS, false, pid));
		}
#else
#endif
	}

	data_member_list.push_back(std::move(temp));
}

void variables::set(std::vector<data_member>& data_member_list,
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

std::string variables::get(std::vector<data_member>& data_member_list,
						   std::istream& ss) {
	std::string name;
	if (!(ss >> name))
		throw errors::types::incomplete_command;

	for (auto& member : data_member_list)
		if (member.name == name) {
			member.update_data();
			return member.get_data();
		}
	throw errors::types::not_a_variable;
}
