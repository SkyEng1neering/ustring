#ifndef USTRING_H
#define USTRING_H

#include "uvector.h"

#define MIN_STRING_RESERVE    5

class ustring
{
private:
    uvector<char> ch_container;
    heap_t *alloc_mem_ptr;

public:
    ustring::ustring(uint32_t _size, heap_t *_alloc_mem_ptr);
    ustring::ustring(heap_t *_alloc_mem_ptr);
    ustring::ustring(const char *str, heap_t *_alloc_mem_ptr);
    ustring::ustring(const ustring &string);
    ustring::ustring();
    ustring::~ustring();
    ustring& ustring::operator = (const ustring &string);

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
	ustring ustring::operator + (ustring &str);
	ustring ustring::operator + (const char *str);
	bool assign(const char *str);
	bool assign(char *str, uint32_t str_len);
	bool assign(ustring str);
	bool ustring::assign(const char *str, uint32_t str_len);
	heap_t* get_mem_pointer() const;
	void assign_mem_pointer(heap_t *mem_ptr);
	void clear();
};

#endif // USTRING_H
