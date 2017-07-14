#ifndef __TRAINER_VARIABLES_HPP__
#define __TRAINER_VARIABLES_HPP__

#include "data_member.hpp"

#include <istream>
#include <ostream>
#include <vector>

namespace variables {

	void create(std::vector<data_member>& data_member_list, std::istream& in);

	void set(std::vector<data_member>& data_member_list, std::istream& in);

	std::string get(std::vector<data_member>& data_member_list,
					std::istream& in);
}

#endif
