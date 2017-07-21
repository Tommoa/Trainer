#ifndef __TRAINER_ERRORS_HPP__
#define __TRAINER_ERRORS_HPP__

#include <ostream>

namespace errors {
	enum class types {
		invalid_set,
		invalid_file_type,
		type_not_found,
		not_a_variable,
		not_a_command,
		incomplete_command,
		invalid_memory,
		handle_not_set,
	};
	void dispatcher(
#ifdef _MSC_VER
		std::wostream& out);
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#else
		std::ostream& out);
#endif
}

#endif
