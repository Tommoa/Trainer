#include "read_stream.hpp"

#include <iostream>
#include <vector>

int main() {
	std::vector<data_member*> data_member_list;
	read_stream(data_member_list, std::cin, std::cout, std::cerr);
}
