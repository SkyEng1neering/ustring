/**
 * @file ustring.h
 * @brief Dynamic string class for embedded systems using dalloc memory allocator
 *
 * ustring is a lightweight std::string alternative designed for embedded systems.
 * It uses uvector<char> internally and the dalloc defragmenting memory allocator
 * for dynamic memory management.
 *
 * Features:
 * - Dynamic string with automatic memory management
 * - Null-terminated C string compatibility via c_str()
 * - Two modes: multi-heap (explicit heap pointer) and single-heap (global default)
 * - STL-like interface (at, append, resize, operator+, etc.)
 *
 * @note Requires uvector and dalloc libraries
 * @note Define USE_SINGLE_HEAP_MEMORY to use single global heap mode
 *
 * @author Alexey Vasilenko
 * @copyright 2021 Alexey Vasilenko, Apache License 2.0
 */

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

/** @brief Library version string */
#define USTRING_VERSION         "1.3.0"

/** @brief Minimum string capacity reservation */
#define MIN_STRING_RESERVE      5

/**
 * @brief Dynamic string class using dalloc memory allocator
 *
 * ustring provides a dynamic string similar to std::string but designed for
 * embedded systems with custom memory allocation via dalloc.
 *
 * Internally stores characters in uvector<char> with automatic null termination.
 * The size() method returns the string length excluding the null terminator.
 *
 * Example usage (multi-heap mode):
 * @code
 * uint8_t buffer[4096];
 * heap_t heap;
 * heap_init(&heap, buffer, 4096);
 *
 * ustring str("Hello", &heap);
 * str += " World";
 * printf("%s\n", str.c_str());  // Output: Hello World
 * @endcode
 *
 * Example usage (single-heap mode, requires USE_SINGLE_HEAP_MEMORY):
 * @code
 * ustring str("Hello");
 * str += " World";
 * @endcode
 */
class ustring
{
private:
    uvector<char> ch_container;  ///< Internal character storage (null-terminated)

public:
    // ==================== Constructors & Destructor ====================

    /**
     * @brief Default constructor
     *
     * Creates an empty string. In multi-heap mode, heap must be assigned
     * via assign_mem_pointer() before use.
     */
    ustring();

#ifdef USE_SINGLE_HEAP_MEMORY
    /**
     * @brief Constructor with initial size (single-heap mode)
     * @param _size Initial string size to allocate
     */
    ustring(uint32_t _size);

    /**
     * @brief Constructor from C string (single-heap mode)
     * @param str Null-terminated C string to copy
     */
    ustring(const char *str);
#else
    /**
     * @brief Constructor with heap pointer (multi-heap mode)
     * @param _alloc_mem_ptr Pointer to heap for memory allocation
     */
    ustring(heap_t *_alloc_mem_ptr);

    /**
     * @brief Constructor with initial size and heap pointer (multi-heap mode)
     * @param _size Initial string size to allocate
     * @param _alloc_mem_ptr Pointer to heap for memory allocation
     */
    ustring(uint32_t _size, heap_t *_alloc_mem_ptr);

    /**
     * @brief Constructor from C string with heap pointer (multi-heap mode)
     * @param str Null-terminated C string to copy
     * @param _alloc_mem_ptr Pointer to heap for memory allocation
     */
    ustring(const char *str, heap_t *_alloc_mem_ptr);

    /**
     * @brief Assign heap pointer for memory allocation
     * @param mem_ptr Pointer to heap structure
     *
     * Must be called before any string operations in multi-heap mode
     * if default constructor was used.
     */
    void assign_mem_pointer(heap_t *mem_ptr);
#endif

    /**
     * @brief Copy constructor
     * @param string Source string to copy
     *
     * Creates a deep copy of the source string. Inherits heap pointer from source.
     *
     * @note If memory allocation fails during copy, the string may be partially
     *       copied. This is a design limitation for embedded systems without exceptions.
     *       Ensure sufficient heap space before copying large strings.
     */
    ustring(const ustring &string);

    /**
     * @brief Destructor
     *
     * Frees all allocated memory.
     */
    ~ustring();

    /**
     * @brief Copy assignment operator
     * @param string Source string to copy
     * @return Reference to this string
     *
     * Replaces contents with a copy of string. Inherits heap pointer from source.
     *
     * @note If memory allocation fails during copy, the string may be partially
     *       copied. Ensure sufficient heap space before assigning large strings.
     */
    ustring& operator = (const ustring &string);

    // ==================== Element Access ====================

    /**
     * @brief Access character at index
     * @param i Index of character to access
     * @return Reference to character at index i
     * @warning No bounds checking. Undefined behavior if i >= size().
     */
    char& at(uint32_t i);

    /**
     * @brief Access character at index (alias for at())
     * @param i Index of character to access
     * @return Reference to character at index i
     * @see at()
     */
    char& operator[](uint32_t i);

    /**
     * @brief Access first character
     * @return Reference to first character
     * @warning Undefined behavior if string is empty
     */
    char& front();

    /**
     * @brief Access last character (before null terminator)
     * @return Reference to last character
     * @warning Undefined behavior if string is empty
     */
    char& back();

    /**
     * @brief Get pointer to character data
     * @return Pointer to internal character array
     */
    char* data() const;

    /**
     * @brief Get null-terminated C string
     * @return Pointer to null-terminated character array
     *
     * Equivalent to data(). Guaranteed to be null-terminated.
     */
    const char* c_str() const;

    // ==================== Capacity ====================

    /**
     * @brief Check if string is empty
     * @return true if size() == 0, false otherwise
     */
    bool empty();

    /**
     * @brief Get string length
     * @return Number of characters (excluding null terminator)
     */
    uint32_t size() const;

    /**
     * @brief Get string length (alias for size())
     * @return Number of characters (excluding null terminator)
     * @see size()
     */
    uint32_t length() const;

    /**
     * @brief Reserve capacity for at least new_string_size characters
     * @param new_string_size Minimum capacity to reserve
     * @return true if successful, false if allocation failed
     */
    bool reserve(uint32_t new_string_size);

    /**
     * @brief Get current capacity
     * @return Number of characters that can be held without reallocation
     */
    uint32_t capacity();

    /**
     * @brief Reduce capacity to match size
     * @return true if successful, false if reallocation failed
     */
    bool shrink_to_fit();

    // ==================== Modifiers ====================

    /**
     * @brief Remove all characters
     *
     * Sets size to 0. Capacity remains unchanged.
     */
    void clear();

    /**
     * @brief Append single character
     * @param item Character to append
     * @return true if successful, false if allocation failed
     */
    bool push_back(char item);

    /**
     * @brief Remove last character
     * @return true if character was removed, false if string was empty
     */
    bool pop_back();

    /**
     * @brief Append null-terminated C string
     * @param str C string to append
     * @return true if successful, false if allocation failed or str is empty
     */
    bool append(const char *str);

    /**
     * @brief Append character array with specified length
     * @param str Pointer to character array
     * @param str_len Number of characters to append
     * @return true if successful, false if allocation failed
     */
    bool append(char *str, uint32_t str_len);

    /**
     * @brief Append character array with specified length (const version)
     * @param str Pointer to character array
     * @param str_len Number of characters to append
     * @return true if successful, false if allocation failed
     */
    bool append(const char *str, uint32_t str_len);

    /**
     * @brief Append another ustring
     * @param str String to append
     * @return true if successful, false if allocation failed
     *
     * Pre-reserves capacity to avoid pointer invalidation during append.
     */
    bool append(const ustring& str);

    /**
     * @brief Append single character (alias for push_back)
     * @param ch Character to append
     * @return true if successful, false if allocation failed
     */
    bool append(char ch);

    /**
     * @brief Append C string operator
     * @param str C string to append
     * @return true if successful, false if allocation failed
     */
    bool operator+=(const char *str);

    /**
     * @brief Append ustring operator
     * @param str String to append
     * @return true if successful, false if allocation failed
     */
    bool operator+=(const ustring& str);

    /**
     * @brief Append character operator
     * @param ch Character to append
     * @return true if successful, false if allocation failed
     */
    bool operator+=(char ch);

    /**
     * @brief Resize string to new_str_size characters
     * @param new_str_size New string length
     * @return true if successful, false if allocation failed
     *
     * If shrinking, characters are removed from end.
     * If growing, new positions are filled with null characters.
     */
    bool resize(uint32_t new_str_size);

    /**
     * @brief Resize string and fill new positions with value
     * @param new_str_size New string length
     * @param value Character to fill new positions with
     * @return true if successful, false if allocation failed
     */
    bool resize(uint32_t new_str_size, char value);

    /**
     * @brief Replace content with C string
     * @param str C string to assign
     * @return true if successful, false if allocation failed or str is empty
     */
    bool assign(const char *str);

    /**
     * @brief Replace content with character array
     * @param str Pointer to character array
     * @param str_len Number of characters to assign
     * @return true if successful, false if allocation failed
     */
    bool assign(char *str, uint32_t str_len);

    /**
     * @brief Replace content with character array (const version)
     * @param str Pointer to character array
     * @param str_len Number of characters to assign
     * @return true if successful, false if allocation failed
     */
    bool assign(const char *str, uint32_t str_len);

    /**
     * @brief Replace content with another ustring
     * @param str String to assign
     * @return true if successful, false if allocation failed
     */
    bool assign(const ustring& str);

    // ==================== Comparison Operators ====================

    /**
     * @brief Equality comparison (const version)
     * @param name String to compare with
     * @return true if strings are equal, false otherwise
     */
    bool operator==(const ustring & name) const;

    /**
     * @brief Equality comparison
     * @param str String to compare with
     * @return true if strings are equal, false otherwise
     */
    bool operator==(ustring &str) const;

    /**
     * @brief Inequality comparison (const version)
     * @param name String to compare with
     * @return true if strings are not equal, false otherwise
     */
    bool operator!=(const ustring & name) const;

    /**
     * @brief Inequality comparison
     * @param name String to compare with
     * @return true if strings are not equal, false otherwise
     */
    bool operator!=(ustring &name) const;

    // ==================== Concatenation Operators ====================

    /**
     * @brief Concatenate with another ustring
     * @param str String to concatenate
     * @return New string containing concatenation result
     */
    ustring operator + (ustring &str);

    /**
     * @brief Concatenate with C string
     * @param str C string to concatenate
     * @return New string containing concatenation result
     */
    ustring operator + (const char *str);

    // ==================== Utility ====================

    /**
     * @brief Get heap pointer used for allocation
     * @return Pointer to heap_t structure, or nullptr
     */
    heap_t* get_mem_pointer() const;
};

#endif // USTRING_H
