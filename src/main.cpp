#include "interpreter.hpp"

#include <iostream>
#include <vector>

int main() {
	std::vector<data_member> data_member_list;
	interpreter::read_stream(data_member_list, std::cin, std::cout, std::cerr);
}
