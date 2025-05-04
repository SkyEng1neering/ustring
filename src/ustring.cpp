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

#include "ustring.h"

char& ustring::at(uint32_t i){
	return ch_container.at(i);
}

char& ustring::operator[](uint32_t i){
	return at(i);
}

char& ustring::front(){
	return ch_container.front();
}

char& ustring::back(){
	return ch_container.back();
}

char* ustring::data() const{
	return ch_container.data();
}

const char* ustring::c_str() const{
	return data();
}

bool ustring::empty(){
	return ch_container.empty();
}

uint32_t ustring::size() const{
	if(ch_container.size() > 0){
		return ch_container.size() - 1;//size without null terminate symbol
	}
	return ch_container.size();
}

uint32_t ustring::length() const{
	return size();
}

bool ustring::reserve(uint32_t new_string_size){
	return ch_container.reserve(new_string_size + 1);//+ null terminate symbol
}

uint32_t ustring::capacity(){
	return ch_container.capacity();
}

bool ustring::shrink_to_fit(){
	return ch_container.shrink_to_fit();
}

void ustring::clear(){
	ch_container.clear();
}

bool ustring::push_back(char item){
	if(ch_container.size() > 0){
		if(ch_container.pop_back() != true){//remove null terminate symbol
			return false;
		}
	}
	if(ch_container.push_back(item) != true){
		return false;
	}
	return ch_container.push_back(0);//add null terminate symbol
}

bool ustring::pop_back(){
	if(size() == 0){
		return false;
	}

	if(ch_container.pop_back() != true){//remove null terminate symbol
		return false;
	}

	if(ch_container.pop_back() != true){//remove last string symbol
		return false;
	}

	return ch_container.push_back(0);//add null terminate symbol
}

bool ustring::append(const char *str){
	uint32_t ind = 0;
	if(str[ind] == '\0'){
		return false;
	}
	while(str[ind] != '\0'){
		push_back(str[ind]);
		ind++;
	}
	return true;
}

bool ustring::append(char *str, uint32_t str_len){
	for(uint32_t i = 0; i < str_len; i++){
		if(push_back(str[i]) != true){
			return false;
		}
	}
	return true;
}

bool ustring::append(const char *str, uint32_t str_len){
	return append(const_cast<char*>(str), str_len);
}

bool ustring::append(ustring str){
	uint32_t str_len = str.size();
	return append(str.c_str(), str_len);
}

bool ustring::append(char ch){
	return push_back(ch);
}

bool ustring::operator+=(const char *str){
	return append(str);
}

bool ustring::operator+=(ustring str){
	return append(str);
}

bool ustring::operator+=(char ch){
	return append(ch);
}

bool ustring::operator==(const ustring &str) const {
	if (size() != str.size()) {
		return false;
	}
	if (strncmp(ch_container.data(), str.data(), str.size()) != 0) {
		return false;
	}
	return true;
}

bool ustring::operator==(ustring &str) const {
	if (size() != str.size()) {
		return false;
	}
	if (strncmp(ch_container.data(), str.data(), str.size()) != 0) {
		return false;
	}
	return true;
}

bool ustring::operator!=(const ustring &name) const {
	return !(*this == name);
}

bool ustring::operator!=(ustring &name) const {
	return !(*this == name);
}

bool ustring::resize(uint32_t new_str_size){
	return resize(new_str_size, 0);
}

bool ustring::resize(uint32_t new_str_size, char value){
	if(size() == new_str_size){
		return true;
	}
	if(size() > new_str_size){
		for(uint32_t i = 0; i < size() - new_str_size; i++){
			pop_back();
		}
		return true;
	}

	if(capacity() > new_str_size){
		if(ch_container.resize(new_str_size + 1, value) != true){
			return false;
		}
		ch_container.back() = '\0';
		if(ch_container.pop_back() != true){
			return false;
		}
		return true;
	}
	if(reserve(new_str_size) != true){
		return false;
	}
	if(ch_container.resize(new_str_size + 1, value) != true){
		return false;
	}
	ch_container.back() = '\0';
	if(ch_container.pop_back() != true){
		return false;
	}

	return true;
}

bool ustring::assign(const char *str){
	uint32_t ind = 0;
	if(str[ind] == '\0'){
		return false;
	}
	ch_container.clear();
	while(str[ind] != '\0'){
		if(push_back(str[ind]) != true){
			return false;
		}
		ind++;
	}
	return true;
}

bool ustring::assign(char *str, uint32_t str_len){
	ch_container.clear();
	for(uint32_t i = 0; i < str_len; i++){
		if(push_back(str[i]) != true){
			return false;
		}
	}
	return true;
}

bool ustring::assign(const char *str, uint32_t str_len){
	return assign(const_cast<char*>(str), str_len);
}

bool ustring::assign(ustring str){
	uint32_t str_len = str.size();
	ch_container.clear();
	return assign(str.c_str(), str_len);
}

heap_t* ustring::get_mem_pointer() const{
    return ch_container.get_mem_pointer();
}

#ifdef USE_SINGLE_HEAP_MEMORY
ustring::ustring(uint32_t _size){
    resize(_size);
}

//TODO: reserve memory before assign string
ustring::ustring(const char *str){
    assign(str);
}

ustring::ustring(const ustring &string){
    for(uint32_t i = 0; i < string.size(); i++){
        this->push_back(string.data()[i]);
    }
}

ustring& ustring::operator = (const ustring &string){
    if(&string != this){
        for(uint32_t i = 0; i < string.size(); i++){
            this->push_back(string.data()[i]);
        }
    }
    return *this;
}

ustring ustring::operator + (ustring &str){
    uint32_t self_str_len = this->size();
    uint32_t new_str_len = str.size();
    ustring new_string(self_str_len + new_str_len);
    new_string.clear();

    for(uint32_t i = 0; i < self_str_len; i++){
        new_string.push_back(this->at(i));
    }
    for(uint32_t i = 0; i < new_str_len; i++){
        new_string.push_back(str.at(i));
    }
    return new_string;
}

ustring ustring::operator + (const char *str){
    uint32_t self_str_len = this->size();
    uint32_t new_str_len = strlen(str);

    ustring new_string(self_str_len + new_str_len);
    new_string.clear();

    for(uint32_t i = 0; i < self_str_len; i++){
        new_string.push_back(this->at(i));
    }
    for(uint32_t i = 0; i < new_str_len; i++){
        new_string.push_back(str[i]);
    }
    return new_string;
}
#else
void ustring::assign_mem_pointer(heap_t *mem_ptr){
    ch_container.assign_mem_pointer(mem_ptr);
}

ustring::ustring(uint32_t _size, heap_t *_alloc_mem_ptr){
    ch_container.assign_mem_pointer(_alloc_mem_ptr);
    resize(_size);
}

ustring::ustring(heap_t *_alloc_mem_ptr){
    ch_container.assign_mem_pointer(_alloc_mem_ptr);
}

ustring::ustring(const char *str, heap_t *_alloc_mem_ptr){
    ch_container.assign_mem_pointer(_alloc_mem_ptr);
    assign(str);
}

ustring::ustring(const ustring &string){
    this->assign_mem_pointer(string.get_mem_pointer());
    for(uint32_t i = 0; i < string.size(); i++){
        this->push_back(string.data()[i]);
    }
}

ustring& ustring::operator = (const ustring &string){
    if(&string != this){
        this->assign_mem_pointer(string.get_mem_pointer());
        for(uint32_t i = 0; i < string.size(); i++){
            this->push_back(string.data()[i]);
        }
    }
    return *this;
}

ustring ustring::operator + (ustring &str){
    uint32_t self_str_len = this->size();
    uint32_t new_str_len = str.size();
    ustring new_string(self_str_len + new_str_len, this->ch_container.get_mem_pointer());
    new_string.clear();

    for(uint32_t i = 0; i < self_str_len; i++){
        new_string.push_back(this->at(i));
    }
    for(uint32_t i = 0; i < new_str_len; i++){
        new_string.push_back(str.at(i));
    }
    return new_string;
}

ustring ustring::operator + (const char *str){
    uint32_t self_str_len = this->size();
    uint32_t new_str_len = strlen(str);

    ustring new_string(self_str_len + new_str_len, this->ch_container.get_mem_pointer());
    new_string.clear();

    for(uint32_t i = 0; i < self_str_len; i++){
        new_string.push_back(this->at(i));
    }
    for(uint32_t i = 0; i < new_str_len; i++){
        new_string.push_back(str[i]);
    }
    return new_string;
}
#endif

ustring::ustring(){

}

ustring::~ustring(){

}
