#ifndef __TRAINER_ERRORS_HPP__
#define __TRAINER_ERRORS_HPP__

#include <ostream>

namespace errors {
	enum class types {
		invalid_set,
		invalid_file_type,
		type_not_found,
		not_a_variable,
		not_a_command
	};
	void dispatcher(std::ostream& out);
}
#endif
