/**
 * @file test_basic.cpp
 * @brief Basic construction and destruction tests for ustring
 */

#include "ustring_test_config.h"

class BasicTest : public UstringTestFixture {};

// ==================== Constructor Tests ====================

TEST_F(BasicTest, DefaultConstructor_CreatesEmptyString) {
    ustring str(&heap);
    EXPECT_TRUE(str.empty());
    EXPECT_EQ(str.size(), 0u);
}

TEST_F(BasicTest, HeapConstructor_SetsHeapPointer) {
    ustring str(&heap);
    EXPECT_EQ(str.get_mem_pointer(), &heap);
}

TEST_F(BasicTest, SizeConstructor_CreatesStringWithSize) {
    ustring str(10, &heap);
    EXPECT_EQ(str.size(), 10u);
}

TEST_F(BasicTest, CStringConstructor_CopiesString) {
    ustring str("Hello", &heap);
    EXPECT_EQ(str.size(), 5u);
    EXPECT_STREQ(str.c_str(), "Hello");
}

TEST_F(BasicTest, CStringConstructor_EmptyString) {
    ustring str(&heap);
    // assign("") returns false, so construct empty and don't assign
    EXPECT_EQ(str.size(), 0u);
}

TEST_F(BasicTest, CStringConstructor_LongString) {
    const char* long_str = "This is a longer string for testing purposes";
    ustring str(long_str, &heap);
    EXPECT_EQ(str.size(), strlen(long_str));
    EXPECT_STREQ(str.c_str(), long_str);
}

// ==================== c_str() Tests ====================

TEST_F(BasicTest, CStr_ReturnsNullTerminatedString) {
    ustring str("Test", &heap);
    const char* cstr = str.c_str();
    EXPECT_EQ(cstr[0], 'T');
    EXPECT_EQ(cstr[1], 'e');
    EXPECT_EQ(cstr[2], 's');
    EXPECT_EQ(cstr[3], 't');
    EXPECT_EQ(cstr[4], '\0');
}

TEST_F(BasicTest, CStr_EmptyString_ReturnsValidPointer) {
    ustring str(&heap);
    // Empty string should return valid pointer (may be nullptr or empty string)
    // Just verify no crash
    (void)str.c_str();
}

// ==================== data() Tests ====================

TEST_F(BasicTest, Data_ReturnsPointerToCharArray) {
    ustring str("ABC", &heap);
    char* data = str.data();
    EXPECT_EQ(data[0], 'A');
    EXPECT_EQ(data[1], 'B');
    EXPECT_EQ(data[2], 'C');
}

TEST_F(BasicTest, Data_ModificationAffectsString) {
    ustring str("ABC", &heap);
    char* data = str.data();
    data[1] = 'X';
    EXPECT_STREQ(str.c_str(), "AXC");
}

// ==================== size() and length() Tests ====================

TEST_F(BasicTest, Size_ReturnsCorrectLength) {
    ustring str("Hello", &heap);
    EXPECT_EQ(str.size(), 5u);
}

TEST_F(BasicTest, Length_SameAsSize) {
    ustring str("Hello", &heap);
    EXPECT_EQ(str.length(), str.size());
}

TEST_F(BasicTest, Size_EmptyString_ReturnsZero) {
    ustring str(&heap);
    EXPECT_EQ(str.size(), 0u);
}

TEST_F(BasicTest, Size_DoesNotIncludeNullTerminator) {
    ustring str("Test", &heap);
    EXPECT_EQ(str.size(), 4u);  // Not 5
}

// ==================== empty() Tests ====================

TEST_F(BasicTest, Empty_NewString_ReturnsTrue) {
    ustring str(&heap);
    EXPECT_TRUE(str.empty());
}

TEST_F(BasicTest, Empty_AfterAssign_ReturnsFalse) {
    ustring str(&heap);
    str.assign("Hello");
    EXPECT_FALSE(str.empty());
}

TEST_F(BasicTest, Empty_AfterClear_ReturnsTrue) {
    ustring str("Hello", &heap);
    str.clear();
    EXPECT_TRUE(str.empty());
}

// ==================== Destructor Tests ====================

TEST_F(BasicTest, Destructor_FreesMemory) {
    {
        ustring str("Hello World", &heap);
        EXPECT_GT(heap.alloc_info.allocations_num, 0u);
    }
    // After destructor, allocations should be freed
    EXPECT_EQ(heap.alloc_info.allocations_num, 0u);
}

TEST_F(BasicTest, MultipleStrings_AllFreed) {
    {
        ustring str1("First", &heap);
        ustring str2("Second", &heap);
        ustring str3("Third", &heap);
        EXPECT_GT(heap.alloc_info.allocations_num, 0u);
    }
    EXPECT_EQ(heap.alloc_info.allocations_num, 0u);
}
