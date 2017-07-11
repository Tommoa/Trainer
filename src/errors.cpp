#include "errors.hpp"

#include <exception>

void errors::dispatcher(std::ostream& out) {
	try {
		throw;
	} catch (types T) {
		if (T == types::invalid_set)
			out << "Invalid entry" << std::endl;
		if (T == types::invalid_file_type)
			out << "Invalid file type. Allowed types are 'file' and 'binary'"
				<< std::endl;
	} catch (std::exception e) {
		out << "Error!\n\t" << e.what() << std::endl;
	}
}
