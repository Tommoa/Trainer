#include "variables.hpp"

#include "errors.hpp"

#ifdef _MSC_VER
#include <tlhelp32.h>

size_t get_proccess_by_name(std::wstring name) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	std::string s(name.begin(), name.end());

	HANDLE snapshot = CreateToolhelp32Snapshot(2, NULL);

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (stricmp(entry.szExeFile, s.c_str()) == 0) {
				return reinterpret_cast<size_t>(OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID));
			}
		}
	}
 
	return 0;
}
#endif
 
void variables::create(std::vector<data_member>& data_member_list,
#ifdef _MSC_VER
					   std::wistream& ss) {
	std::wstring type;
	std::wstring name;
#else
					   std::istream& ss) {
	std::string type;
	std::string name;
#endif

	data_member temp;

	if (!(ss >> type))
		throw errors::types::incomplete_command;
	temp.str_to_type_id(type);

	if (!(ss >> name))
		throw errors::types::incomplete_command;
	temp.name = name;

	size_t offset;
	while ((ss >> std::hex >> offset)) {
		temp.offsets.push_back(std::move(offset));
	}

	if (current_pid == 0) {
		if (name_of_window.length() > 0) {
			temp.window_name = name_of_window;
#ifdef _MSC_VER
			auto window_handle = FindWindowW(NULL, name_of_window.c_str());
			if (window_handle == 0)
				temp.handle = 0;
			else {
				auto pid = GetWindowThreadProcessId(window_handle, NULL);
				current_pid = temp.handle = reinterpret_cast<size_t>(
					OpenProcess(PROCESS_ALL_ACCESS, false, pid));
			}
#else
#endif 
		}

		if (name_of_process.length() > 0) {
			temp.process_name = name_of_process;
			current_pid = temp.handle = get_proccess_by_name(name_of_process);
#ifdef _MSC_VER
#else
#endif
		}
	}
	else temp.handle = current_pid;

	data_member_list.push_back(std::move(temp));
}

void variables::set(std::vector<data_member>& data_member_list,
#ifdef _MSC_VER
					std::wistream& ss) {
	std::wstring name, to;
#else
					std::istream& ss) {
	std::string name;
	std::string to;
#endif

	if (!(ss >> name))
		throw errors::types::incomplete_command;

	if (!(ss >> to))
		throw errors::types::incomplete_command;

	for (auto& member : data_member_list)
		if (member.name == name) {
			member.set_data(to);
			return;
		}

	throw errors::types::not_a_variable;
}

#ifdef _MSC_VER
std::wstring variables::get(std::vector<data_member>& data_member_list,
						   std::wistream& ss) {
	std::wstring name;
#else
std::string variables::get(std::vector<data_member>& data_member_list,
						   std::istream& ss) {
	std::string name;
#endif

	if (!(ss >> name))
		throw errors::types::incomplete_command;

	for (auto& member : data_member_list)
		if (member.name == name) {
			member.update_data();
			return member.get_data();
		}
	throw errors::types::not_a_variable;
}
#ifdef _MSC_VER
void variables::set_window(std::wstring str) {
#else 
void variables::set_window(std::string str) {
#endif
	name_of_process.clear();
	current_pid = 0;
	name_of_window = str;
}

#ifdef _MSC_VER
void variables::set_executable(std::wstring str) {
#else 
void variables::set_executable(std::string str) {
#endif
	name_of_window.clear();
	current_pid = 0;
	name_of_process = str;
}
