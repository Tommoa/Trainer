#ifndef __TRAINER_VARIABLES_HPP__
#define __TRAINER_VARIABLES_HPP__

#include "data_member.hpp"

#include <istream>
#include <vector>

namespace variables {

#ifdef _MSC_VER
	static std::wstring name_of_process;

	void create(std::vector<data_member>& data_member_list, std::wistream& in);

	void set(std::vector<data_member>& data_member_list, std::wistream& in);

	std::wstring get(std::vector<data_member>& data_member_list,
					 std::wistream& in);
#else
	static std::string name_of_process;

	void create(std::vector<data_member>& data_member_list, std::istream& in);

	void set(std::vector<data_member>& data_member_list, std::istream& in);

	std::string get(std::vector<data_member>& data_member_list,
					std::istream& in);
#endif
}

#endif
