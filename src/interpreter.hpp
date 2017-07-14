#ifndef __TRAINER_READ_STREAM_HPP__
#define __TRAINER_READ_STREAM_HPP__

#include "data_member.hpp"

#include <istream>
#include <ostream>
#include <vector>

namespace interpreter {
	void read_stream(std::vector<data_member>& data_member_list, std::istream& in,
					 std::ostream& out, std::ostream& err);
}
#endif
