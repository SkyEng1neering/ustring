/**
 * @file ustring_test_conf.h
 * @brief dalloc configuration for ustring unit tests
 */

#ifndef USTRING_TEST_CONF_H
#define USTRING_TEST_CONF_H

// Maximum allocations for tests
#define MAX_NUM_OF_ALLOCATIONS      100

// Enable alignment
#define USE_ALIGNMENT
#define ALLOCATION_ALIGNMENT_BYTES  4

// Zero memory on free for better debugging
#define FILL_FREED_MEMORY_BY_NULLS  true

// Disable debug output during tests
#define dalloc_debug(...)

#endif // USTRING_TEST_CONF_H
