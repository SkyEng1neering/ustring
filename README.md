# ustring

Dynamic string class for embedded systems using [dalloc](https://github.com/SkyEng1neering/dalloc) memory allocator.

**Version:** 1.3.0
**License:** Apache 2.0
**Author:** Alexey Vasilenko

## Features

- Dynamic string with automatic memory management
- Built on top of [uvector](https://github.com/SkyEng1neering/uvector)
- Two modes: multi-heap (explicit heap pointer) and single-heap (global default)
- STL-like interface (`at`, `c_str`, `append`, `resize`, `operator+`, etc.)
- Memory defragmentation support via dalloc
- No memory fragmentation issues for long-running embedded applications

## Dependencies

ustring requires:
- [uvector](https://github.com/SkyEng1neering/uvector) v1.3.0+
- [dalloc](https://github.com/SkyEng1neering/dalloc) v1.5.0+

## API Reference

### Constructors

| Constructor | Description |
|-------------|-------------|
| `ustring()` | Default constructor (single-heap mode only) |
| `ustring(heap_t* heap)` | Constructor with heap pointer (multi-heap mode) |
| `ustring(uint32_t size, heap_t* heap)` | Constructor with initial size |
| `ustring(const char* str, heap_t* heap)` | Constructor from C string |
| `ustring(const ustring& other)` | Copy constructor |

### Element Access

| Method | Description |
|--------|-------------|
| `char& at(uint32_t i)` | Access character with bounds checking |
| `char& operator[](uint32_t i)` | Access character without bounds checking |
| `char& front()` | Access first character |
| `char& back()` | Access last character |
| `char* data()` | Direct access to underlying array |
| `const char* c_str()` | Get null-terminated C string |

### Capacity

| Method | Description |
|--------|-------------|
| `bool empty()` | Check if string is empty |
| `uint32_t size()` | Return string length (excluding null terminator) |
| `uint32_t length()` | Same as size() |
| `uint32_t capacity()` | Return current capacity |
| `bool reserve(uint32_t n)` | Reserve capacity for n characters |
| `bool shrink_to_fit()` | Reduce capacity to match size |

### Modifiers

| Method | Description |
|--------|-------------|
| `bool push_back(char c)` | Append character |
| `bool pop_back()` | Remove last character |
| `bool append(const char* str)` | Append C string |
| `bool append(const ustring& str)` | Append ustring |
| `bool append(char c)` | Append character |
| `void clear()` | Remove all characters |
| `bool resize(uint32_t n)` | Change string size |
| `bool resize(uint32_t n, char c)` | Change size with fill character |
| `bool assign(const char* str)` | Replace content with C string |
| `bool assign(const ustring& str)` | Replace content with ustring |

### Operators

| Operator | Description |
|----------|-------------|
| `operator=` | Assignment |
| `operator+=` | Append |
| `operator+` | Concatenation |
| `operator==` | Equality comparison |
| `operator!=` | Inequality comparison |

## Usage

### Single-heap mode

Enable single-heap mode via compiler flags or custom config file:

```bash
gcc -DUSE_SINGLE_HEAP_MEMORY ...
```

```cpp
#include "ustring.h"

__attribute__((aligned(4)))
static uint8_t heap_buffer[4096];

int main() {
    if (!dalloc_register_heap(heap_buffer, sizeof(heap_buffer))) {
        return -1;
    }

    ustring str1("Hello");
    ustring str2(" World");
    ustring result = str1 + str2;

    printf("Result: %s\n", result.c_str());  // "Hello World"

    return 0;
}
```

### Multi-heap mode

```cpp
#include "ustring.h"

__attribute__((aligned(4)))
static uint8_t heap_buffer[4096];
heap_t heap;

int main() {
    heap_init(&heap, heap_buffer, sizeof(heap_buffer));

    ustring str1("Hello", &heap);
    ustring str2(" World", &heap);

    str1 += str2;
    printf("Result: %s\n", str1.c_str());  // "Hello World"

    heap_deinit(&heap);
    return 0;
}
```

### Using with uvector

```cpp
#include "uvector.h"
#include "ustring.h"

uvector<ustring> strings(&heap);

strings.push_back(ustring("First", &heap));
strings.push_back(ustring("Second", &heap));

for (uint32_t i = 0; i < strings.size(); i++) {
    printf("String %u: %s\n", i, strings.at(i).c_str());
}
```

## Testing

ustring includes comprehensive unit tests using Google Test:

```bash
cd libs/ustring/tests
mkdir build && cd build
cmake ..
make -j$(nproc)
./ustring_tests              # Multi-heap tests (139 tests)
./ustring_single_heap_tests  # Single-heap tests (28 tests)
```

Tests cover:
- Basic operations (construction, assignment, access)
- String manipulation (append, resize, clear)
- Operators (+, +=, ==, !=)
- Copy semantics (copy constructor, assignment)
- Edge cases (empty string, single character)
- Stress tests (rapid operations, long strings)

All tests run with AddressSanitizer and UndefinedBehaviorSanitizer enabled.

## License

Apache License 2.0. See [LICENSE](LICENSE) for details.
