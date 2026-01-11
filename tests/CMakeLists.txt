cmake_minimum_required(VERSION 3.14)
project(ustring_tests)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Compiler flags
add_compile_options(-Wall -Wextra -g)

# Sanitizers (optional but recommended)
option(USE_SANITIZERS "Enable ASan/UBSan" ON)
if(USE_SANITIZERS AND NOT WIN32)
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()

# ==================== Multi-Heap Mode Tests ====================

# dalloc library for multi-heap mode
add_library(dalloc_multiheap STATIC ../../dalloc/src/dalloc.c)
target_include_directories(dalloc_multiheap PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../dalloc/inc
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(dalloc_multiheap PUBLIC
    DALLOC_CUSTOM_CONF_FILE="ustring_test_conf.h"
)

# ustring library for multi-heap mode
add_library(ustring_multiheap STATIC ../src/ustring.cpp)
target_include_directories(ustring_multiheap PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../uvector/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../dalloc/inc
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(ustring_multiheap PUBLIC dalloc_multiheap)
target_compile_definitions(ustring_multiheap PUBLIC
    DALLOC_CUSTOM_CONF_FILE="ustring_test_conf.h"
)

# Multi-heap test sources
set(MULTIHEAP_TEST_SOURCES
    test_basic.cpp
    test_element_access.cpp
    test_capacity.cpp
    test_modifiers.cpp
    test_operators.cpp
    test_copy_semantics.cpp
    test_edge_cases.cpp
)

# Multi-heap test executable
add_executable(ustring_tests ${MULTIHEAP_TEST_SOURCES})
target_link_libraries(ustring_tests
    ustring_multiheap
    GTest::gtest_main
)
target_include_directories(ustring_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../uvector/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../dalloc/inc
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ==================== Single-Heap Mode Tests ====================

# dalloc library for single-heap mode
add_library(dalloc_singleheap STATIC ../../dalloc/src/dalloc.c)
target_include_directories(dalloc_singleheap PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../dalloc/inc
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(dalloc_singleheap PUBLIC
    USE_SINGLE_HEAP_MEMORY
    DALLOC_CUSTOM_CONF_FILE="ustring_test_conf.h"
)

# ustring library for single-heap mode
add_library(ustring_singleheap STATIC ../src/ustring.cpp)
target_include_directories(ustring_singleheap PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../uvector/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../dalloc/inc
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(ustring_singleheap PUBLIC dalloc_singleheap)
target_compile_definitions(ustring_singleheap PUBLIC
    USE_SINGLE_HEAP_MEMORY
    DALLOC_CUSTOM_CONF_FILE="ustring_test_conf.h"
)

# Single-heap test executable
add_executable(ustring_single_heap_tests test_single_heap.cpp)
target_link_libraries(ustring_single_heap_tests
    ustring_singleheap
    GTest::gtest_main
)
target_include_directories(ustring_single_heap_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../uvector/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../dalloc/inc
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(ustring_single_heap_tests PRIVATE
    USE_SINGLE_HEAP_MEMORY
)

# Enable CTest integration
include(GoogleTest)
gtest_discover_tests(ustring_tests)
gtest_discover_tests(ustring_single_heap_tests)
