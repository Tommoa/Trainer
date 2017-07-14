#include "porter.hpp"

#include "Snippets/defer.hpp"
#include "errors.hpp"
#include "interpreter.hpp"

#include <fstream>

void porter::stream_export(std::vector<data_member>& data_member_list,
						   std::istream& in) {
	std::string file_format;
	if (!(in >> file_format))
		throw errors::types::incomplete_command;

	std::string filename;
	if (!(in >> file_format))
		throw errors::types::incomplete_command;

	if (file_format != "binary" && file_format != "file") {
		throw errors::types::invalid_file_type;
	}

	std::ofstream file(
		filename, file_format == "binary" ? std::ios::binary : std::ios::trunc);

	defer { file.close(); }; // Ensure we close the file regardless of errors

	for (auto& member : data_member_list) {
		file << "create " << member.get_type() << " " << member.name;
		file << std::endl;
		file << "set " << member.name << " " << member.get_data() << std::endl;
	}
}

void porter::stream_import(std::vector<data_member>& data_member_list,
						   std::istream& in, std::ostream& out,
						   std::ostream& err) {
	std::string mode;
	if (!(in >> mode))
		throw errors::types::incomplete_command;

	std::string filename;
	if (!(in >> filename))
		throw errors::types::incomplete_command;

	if (mode != "binary" && mode != "file") {
		throw errors::types::invalid_file_type;
	}
	std::ifstream file(filename,
					   mode == "binary" ? std::ios::binary : std::ios::in);
	defer { file.close(); };
	interpreter::read_stream(data_member_list, file, out, err);
}
