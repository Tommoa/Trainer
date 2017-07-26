#ifndef __TRAINER_VARIABLES_HPP__
#define __TRAINER_VARIABLES_HPP__

#include "data_member.hpp"

#include <istream>
#include <vector>

namespace variables {

	static size_t current_pid;

	static std::string name_of_process;
	static std::string name_of_window;

	void create(std::vector<data_member>& data_member_list, std::istream& in);

	void set(std::vector<data_member>& data_member_list, std::istream& in);

	std::string get(std::vector<data_member>& data_member_list,
					std::istream& in);

	void set_executable(std::string str);

	void set_window(std::string str);
}

#endif
