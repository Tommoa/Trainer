#ifndef __TRAINER_PORTER_HPP__
#define __TRAINER_PORTER_HPP__

#include "data_member.hpp"

#include <istream>
#include <vector>

namespace porter {
	void stream_export(std::vector<data_member>& data_member_list,
#ifdef _MSC_VER
					   std::wistream& ss);
#else
					   std::istream& ss);
#endif

	void stream_import(std::vector<data_member>& data_member_list,
#ifdef _MSC_VER
					   std::wistream& ss, std::wostream& out,
					   std::wofstream& err);
#else
					   std::istream& ss, std::ostream& out, std::ostream& err);
#endif
}

#endif
