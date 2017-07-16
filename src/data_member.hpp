#ifndef __TRAINER_DATA_MEMBER_HPP__
#define __TRAINER_DATA_MEMBER_HPP__

#include <string>
#include <typeinfo>
#include <vector>

#ifdef _MSC_VER
#include <Windows.h>
#endif

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
	size_t handle = 0;
	std::string window_name;	 // The name of the window that can be used to get PID
	std::string name;			 // The name of the data_member
	std::vector<size_t> offsets; // The memory offsets to get to the data member
	std::type_info* type;
	void str_to_type_id(std::string str);
	std::string get_type();
	std::string get_data();
	void set_data(std::string data);
	void update_data();
	template<typename T> void put_data(T data);
};

template<typename T> void data_member::put_data(T data) {
	size_t offset;
	for (auto a : offsets) { 
		offset += a;
#ifdef _MSC_VER
		size_t nBytes;
		ReadProcessMemory(reinterpret_cast<void*>(handle), reinterpret_cast<void*>(offset), reinterpret_cast<void*>(&offset), sizeof(size_t), &nBytes); 
		if (nBytes != sizeof(size_t))
			throw errors::types::invalid_memory;
#else
#endif
	}
#ifdef _MSC_VER
	size_t nBytes;
	WriteProcessMemory(reinterpret_cast<void*>(handle), reinterpret_cast<void*>(offset), reinterpret_cast<void*>(&data), sizeof T, &nBytes); 
	if (nBytes != sizeof T)
		throw errors::types::invalid_memory;
#else
#endif
}

#endif
