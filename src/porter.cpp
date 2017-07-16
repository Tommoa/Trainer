#include "porter.hpp"

#include "Snippets/defer.hpp"
#include "errors.hpp"
#include "interpreter.hpp"

#include <fstream>

#ifdef _MSC_VER
const std::wstring binary_file = L"binary";
const std::wstring standard_file = L"file";
#else
const std::string binary_file = "binary";
const std::string standard_file = "file";
#endif

void porter::stream_export(std::vector<data_member>& data_member_list,
#ifdef _MSC_VER
						   std::wistream& in) {
	std::wstring file_format, filename;
#else
						   std::istream& in) {
	std::string file_format, filename;
#endif
	if (!(in >> file_format))
		throw errors::types::incomplete_command;

	if (!(in >> filename))
		throw errors::types::incomplete_command;

	if (file_format != binary_file && file_format != standard_file) {
		throw errors::types::invalid_file_type;
	}

#ifdef _MSC_VER
	std::wofstream file(
#else
	std::ofstream file(
#endif
		filename, file_format == binary_file ? std::ios::binary : std::ios::trunc);

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
