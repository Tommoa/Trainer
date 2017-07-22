#ifndef __TRAINER_DATA_MEMBER_HPP__
#define __TRAINER_DATA_MEMBER_HPP__

#include "errors.hpp"

#include <string>
#include <typeinfo>
#include <vector>

#ifdef _MSC_VER
#include <Windows.h>
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
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
#ifdef _MSC_VER
	std::wstring window_name;
	std::wstring process_name;
	std::wstring name;
	std::wstring get_type();
	std::wstring get_data();
	void str_to_type_id(std::wstring str);
	void set_data(std::wstring data);
#else
	std::string
		window_name;  // The name of the window that can be used to get PID
	std::string process_name;
	std::string name; // The name of the data_member
	std::string get_type();
	std::string get_data();
	void str_to_type_id(std::string str);
	void set_data(std::string data);
#endif
	std::vector<size_t> offsets; // The memory offsets to get to the data member
	std::type_info* type;
	void update_data();
	template <typename T> void put_data(T data);
};

template <typename T> void data_member::put_data(T data) {
	if (!handle)
		throw errors::types::handle_not_set;
	size_t offset;
	for (auto a : offsets) {
		offset += a;
#ifdef _MSC_VER
		size_t nBytes;
		ReadProcessMemory(
			reinterpret_cast<void*>(handle), reinterpret_cast<void*>(offset),
			reinterpret_cast<void*>(&offset), sizeof(size_t), &nBytes);
		if (nBytes != sizeof(size_t))
			throw errors::types::invalid_memory;
#else
#endif
	}
#ifdef _MSC_VER
	size_t nBytes;
	WriteProcessMemory(reinterpret_cast<void*>(handle),
					   reinterpret_cast<void*>(offset),
					   reinterpret_cast<void*>(&data), sizeof T, &nBytes);
	if (nBytes != sizeof T)
		throw errors::types::invalid_memory;
#else
#endif
}

#endif
