#ifndef __TRAINER_VARIABLES_HPP__
#define __TRAINER_VARIABLES_HPP__

#include "data_member.hpp"

#include <istream>
#include <vector>

namespace variables {

	static size_t current_pid;

#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
	static std::wstring name_of_process;
	static std::wstring name_of_window;

	void create(std::vector<data_member>& data_member_list, std::wistream& in);

	void set(std::vector<data_member>& data_member_list, std::wistream& in);

	std::wstring get(std::vector<data_member>& data_member_list,
					 std::wistream& in);

	void set_executable(std::wstring str);

	void set_window(std::wstring str);
#else
	static std::string name_of_process;
	static std::string name_of_window;

	void create(std::vector<data_member>& data_member_list, std::istream& in);

	void set(std::vector<data_member>& data_member_list, std::istream& in);

	std::string get(std::vector<data_member>& data_member_list,
					std::istream& in);

	void set_executable(std::string str);

	void set_window(std::string str);
#endif
}

#endif
