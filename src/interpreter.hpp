#ifndef __TRAINER_READ_STREAM_HPP__
#define __TRAINER_READ_STREAM_HPP__

#include "data_member.hpp"

#include <istream>
#include <ostream>
#include <vector>

namespace interpreter {
	bool read_stream(std::vector<data_member>& data_member_list,
#ifdef _MSC_VER
					 std::wistream& in, std::wostream& out, std::wostream& err);
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#else
					 std::istream& in, std::ostream& out, std::ostream& err);
#endif
}
#endif
