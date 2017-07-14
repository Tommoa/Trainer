#ifndef __TRAINER_PORTER_HPP__
#define __TRAINER_PORTER_HPP__

#include "data_member.hpp"

#include <istream>
#include <vector>

namespace porter {
	void stream_export(std::vector<data_member>& data_member_list,
					   std::istream& ss);

	void stream_import(std::vector<data_member>& data_member_list,
					   std::istream& ss, std::ostream& out, std::ostream& err);
}

#endif
