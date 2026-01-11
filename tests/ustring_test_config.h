/**
 * @file ustring_test_config.h
 * @brief Test fixtures and helpers for ustring unit tests
 */

#ifndef USTRING_TEST_CONFIG_H
#define USTRING_TEST_CONFIG_H

#include <gtest/gtest.h>
#include <cstring>
#include <cstdint>

extern "C" {
#include "dalloc.h"
}

// Suppress debug output
#ifdef uvector_debug
#undef uvector_debug
#endif
#define uvector_debug(...)

#include "ustring.h"

// ==================== Test Configuration ====================

static constexpr size_t TEST_HEAP_SIZE = 4096;
static constexpr size_t TEST_LARGE_HEAP_SIZE = 16384;

// Canary configuration for memory corruption detection
static constexpr size_t CANARY_SIZE = 64;
static constexpr uint8_t CANARY_PATTERN = 0xCD;
static constexpr uint8_t UNINIT_PATTERN = 0xAA;

// ==================== Base Test Fixture ====================

/**
 * @brief Base fixture with canary-protected heap
 */
class UstringTestFixture : public ::testing::Test {
protected:
    uint8_t canary_before[CANARY_SIZE];
    alignas(4) uint8_t heap_memory[TEST_HEAP_SIZE];
    uint8_t canary_after[CANARY_SIZE];
    heap_t heap;

    void SetUp() override {
        std::memset(canary_before, CANARY_PATTERN, CANARY_SIZE);
        std::memset(canary_after, CANARY_PATTERN, CANARY_SIZE);
        std::memset(heap_memory, UNINIT_PATTERN, TEST_HEAP_SIZE);
        heap_init(&heap, heap_memory, TEST_HEAP_SIZE);
    }

    void TearDown() override {
        checkCanaryIntegrity();
        heap_deinit(&heap);
    }

    void checkCanaryIntegrity() {
        for (size_t i = 0; i < CANARY_SIZE; i++) {
            EXPECT_EQ(canary_before[i], CANARY_PATTERN)
                << "Buffer UNDERFLOW at canary_before[" << i << "]";
            EXPECT_EQ(canary_after[i], CANARY_PATTERN)
                << "Buffer OVERFLOW at canary_after[" << i << "]";
        }
    }
};

/**
 * @brief Large heap fixture for stress tests
 */
class UstringLargeHeapFixture : public ::testing::Test {
protected:
    uint8_t canary_before[CANARY_SIZE];
    alignas(4) uint8_t heap_memory[TEST_LARGE_HEAP_SIZE];
    uint8_t canary_after[CANARY_SIZE];
    heap_t heap;

    void SetUp() override {
        std::memset(canary_before, CANARY_PATTERN, CANARY_SIZE);
        std::memset(canary_after, CANARY_PATTERN, CANARY_SIZE);
        std::memset(heap_memory, UNINIT_PATTERN, TEST_LARGE_HEAP_SIZE);
        heap_init(&heap, heap_memory, TEST_LARGE_HEAP_SIZE);
    }

    void TearDown() override {
        for (size_t i = 0; i < CANARY_SIZE; i++) {
            EXPECT_EQ(canary_before[i], CANARY_PATTERN);
            EXPECT_EQ(canary_after[i], CANARY_PATTERN);
        }
        heap_deinit(&heap);
    }
};

#ifdef USE_SINGLE_HEAP_MEMORY
/**
 * @brief Fixture for single heap mode tests
 */
class UstringSingleHeapFixture : public ::testing::Test {
protected:
    static constexpr size_t BUFFER_SIZE = 4096;
    alignas(4) uint8_t buffer[BUFFER_SIZE];

    void SetUp() override {
        dalloc_unregister_heap(true);
        std::memset(buffer, UNINIT_PATTERN, BUFFER_SIZE);
        dalloc_register_heap(buffer, BUFFER_SIZE);
    }

    void TearDown() override {
        dalloc_unregister_heap(true);
    }
};
#endif // USE_SINGLE_HEAP_MEMORY

#endif // USTRING_TEST_CONFIG_H
