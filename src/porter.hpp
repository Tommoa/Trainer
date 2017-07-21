#ifndef __TRAINER_PORTER_HPP__
#define __TRAINER_PORTER_HPP__

#include "data_member.hpp"

#include <istream>
#include <vector>

namespace porter {
	void stream_export(std::vector<data_member>& data_member_list,
#ifdef _MSC_VER
					   std::wistream& ss);
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#else
					   std::istream& ss);
#endif

	void stream_import(std::vector<data_member>& data_member_list,
#ifdef _MSC_VER
					   std::wistream& ss, std::wostream& out,
					   std::wostream& err);
#else
					   std::istream& ss, std::ostream& out, std::ostream& err);
#endif
}

#endif
