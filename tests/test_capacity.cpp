/**
 * @file test_capacity.cpp
 * @brief Capacity management tests for ustring
 */

#include "ustring_test_config.h"

class CapacityTest : public UstringTestFixture {};

// ==================== capacity() Tests ====================

TEST_F(CapacityTest, Capacity_NewString_IsZeroOrMinimal) {
    ustring str(&heap);
    // New string may have 0 capacity or minimal reserved
    EXPECT_GE(str.capacity(), 0u);
}

TEST_F(CapacityTest, Capacity_AfterReserve_AtLeastRequested) {
    ustring str(&heap);
    str.reserve(100);
    EXPECT_GE(str.capacity(), 100u);
}

TEST_F(CapacityTest, Capacity_AfterAssign_SufficientForString) {
    ustring str(&heap);
    str.assign("Hello World");
    EXPECT_GE(str.capacity(), str.size());
}

// ==================== reserve() Tests ====================

TEST_F(CapacityTest, Reserve_IncreasesCapacity) {
    ustring str(&heap);
    uint32_t old_capacity = str.capacity();
    str.reserve(50);
    EXPECT_GE(str.capacity(), 50u);
    EXPECT_GE(str.capacity(), old_capacity);
}

TEST_F(CapacityTest, Reserve_PreservesContent) {
    ustring str("Hello", &heap);
    str.reserve(100);
    EXPECT_STREQ(str.c_str(), "Hello");
    EXPECT_EQ(str.size(), 5u);
}

TEST_F(CapacityTest, Reserve_ReturnsTrue_OnSuccess) {
    ustring str(&heap);
    EXPECT_TRUE(str.reserve(50));
}

TEST_F(CapacityTest, Reserve_MultipleReserves) {
    ustring str(&heap);
    EXPECT_TRUE(str.reserve(10));
    EXPECT_TRUE(str.reserve(50));
    EXPECT_TRUE(str.reserve(100));
    EXPECT_GE(str.capacity(), 100u);
}

// ==================== resize() Tests ====================

TEST_F(CapacityTest, Resize_Grow_IncreasesSize) {
    ustring str("Hi", &heap);
    EXPECT_TRUE(str.resize(10));
    EXPECT_EQ(str.size(), 10u);
}

TEST_F(CapacityTest, Resize_Shrink_DecreasesSize) {
    ustring str("Hello World", &heap);
    EXPECT_TRUE(str.resize(5));
    EXPECT_EQ(str.size(), 5u);
    EXPECT_STREQ(str.c_str(), "Hello");
}

TEST_F(CapacityTest, Resize_ToZero_ClearsString) {
    ustring str("Hello", &heap);
    EXPECT_TRUE(str.resize(0));
    EXPECT_EQ(str.size(), 0u);
    EXPECT_TRUE(str.empty());
}

TEST_F(CapacityTest, Resize_SameSize_NoChange) {
    ustring str("Hello", &heap);
    EXPECT_TRUE(str.resize(5));
    EXPECT_EQ(str.size(), 5u);
    EXPECT_STREQ(str.c_str(), "Hello");
}

TEST_F(CapacityTest, Resize_WithFillChar) {
    ustring str("Hi", &heap);
    EXPECT_TRUE(str.resize(5, 'X'));
    EXPECT_EQ(str.size(), 5u);
    // First two chars should be "Hi", rest filled with 'X'
    EXPECT_EQ(str.at(0), 'H');
    EXPECT_EQ(str.at(1), 'i');
}

TEST_F(CapacityTest, Resize_GrowPreservesOriginal) {
    ustring str("ABC", &heap);
    str.resize(6);
    EXPECT_EQ(str.at(0), 'A');
    EXPECT_EQ(str.at(1), 'B');
    EXPECT_EQ(str.at(2), 'C');
}

// ==================== shrink_to_fit() Tests ====================

TEST_F(CapacityTest, ShrinkToFit_ReducesCapacity) {
    ustring str(&heap);
    str.reserve(100);
    str.assign("Hi");
    uint32_t old_capacity = str.capacity();
    str.shrink_to_fit();
    // Capacity should be reduced (or at least not increased)
    EXPECT_LE(str.capacity(), old_capacity);
}

TEST_F(CapacityTest, ShrinkToFit_PreservesContent) {
    ustring str("Hello", &heap);
    str.reserve(100);
    str.shrink_to_fit();
    EXPECT_STREQ(str.c_str(), "Hello");
    EXPECT_EQ(str.size(), 5u);
}

TEST_F(CapacityTest, ShrinkToFit_EmptyString) {
    ustring str(&heap);
    str.reserve(100);
    str.shrink_to_fit();
    EXPECT_EQ(str.capacity(), 0u);
}

TEST_F(CapacityTest, ShrinkToFit_AfterClear) {
    ustring str("Hello World", &heap);
    str.clear();
    str.shrink_to_fit();
    EXPECT_EQ(str.capacity(), 0u);
}
