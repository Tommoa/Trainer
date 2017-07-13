#ifndef __TRAINER_DATA_MEMBER_HPP__
#define __TRAINER_DATA_MEMBER_HPP__

#include <string>
#include <typeinfo>
#include <vector>

class data_member {
	union {
		char* str;
		int integer;
		long long_int;
		float floating;
		double prec;
		bool boolean;
	} data;

  public:
	std::string name; // The name of the data_member
	std::vector<size_t>
		offsets; // The memory offsets to get to the data member
	std::type_info* type;
	void str_to_type_id(std::string str);
	std::string get_type();
	std::string get_data();
	void set_data(std::string data);
};
#endif
