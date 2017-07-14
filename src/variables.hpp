#ifndef __TRAINER_VARIABLES_HPP__
#define __TRAINER_VARIABLES_HPP__

#include "data_member.hpp"

#include <vector>
#include <istream>
#include <ostream>

namespace variables {

	void create_variable(std::vector<data_member>& data_member_list, std::istream& in);

	void set_variable(std::vector<data_member>& data_member_list, std::istream& in);

	std::string get_variable(std::vector<data_member>& data_member_list, std::istream& in);

}

#endif
