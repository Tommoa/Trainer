#include "interpreter.hpp"

#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {
	std::vector<data_member> data_member_list;
	bool cont = true;
	if (argc > 1) { 
		std::ifstream file(argv[1]);
		cont = interpreter::read_stream(data_member_list, file,
								std::cout, std::cerr);
	}
	if (cont) {
		interpreter::read_stream(data_member_list,
								std::cin, std::cout, std::cerr);
	}
}
