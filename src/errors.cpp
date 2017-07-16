#include "errors.hpp"

#include <exception>

void errors::dispatcher(std::ostream& out) {
	try {
		throw;
	} catch (types T) {
		switch (T) {
			case types::invalid_set:
				out << "Invalid entry" << std::endl;
				break;
			case types::invalid_file_type:
				out << "Invalid file type. Allowed types are 'file' and "
					   "'binary'"
					<< std::endl;
				break;
			case types::type_not_found:
				out << "Either type is not found or the type was not set "
					   "successfully"
					<< std::endl;
				break;
			case types::not_a_variable:
				out << "Attempted to call a variable that doesn't exist"
					<< std::endl;
				break;
			case types::not_a_command:
				out << "Call is not a defined command" << std::endl;
				break;
			case types::incomplete_command:
				out << "The command given is incomplete" << std::endl;
				break;
			case types::invalid_memory:
				out << "The program attempted to access invalid memory"
					<< std::endl;
				break;
			case types::handle_not_set: 
				out << "The handle for this variable hasn't been set yet"
					<< std::endl;
				break;
			default:
				out << "Unknown error" << std::endl;
				break;
		}
	} catch (std::exception e) {
		out << "Error!\n\t" << e.what() << std::endl;
	}
}
