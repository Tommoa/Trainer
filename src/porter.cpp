#include "porter.hpp"

#include "Snippets/defer.hpp"
#include "errors.hpp"
#include "interpreter.hpp"
#include "base64\encode.h"

#include <fstream>
#include <sstream>

const std::string b64_file = "b64";
const std::string standard_file = "file";

void porter::stream_export(std::vector<data_member>& data_member_list, std::istream& in) {
	std::string filename;
	std::string mode;

	if (!(in >> mode))
		throw errors::types::incomplete_command;

	if (!(in >> filename))
		throw errors::types::incomplete_command; 

	if (mode != b64_file && mode != standard_file)
		throw errors::types::invalid_file_type;

	bool b64 = (mode == b64_file);

	std::stringstream tmpStream(std::ios::out|std::ios::in); 
	std::ofstream file(
		filename,
		std::ios::trunc);
	defer { file.close(); }; // Ensure we close the file regardless of errors
 
	for (auto& member : data_member_list) {
		tmpStream << "[" << (member.process_name.length() > 0 ?
			member.process_name :
			"[" + member.window_name + "]")
			<< "]" << std::endl;
		tmpStream << "create " << member.get_type() << " " << member.name;
		for (auto offset : member.offsets)
			tmpStream << " " << std::hex << offset;
		tmpStream << std::endl;
		tmpStream << "set " << member.name << " " << member.get_data() << std::endl; 
	}
	tmpStream << "exit cont" << std::endl;

	if (b64) { 
		base64::encoder encoder_obj(128);
		encoder_obj.encode(tmpStream, file);
		return;
	}

	while (file << tmpStream.rdbuf());
}

void porter::stream_import(std::vector<data_member>& data_member_list,
						   std::istream& in, std::ostream& out,
						   std::ostream& err) {
	std::string filename;

	if (!(in >> filename))
		throw errors::types::incomplete_command;

	std::ifstream file( filename,std::ios::in);
	defer { file.close(); };
	interpreter::read_stream(data_member_list, file, out, err);
}