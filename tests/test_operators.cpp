/**
 * @file test_operators.cpp
 * @brief Operator tests for ustring (+=, +, ==, !=)
 */

#include "ustring_test_config.h"

class OperatorsTest : public UstringTestFixture {};

// ==================== operator+= Tests ====================

TEST_F(OperatorsTest, PlusEquals_CString) {
    ustring str("Hello", &heap);
    EXPECT_TRUE(str += " World");
    EXPECT_STREQ(str.c_str(), "Hello World");
}

TEST_F(OperatorsTest, PlusEquals_Ustring) {
    ustring str1("Hello", &heap);
    ustring str2(" World", &heap);
    EXPECT_TRUE(str1 += str2);
    EXPECT_STREQ(str1.c_str(), "Hello World");
}

TEST_F(OperatorsTest, PlusEquals_Char) {
    ustring str("Hello", &heap);
    EXPECT_TRUE(str += '!');
    EXPECT_STREQ(str.c_str(), "Hello!");
}

TEST_F(OperatorsTest, PlusEquals_Multiple) {
    ustring str(&heap);
    str += 'A';
    str += 'B';
    str += 'C';
    EXPECT_STREQ(str.c_str(), "ABC");
}

TEST_F(OperatorsTest, PlusEquals_ChainedCStrings) {
    ustring str(&heap);
    str.assign("A");
    str += "B";
    str += "C";
    str += "D";
    EXPECT_STREQ(str.c_str(), "ABCD");
}

// ==================== operator+ Tests ====================

TEST_F(OperatorsTest, Plus_TwoUstrings) {
    ustring str1("Hello", &heap);
    ustring str2(" World", &heap);
    ustring result = str1 + str2;
    EXPECT_STREQ(result.c_str(), "Hello World");
}

TEST_F(OperatorsTest, Plus_UstringAndCString) {
    ustring str("Hello", &heap);
    ustring result = str + " World";
    EXPECT_STREQ(result.c_str(), "Hello World");
}

TEST_F(OperatorsTest, Plus_OriginalUnchanged) {
    ustring str1("Hello", &heap);
    ustring str2(" World", &heap);
    ustring result = str1 + str2;
    EXPECT_STREQ(str1.c_str(), "Hello");
    EXPECT_STREQ(str2.c_str(), " World");
}

TEST_F(OperatorsTest, Plus_EmptyStrings) {
    ustring str1(&heap);
    ustring str2(&heap);
    ustring result = str1 + str2;
    EXPECT_TRUE(result.empty());
}

TEST_F(OperatorsTest, Plus_SelfConcat) {
    ustring str("AB", &heap);
    ustring result = str + str;
    EXPECT_STREQ(result.c_str(), "ABAB");
    EXPECT_STREQ(str.c_str(), "AB");  // Original unchanged
}

TEST_F(OperatorsTest, PlusEquals_SelfAppend) {
    ustring str("Hello", &heap);
    str += str;
    EXPECT_STREQ(str.c_str(), "HelloHello");
}

// ==================== operator== Tests ====================

TEST_F(OperatorsTest, Equals_SameContent_ReturnsTrue) {
    ustring str1("Hello", &heap);
    ustring str2("Hello", &heap);
    EXPECT_TRUE(str1 == str2);
}

TEST_F(OperatorsTest, Equals_DifferentContent_ReturnsFalse) {
    ustring str1("Hello", &heap);
    ustring str2("World", &heap);
    EXPECT_FALSE(str1 == str2);
}

TEST_F(OperatorsTest, Equals_DifferentLength_ReturnsFalse) {
    ustring str1("Hello", &heap);
    ustring str2("Hello World", &heap);
    EXPECT_FALSE(str1 == str2);
}

TEST_F(OperatorsTest, Equals_EmptyStrings_ReturnsTrue) {
    ustring str1(&heap);
    ustring str2(&heap);
    EXPECT_TRUE(str1 == str2);
}

TEST_F(OperatorsTest, Equals_CaseSensitive) {
    ustring str1("Hello", &heap);
    ustring str2("hello", &heap);
    EXPECT_FALSE(str1 == str2);
}

TEST_F(OperatorsTest, Equals_SameObject) {
    ustring str("Hello", &heap);
    EXPECT_TRUE(str == str);
}

TEST_F(OperatorsTest, Equals_ConstVersion) {
    const ustring str1("Hello", &heap);
    const ustring str2("Hello", &heap);
    EXPECT_TRUE(str1 == str2);
}

// ==================== operator!= Tests ====================

TEST_F(OperatorsTest, NotEquals_SameContent_ReturnsFalse) {
    ustring str1("Hello", &heap);
    ustring str2("Hello", &heap);
    EXPECT_FALSE(str1 != str2);
}

TEST_F(OperatorsTest, NotEquals_DifferentContent_ReturnsTrue) {
    ustring str1("Hello", &heap);
    ustring str2("World", &heap);
    EXPECT_TRUE(str1 != str2);
}

TEST_F(OperatorsTest, NotEquals_DifferentLength_ReturnsTrue) {
    ustring str1("Hi", &heap);
    ustring str2("Hello", &heap);
    EXPECT_TRUE(str1 != str2);
}

TEST_F(OperatorsTest, NotEquals_EmptyAndNonEmpty) {
    ustring str1(&heap);
    ustring str2("Hello", &heap);
    EXPECT_TRUE(str1 != str2);
}

TEST_F(OperatorsTest, NotEquals_ConstVersion) {
    const ustring str1("Hello", &heap);
    const ustring str2("World", &heap);
    EXPECT_TRUE(str1 != str2);
}
