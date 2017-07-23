#include "data_member.hpp"

#ifdef _MSC_VER
#include "Snippets\defer.hpp"
#include <Windows.h>
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#ifdef _MSC_VER
#include <tlhelp32.h>

size_t get_process_by_name(std::wstring name) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	std::string s(name.begin(), name.end());

	HANDLE snapshot = CreateToolhelp32Snapshot(2, NULL);

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (stricmp(entry.szExeFile, s.c_str()) == 0) {
				return reinterpret_cast<size_t>(OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID));
			}
		}
	}
 
	return 0;
}
#else
size_t get_process_by_name(std::string name) {}
#endif

#ifdef _MSC_VER
size_t get_process_by_window(std::wstring name) {
	size_t handle;
	auto window_handle = FindWindowW(NULL, name.c_str());
	if (window_handle == 0)
		handle = 0;
	else {
		auto pid = GetWindowThreadProcessId(window_handle, NULL);
		handle = reinterpret_cast<size_t>(
			OpenProcess(PROCESS_ALL_ACCESS, false, pid));
	}
	return handle;
}
#else
size_t get_process_by_window(std::string name) {}
#endif

#ifdef _MSC_VER
void data_member::str_to_type_id(std::wstring str) {
	if (str == L"string")
		type = const_cast<std::type_info*>(&typeid(char*));
	if (str == L"int" || str == L"integer")
		type = const_cast<std::type_info*>(&typeid(int));
	if (str == L"long")
		type = const_cast<std::type_info*>(&typeid(long));
	if (str == L"float")
		type = const_cast<std::type_info*>(&typeid(float));
	if (str == L"double")
		type = const_cast<std::type_info*>(&typeid(double));
	if (str == L"bool" || str == L"boolean")
		type = const_cast<std::type_info*>(&typeid(bool));
#else
void data_member::str_to_type_id(std::string str) {
	if (str == "string")
		type = const_cast<std::type_info*>(&typeid(char*));
	if (str == "int" || str == "integer")
		type = const_cast<std::type_info*>(&typeid(int));
	if (str == "long")
		type = const_cast<std::type_info*>(&typeid(long));
	if (str == "float")
		type = const_cast<std::type_info*>(&typeid(float));
	if (str == "double")
		type = const_cast<std::type_info*>(&typeid(double));
	if (str == "bool" || str == "boolean")
		type = const_cast<std::type_info*>(&typeid(bool));
#endif
}

#ifdef _MSC_VER
std::wstring data_member::get_type() {
	if (*type == typeid(char*))
		return L"string";
	if (*type == typeid(int))
		return L"int";
	if (*type == typeid(long))
		return L"long";
	if (*type == typeid(float))
		return L"float";
	if (*type == typeid(double))
		return L"double";
	if (*type == typeid(bool))
		return L"bool";
#else
std::string data_member::get_type() {
	if (*type == typeid(char*))
		return "string";
	if (*type == typeid(int))
		return "int";
	if (*type == typeid(long))
		return "long";
	if (*type == typeid(float))
		return "float";
	if (*type == typeid(double))
		return "double";
	if (*type == typeid(bool))
		return "bool";
#endif
	throw errors::types::type_not_found;
}

#ifdef _MSC_VER
std::wstring data_member::get_data() {
	if (*type == typeid(char*)) {
		std::wstring ret(std::strlen(data.str)+1, L'#');
		mbstowcs(&ret[0], data.str, std::strlen(data.str)+1);
		return ret;
	}
	if (*type == typeid(int))
		return std::to_wstring(data.integer);
	if (*type == typeid(long))
		return std::to_wstring(data.long_int);
	if (*type == typeid(float))
		return std::to_wstring(data.floating);
	if (*type == typeid(double))
		return std::to_wstring(data.prec);
	if (*type == typeid(bool))
		return (data.boolean? L"true" : L"false"); 
#else
std::string data_member::get_data() {
	if (*type == typeid(char*))
		return (data.str);
	if (*type == typeid(int))
		return std::to_string(data.integer);
	if (*type == typeid(long))
		return std::to_string(data.long_int);
	if (*type == typeid(float))
		return std::to_string(data.floating);
	if (*type == typeid(double))
		return std::to_string(data.prec);
	if (*type == typeid(bool))
		return (data.boolean? "true" : "false"); 
#endif
	throw errors::types::type_not_found;
}

size_t data_member::get_offset(int recursion_level) { 
	size_t offset = 0;
	for (auto i = 0; i < offsets.size() - 1; ++i) {
		offset += offsets[i];
#ifdef _MSC_VER
		size_t nBytes = 0;
		if (!ReadProcessMemory(reinterpret_cast<void*>(handle),
								reinterpret_cast<void*>(offset),
								reinterpret_cast<void*>(&offset),
								sizeof(size_t), &nBytes)) {
			if (GetLastError() == ERROR_INVALID_HANDLE) {
				actual_offset = 0;
				if (process_name.length() > 0)
					handle = get_process_by_name(process_name);
				if (window_name.length() > 0)
					handle = get_process_by_window(window_name);
				if (recursion_level > 5)
					throw errors::types::cant_get_handle;
				return get_offset(++recursion_level);
			}
		}
		if (nBytes != sizeof(size_t))
			throw errors::types::invalid_memory;
#else
#endif
	}
	offset += offsets[offsets.size() - 1];
	return offset;
}

#ifdef _MSC_VER
void data_member::set_data(std::wstring data) {
#else
void data_member::set_data(std::string data) {
#endif
	if (*type == typeid(char*)) {
#ifdef _MSC_VER
		char* tempcstr = new char[data.length() * 2];
		defer { delete[] tempcstr; };
		std::wcstombs(tempcstr, data.c_str(), data.length() * 2);
		this->data.str = tempcstr;
#else
		this->data.str = const_cast<char*>(data.c_str());
#endif
		if (handle)
			put_data(this->data.str);
		return;
	}
	if (*type == typeid(int)) {
		this->data.integer = std::stoi(data);
		if (handle)
			put_data(this->data.integer);
		return;
	}
	if (*type == typeid(long)) {
		this->data.long_int = std::stol(data);
		if (handle)
			put_data(this->data.long_int);
		return;
	}
	if (*type == typeid(float)) {
		this->data.floating = std::stof(data);
		if (handle)
			put_data(this->data.floating);
		return;
	}
	if (*type == typeid(double)) {
		this->data.prec = std::stod(data);
		if (handle)
			put_data(this->data.prec);
		return;
	}
	if (*type == typeid(bool)) {
#ifdef _MSC_VER
		if (data != L"true" && data != L"false")
			throw errors::types::bool_incorrect_input;
		this->data.boolean = data == L"true";
#else
		if (data != "true" && data != "false")
			throw errors::types::bool_incorrect_input;
		this->data.boolean = data == "true";
#endif
		if (handle)
			put_data(this->data.boolean);
		return;
	}
	throw errors::types::type_not_found;
}

// TODO: Make this function work without injection.
void data_member::update_data() {
	if (!actual_offset)
		actual_offset = get_offset();
	if (!handle)
		throw errors::types::handle_not_set;
	for (int i = 0; i < 5; ++i) { 
		if (*type == typeid(char*)) {
#ifdef _MSC_VER
			size_t success = 0;
			ReadProcessMemory(reinterpret_cast<void*>(handle),
				reinterpret_cast<void*>(actual_offset), &this->data.str,
				sizeof(char*), &success);
			if (success == sizeof(char*))
				return;
			actual_offset = get_offset();
#else
			this->data.str = reinterpret_cast<char*>(offset);
#endif
		}
		if (*type == typeid(int)) {
#ifdef _MSC_VER
			size_t nBytes = 0;
			ReadProcessMemory(reinterpret_cast<void*>(handle),
				reinterpret_cast<void*>(actual_offset), &this->data.integer,
				sizeof(int), &nBytes);
			if (nBytes == sizeof(int))
				return;
			actual_offset = get_offset();
#else
			this->data.integer = *reinterpret_cast<int*>(offset);
#endif
		}
		if (*type == typeid(long)) {
#ifdef _MSC_VER
			size_t nBytes = 0;
			ReadProcessMemory(reinterpret_cast<void*>(handle),
				reinterpret_cast<void*>(actual_offset), &this->data.long_int,
				sizeof(long), &nBytes);
			if (nBytes == sizeof(long))
				return;
			actual_offset = get_offset();
#else
			this->data.long_int = *reinterpret_cast<long*>(offset);
#endif
		}
		if (*type == typeid(float)) {
#ifdef _MSC_VER
			size_t nBytes = 0;
			ReadProcessMemory(reinterpret_cast<void*>(handle),
				reinterpret_cast<void*>(actual_offset), &this->data.floating,
				sizeof(float), &nBytes);
			if (nBytes == sizeof(float))
				return;
			actual_offset = get_offset();
#else
			this->data.floating = *reinterpret_cast<float*>(offset);
#endif
		}
		if (*type == typeid(double)) {
#ifdef _MSC_VER
			size_t nBytes = 0;
			ReadProcessMemory(reinterpret_cast<void*>(handle),
				reinterpret_cast<void*>(actual_offset), &this->data.prec,
				sizeof(double), &nBytes);
			if (nBytes == sizeof(double))
				return;
			actual_offset = get_offset();
#else
			this->data.prec = *reinterpret_cast<double*>(offset);
#endif
		}
		if (*type == typeid(bool)) {
#ifdef _MSC_VER
			size_t nBytes = 0;
			ReadProcessMemory(reinterpret_cast<void*>(handle),
				reinterpret_cast<void*>(actual_offset), &this->data.boolean,
				sizeof(bool), &nBytes);
			if (nBytes == sizeof(bool))
				return;
			actual_offset = get_offset();
#else
			this->data.boolean = *reinterpret_cast<bool*>(offset);
#endif
		}
	} 
	throw errors::types::invalid_memory;
}