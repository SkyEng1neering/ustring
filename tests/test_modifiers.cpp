/**
 * @file test_modifiers.cpp
 * @brief Modifier tests for ustring (push_back, pop_back, append, assign, clear)
 */

#include "ustring_test_config.h"

class ModifiersTest : public UstringTestFixture {};

// ==================== push_back() Tests ====================

TEST_F(ModifiersTest, PushBack_AddsCharacter) {
    ustring str(&heap);
    EXPECT_TRUE(str.push_back('A'));
    EXPECT_EQ(str.size(), 1u);
    EXPECT_STREQ(str.c_str(), "A");
}

TEST_F(ModifiersTest, PushBack_MultipleChars) {
    ustring str(&heap);
    str.push_back('H');
    str.push_back('i');
    str.push_back('!');
    EXPECT_EQ(str.size(), 3u);
    EXPECT_STREQ(str.c_str(), "Hi!");
}

TEST_F(ModifiersTest, PushBack_ToExistingString) {
    ustring str("Hello", &heap);
    str.push_back('!');
    EXPECT_EQ(str.size(), 6u);
    EXPECT_STREQ(str.c_str(), "Hello!");
}

TEST_F(ModifiersTest, PushBack_NullChar) {
    ustring str("AB", &heap);
    // Pushing null char - string should still work
    str.push_back('\0');
    // Size increases but c_str will show truncated
    EXPECT_EQ(str.size(), 3u);
}

// ==================== pop_back() Tests ====================

TEST_F(ModifiersTest, PopBack_RemovesLastChar) {
    ustring str("Hello", &heap);
    EXPECT_TRUE(str.pop_back());
    EXPECT_EQ(str.size(), 4u);
    EXPECT_STREQ(str.c_str(), "Hell");
}

TEST_F(ModifiersTest, PopBack_MultipleRemoves) {
    ustring str("Hello", &heap);
    str.pop_back();
    str.pop_back();
    str.pop_back();
    EXPECT_EQ(str.size(), 2u);
    EXPECT_STREQ(str.c_str(), "He");
}

TEST_F(ModifiersTest, PopBack_EmptyString_ReturnsFalse) {
    ustring str(&heap);
    EXPECT_FALSE(str.pop_back());
}

TEST_F(ModifiersTest, PopBack_SingleChar_BecomesEmpty) {
    ustring str("X", &heap);
    EXPECT_TRUE(str.pop_back());
    EXPECT_TRUE(str.empty());
    EXPECT_EQ(str.size(), 0u);
}

TEST_F(ModifiersTest, PopBack_ThenPushBack) {
    ustring str("ABC", &heap);
    str.pop_back();
    str.push_back('X');
    EXPECT_STREQ(str.c_str(), "ABX");
}

// ==================== append() Tests ====================

TEST_F(ModifiersTest, Append_CString) {
    ustring str("Hello", &heap);
    EXPECT_TRUE(str.append(" World"));
    EXPECT_STREQ(str.c_str(), "Hello World");
}

TEST_F(ModifiersTest, Append_ToEmptyString) {
    ustring str(&heap);
    EXPECT_TRUE(str.append("Hello"));
    EXPECT_STREQ(str.c_str(), "Hello");
}

TEST_F(ModifiersTest, Append_EmptyCString_ReturnsFalse) {
    ustring str("Hello", &heap);
    EXPECT_FALSE(str.append(""));
    EXPECT_STREQ(str.c_str(), "Hello");
}

TEST_F(ModifiersTest, Append_CStringWithLength) {
    ustring str("Hello", &heap);
    EXPECT_TRUE(str.append(" World!!!", 6));  // Append " World" only
    EXPECT_STREQ(str.c_str(), "Hello World");
}

TEST_F(ModifiersTest, Append_Ustring) {
    ustring str1("Hello", &heap);
    ustring str2(" World", &heap);
    EXPECT_TRUE(str1.append(str2));
    EXPECT_STREQ(str1.c_str(), "Hello World");
}

TEST_F(ModifiersTest, Append_SingleChar) {
    ustring str("Hello", &heap);
    EXPECT_TRUE(str.append('!'));
    EXPECT_STREQ(str.c_str(), "Hello!");
}

TEST_F(ModifiersTest, Append_MultipleAppends) {
    ustring str(&heap);
    str.append("A");
    str.append("B");
    str.append("C");
    EXPECT_STREQ(str.c_str(), "ABC");
}

TEST_F(ModifiersTest, Append_SelfAppend) {
    ustring str("AB", &heap);
    EXPECT_TRUE(str.append(str));  // Self-append
    EXPECT_STREQ(str.c_str(), "ABAB");
    EXPECT_EQ(str.size(), 4u);
}

TEST_F(ModifiersTest, Append_SelfAppend_LongerString) {
    ustring str("Hello", &heap);
    EXPECT_TRUE(str.append(str));
    EXPECT_STREQ(str.c_str(), "HelloHello");
    EXPECT_EQ(str.size(), 10u);
}

// ==================== assign() Tests ====================

TEST_F(ModifiersTest, Assign_CString) {
    ustring str(&heap);
    EXPECT_TRUE(str.assign("Hello"));
    EXPECT_STREQ(str.c_str(), "Hello");
}

TEST_F(ModifiersTest, Assign_ReplacesExisting) {
    ustring str("Old String", &heap);
    EXPECT_TRUE(str.assign("New"));
    EXPECT_STREQ(str.c_str(), "New");
    EXPECT_EQ(str.size(), 3u);
}

TEST_F(ModifiersTest, Assign_EmptyString_ReturnsFalse) {
    ustring str("Hello", &heap);
    EXPECT_FALSE(str.assign(""));
    // Original string unchanged
}

TEST_F(ModifiersTest, Assign_CStringWithLength) {
    ustring str(&heap);
    EXPECT_TRUE(str.assign("Hello World", 5));
    EXPECT_STREQ(str.c_str(), "Hello");
}

TEST_F(ModifiersTest, Assign_Ustring) {
    ustring str1(&heap);
    ustring str2("Source", &heap);
    EXPECT_TRUE(str1.assign(str2));
    EXPECT_STREQ(str1.c_str(), "Source");
}

TEST_F(ModifiersTest, Assign_MultipleAssigns) {
    ustring str(&heap);
    str.assign("First");
    EXPECT_STREQ(str.c_str(), "First");
    str.assign("Second");
    EXPECT_STREQ(str.c_str(), "Second");
    str.assign("Third");
    EXPECT_STREQ(str.c_str(), "Third");
}

TEST_F(ModifiersTest, Assign_Ustring_SelfAssignment) {
    ustring str("Hello World", &heap);
    EXPECT_TRUE(str.assign(str));  // Self-assignment
    EXPECT_STREQ(str.c_str(), "Hello World");
    EXPECT_EQ(str.size(), 11u);
}

// ==================== clear() Tests ====================

TEST_F(ModifiersTest, Clear_EmptiesString) {
    ustring str("Hello World", &heap);
    str.clear();
    EXPECT_TRUE(str.empty());
    EXPECT_EQ(str.size(), 0u);
}

TEST_F(ModifiersTest, Clear_AlreadyEmpty) {
    ustring str(&heap);
    str.clear();
    EXPECT_TRUE(str.empty());
}

TEST_F(ModifiersTest, Clear_ThenAppend) {
    ustring str("Old", &heap);
    str.clear();
    str.append("New");
    EXPECT_STREQ(str.c_str(), "New");
}

TEST_F(ModifiersTest, Clear_PreservesCapacity) {
    ustring str("Hello World", &heap);
    str.reserve(100);
    uint32_t cap_before = str.capacity();
    str.clear();
    // Capacity should be preserved after clear
    EXPECT_EQ(str.capacity(), cap_before);
}
