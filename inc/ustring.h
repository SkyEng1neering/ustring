/*
 * Copyright 2021 Alexey Vasilenko
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef USTRING_H
#define USTRING_H

#include "uvector.h"

#define USTRING_VERSION			"1.2.0"

#define MIN_STRING_RESERVE		5

class ustring
{
private:
	uvector<char> ch_container;

public:
    ustring();
#ifdef USE_SINGLE_HEAP_MEMORY
    ustring(uint32_t _size);
    ustring(const char *str);
#else
    ustring(heap_t *_alloc_mem_ptr);
    ustring(uint32_t _size, heap_t *_alloc_mem_ptr);
    ustring(const char *str, heap_t *_alloc_mem_ptr);
    void assign_mem_pointer(heap_t *mem_ptr);
#endif
	ustring(const ustring &string);

	~ustring();
	ustring& operator = (const ustring &string);

	char& at(uint32_t i);
	char& operator[](uint32_t i);
	char& front();
	char& back();
	char* data() const;
	const char* c_str();
	bool empty();
	uint32_t size() const;
	uint32_t length() const;
	bool reserve(uint32_t new_string_size);
	uint32_t capacity();
	bool shrink_to_fit();
	void clear();
	bool push_back(char item);
	bool pop_back();
	bool append(const char *str);
	bool append(char *str, uint32_t str_len);
	bool append(const char *str, uint32_t str_len);
	bool append(ustring str);
	bool append(char ch);
	bool operator+=(const char *str);
	bool operator+=(ustring str);
	bool operator+=(char ch);
	bool resize(uint32_t new_str_size);
	bool resize(uint32_t new_str_size, char value);
	ustring operator + (ustring &str);
	ustring operator + (const char *str);
	bool assign(const char *str);
	bool assign(char *str, uint32_t str_len);
	bool assign(ustring str);
	bool assign(const char *str, uint32_t str_len);
	heap_t* get_mem_pointer() const;
};

#endif // USTRING_H
