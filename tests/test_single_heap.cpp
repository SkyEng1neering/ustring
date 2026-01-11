/**
 * @file test_single_heap.cpp
 * @brief Tests for ustring in single-heap mode
 */

#include "ustring_test_config.h"

#ifdef USE_SINGLE_HEAP_MEMORY

class SingleHeapTest : public UstringSingleHeapFixture {};

// ==================== Basic Construction ====================

TEST_F(SingleHeapTest, DefaultConstructor_Works) {
    ustring str;
    EXPECT_TRUE(str.empty());
}

TEST_F(SingleHeapTest, SizeConstructor_Works) {
    ustring str(10);
    EXPECT_EQ(str.size(), 10u);
}

TEST_F(SingleHeapTest, CStringConstructor_Works) {
    ustring str("Hello");
    EXPECT_STREQ(str.c_str(), "Hello");
    EXPECT_EQ(str.size(), 5u);
}

// ==================== Basic Operations ====================

TEST_F(SingleHeapTest, PushBack_Works) {
    ustring str;
    str.push_back('A');
    str.push_back('B');
    str.push_back('C');
    EXPECT_STREQ(str.c_str(), "ABC");
}

TEST_F(SingleHeapTest, Append_Works) {
    ustring str("Hello");
    str.append(" World");
    EXPECT_STREQ(str.c_str(), "Hello World");
}

TEST_F(SingleHeapTest, Assign_Works) {
    ustring str;
    str.assign("Test String");
    EXPECT_STREQ(str.c_str(), "Test String");
}

TEST_F(SingleHeapTest, Clear_Works) {
    ustring str("Hello");
    str.clear();
    EXPECT_TRUE(str.empty());
}

// ==================== Copy Semantics ====================

TEST_F(SingleHeapTest, CopyConstructor_Works) {
    ustring original("Original");
    ustring copy(original);
    EXPECT_STREQ(copy.c_str(), "Original");
}

TEST_F(SingleHeapTest, CopyConstructor_Independent) {
    ustring original("Hello");
    ustring copy(original);
    original.append(" World");
    EXPECT_STREQ(copy.c_str(), "Hello");
}

TEST_F(SingleHeapTest, Assignment_Works) {
    ustring str1("Source");
    ustring str2;
    str2 = str1;
    EXPECT_STREQ(str2.c_str(), "Source");
}

// ==================== Operators ====================

TEST_F(SingleHeapTest, PlusOperator_TwoStrings) {
    ustring str1("Hello");
    ustring str2(" World");
    ustring result = str1 + str2;
    EXPECT_STREQ(result.c_str(), "Hello World");
}

TEST_F(SingleHeapTest, PlusOperator_StringAndCString) {
    ustring str("Hello");
    ustring result = str + " World";
    EXPECT_STREQ(result.c_str(), "Hello World");
}

TEST_F(SingleHeapTest, PlusEquals_Works) {
    ustring str("Hello");
    str += " World";
    EXPECT_STREQ(str.c_str(), "Hello World");
}

TEST_F(SingleHeapTest, Equals_SameContent) {
    ustring str1("Test");
    ustring str2("Test");
    EXPECT_TRUE(str1 == str2);
}

TEST_F(SingleHeapTest, NotEquals_DifferentContent) {
    ustring str1("Hello");
    ustring str2("World");
    EXPECT_TRUE(str1 != str2);
}

// ==================== Capacity ====================

TEST_F(SingleHeapTest, Reserve_Works) {
    ustring str;
    EXPECT_TRUE(str.reserve(100));
    EXPECT_GE(str.capacity(), 100u);
}

TEST_F(SingleHeapTest, Resize_Grow) {
    ustring str("Hi");
    str.resize(10);
    EXPECT_EQ(str.size(), 10u);
}

TEST_F(SingleHeapTest, Resize_Shrink) {
    ustring str("Hello World");
    str.resize(5);
    EXPECT_EQ(str.size(), 5u);
    EXPECT_STREQ(str.c_str(), "Hello");
}

TEST_F(SingleHeapTest, ShrinkToFit_Works) {
    ustring str;
    str.reserve(100);
    str.assign("Hi");
    str.shrink_to_fit();
    EXPECT_LE(str.capacity(), 100u);
}

// ==================== Element Access ====================

TEST_F(SingleHeapTest, At_Works) {
    ustring str("ABC");
    EXPECT_EQ(str.at(0), 'A');
    EXPECT_EQ(str.at(1), 'B');
    EXPECT_EQ(str.at(2), 'C');
}

TEST_F(SingleHeapTest, SubscriptOperator_Works) {
    ustring str("XYZ");
    EXPECT_EQ(str[0], 'X');
    EXPECT_EQ(str[1], 'Y');
    EXPECT_EQ(str[2], 'Z');
}

TEST_F(SingleHeapTest, Front_Works) {
    ustring str("Hello");
    EXPECT_EQ(str.front(), 'H');
}

TEST_F(SingleHeapTest, Back_Works) {
    ustring str("Hello");
    EXPECT_EQ(str.back(), 'o');
}

// ==================== Multiple Strings ====================

TEST_F(SingleHeapTest, MultipleStrings_Coexist) {
    ustring str1("First");
    ustring str2("Second");
    ustring str3("Third");

    EXPECT_STREQ(str1.c_str(), "First");
    EXPECT_STREQ(str2.c_str(), "Second");
    EXPECT_STREQ(str3.c_str(), "Third");
}

TEST_F(SingleHeapTest, MultipleStrings_IndependentModification) {
    ustring str1("A");
    ustring str2("B");

    str1.append("1");
    str2.append("2");

    EXPECT_STREQ(str1.c_str(), "A1");
    EXPECT_STREQ(str2.c_str(), "B2");
}

// ==================== Stress Tests ====================

TEST_F(SingleHeapTest, Stress_ManyPushBacks) {
    ustring str;
    for (int i = 0; i < 200; i++) {
        EXPECT_TRUE(str.push_back('A' + (i % 26)));
    }
    EXPECT_EQ(str.size(), 200u);
}

TEST_F(SingleHeapTest, Stress_GrowAndShrink) {
    ustring str;

    // Grow
    for (int i = 0; i < 100; i++) {
        str.push_back('X');
    }

    // Shrink
    str.resize(50);

    // Grow again
    for (int i = 0; i < 50; i++) {
        str.push_back('Y');
    }

    EXPECT_EQ(str.size(), 100u);
}

// ==================== Heap State ====================

TEST_F(SingleHeapTest, HeapState_Accessible) {
    EXPECT_TRUE(dalloc_is_initialized());
    heap_t* heap = dalloc_get_default_heap();
    EXPECT_NE(heap, nullptr);
}

#endif // USE_SINGLE_HEAP_MEMORY
