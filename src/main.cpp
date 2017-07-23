#include "interpreter.hpp"

#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {
	std::vector<data_member> data_member_list;
	bool cont = true;
	if (argc > 1) { 
#ifdef _MSC_VER
		std::wifstream file(argv[1]);
#else
		std::ifstream file(argv[1]);
#endif 
		cont = interpreter::read_stream(data_member_list, file,
#ifdef _MSC_VER
								std::wcout, std::wcerr);
#else
								std::cout, std::cerr);
#endif
	}
	if (cont) {
		interpreter::read_stream(data_member_list,
#ifdef _MSC_VER
								std::wcin, std::wcout, std::wcerr);
#else
								std::cin, std::cout, std::cerr);
#endif
	}
}
