/**
 * @file test_copy_semantics.cpp
 * @brief Copy constructor and assignment operator tests for ustring
 */

#include "ustring_test_config.h"

class CopySemanticsTest : public UstringTestFixture {};

// ==================== Copy Constructor Tests ====================

TEST_F(CopySemanticsTest, CopyConstructor_CopiesContent) {
    ustring original("Hello World", &heap);
    ustring copy(original);
    EXPECT_STREQ(copy.c_str(), "Hello World");
    EXPECT_EQ(copy.size(), original.size());
}

TEST_F(CopySemanticsTest, CopyConstructor_IndependentCopy) {
    ustring original("Hello", &heap);
    ustring copy(original);

    // Modify original
    original.append(" World");

    // Copy should be unchanged
    EXPECT_STREQ(copy.c_str(), "Hello");
    EXPECT_STREQ(original.c_str(), "Hello World");
}

TEST_F(CopySemanticsTest, CopyConstructor_ModifyCopy) {
    ustring original("Hello", &heap);
    ustring copy(original);

    // Modify copy
    copy.append(" World");

    // Original should be unchanged
    EXPECT_STREQ(original.c_str(), "Hello");
    EXPECT_STREQ(copy.c_str(), "Hello World");
}

TEST_F(CopySemanticsTest, CopyConstructor_EmptyString) {
    ustring original(&heap);
    ustring copy(original);
    EXPECT_TRUE(copy.empty());
}

TEST_F(CopySemanticsTest, CopyConstructor_InheritsHeap) {
    ustring original("Test", &heap);
    ustring copy(original);
    EXPECT_EQ(copy.get_mem_pointer(), original.get_mem_pointer());
}

// ==================== Assignment Operator Tests ====================

TEST_F(CopySemanticsTest, Assignment_CopiesContent) {
    ustring str1("First", &heap);
    ustring str2(&heap);
    str2 = str1;
    EXPECT_STREQ(str2.c_str(), "First");
}

TEST_F(CopySemanticsTest, Assignment_SelfAssignment_Safe) {
    ustring str("Hello", &heap);
    str = str;  // Self-assignment
    EXPECT_STREQ(str.c_str(), "Hello");
}

TEST_F(CopySemanticsTest, Assignment_IndependentAfterAssign) {
    ustring str1("Hello", &heap);
    ustring str2(&heap);
    str2 = str1;

    str1.append(" World");

    // str2 should remain unchanged after modifying str1
    EXPECT_STREQ(str2.c_str(), "Hello");
}

TEST_F(CopySemanticsTest, Assignment_ChainAssignment) {
    ustring str1("Value", &heap);
    ustring str2(&heap);
    ustring str3(&heap);

    str3 = str2 = str1;

    EXPECT_STREQ(str2.c_str(), "Value");
    EXPECT_STREQ(str3.c_str(), "Value");
}

TEST_F(CopySemanticsTest, Assignment_ToNonEmpty) {
    ustring str1("New Value", &heap);
    ustring str2("Old Value", &heap);

    str2 = str1;

    // str2 should be completely replaced with str1's content
    EXPECT_STREQ(str2.c_str(), "New Value");
    EXPECT_EQ(str2.size(), str1.size());
}

// ==================== Deep Copy Verification ====================

TEST_F(CopySemanticsTest, DeepCopy_DifferentMemory) {
    ustring original("Test String", &heap);
    ustring copy(original);

    // Verify they use different memory locations for data
    EXPECT_NE(original.data(), copy.data());
}

TEST_F(CopySemanticsTest, DeepCopy_ModifyViaData_NoAffect) {
    ustring original("ABC", &heap);
    ustring copy(original);

    // Modify copy via data pointer
    copy.data()[0] = 'X';

    // Original should be unchanged
    EXPECT_EQ(original.at(0), 'A');
    EXPECT_EQ(copy.at(0), 'X');
}

// ==================== Multiple Copies ====================

TEST_F(CopySemanticsTest, MultipleCopies_AllIndependent) {
    ustring original("Base", &heap);
    ustring copy1(original);
    ustring copy2(original);
    ustring copy3(original);

    copy1.append("1");
    copy2.append("22");
    copy3.append("333");

    EXPECT_STREQ(original.c_str(), "Base");
    EXPECT_STREQ(copy1.c_str(), "Base1");
    EXPECT_STREQ(copy2.c_str(), "Base22");
    EXPECT_STREQ(copy3.c_str(), "Base333");
}

// ==================== Copy with Different Sizes ====================

TEST_F(CopySemanticsTest, CopyLongString) {
    const char* long_str = "This is a very long string that tests copying of larger strings";
    ustring original(long_str, &heap);
    ustring copy(original);
    EXPECT_STREQ(copy.c_str(), long_str);
}

TEST_F(CopySemanticsTest, CopySingleChar) {
    ustring original("X", &heap);
    ustring copy(original);
    EXPECT_STREQ(copy.c_str(), "X");
    EXPECT_EQ(copy.size(), 1u);
}
