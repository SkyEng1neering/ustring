/**
 * @file test_element_access.cpp
 * @brief Element access tests for ustring (at, [], front, back)
 */

#include "ustring_test_config.h"

class ElementAccessTest : public UstringTestFixture {};

// ==================== at() Tests ====================

TEST_F(ElementAccessTest, At_ValidIndex_ReturnsCorrectChar) {
    ustring str("Hello", &heap);
    EXPECT_EQ(str.at(0), 'H');
    EXPECT_EQ(str.at(1), 'e');
    EXPECT_EQ(str.at(2), 'l');
    EXPECT_EQ(str.at(3), 'l');
    EXPECT_EQ(str.at(4), 'o');
}

TEST_F(ElementAccessTest, At_ModifyCharacter) {
    ustring str("Hello", &heap);
    str.at(0) = 'J';
    EXPECT_STREQ(str.c_str(), "Jello");
}

TEST_F(ElementAccessTest, At_AllPositions) {
    ustring str("ABCDEFGHIJ", &heap);
    for (uint32_t i = 0; i < str.size(); i++) {
        EXPECT_EQ(str.at(i), 'A' + i);
    }
}

// ==================== operator[] Tests ====================

TEST_F(ElementAccessTest, SubscriptOperator_SameAsAt) {
    ustring str("Test", &heap);
    EXPECT_EQ(str[0], str.at(0));
    EXPECT_EQ(str[1], str.at(1));
    EXPECT_EQ(str[2], str.at(2));
    EXPECT_EQ(str[3], str.at(3));
}

TEST_F(ElementAccessTest, SubscriptOperator_Modify) {
    ustring str("ABC", &heap);
    str[1] = 'X';
    EXPECT_STREQ(str.c_str(), "AXC");
}

// ==================== front() Tests ====================

TEST_F(ElementAccessTest, Front_ReturnsFirstChar) {
    ustring str("Hello", &heap);
    EXPECT_EQ(str.front(), 'H');
}

TEST_F(ElementAccessTest, Front_ModifyFirstChar) {
    ustring str("Hello", &heap);
    str.front() = 'J';
    EXPECT_STREQ(str.c_str(), "Jello");
}

TEST_F(ElementAccessTest, Front_SingleChar) {
    ustring str("X", &heap);
    EXPECT_EQ(str.front(), 'X');
}

// ==================== back() Tests ====================

TEST_F(ElementAccessTest, Back_ReturnsLastChar) {
    ustring str("Hello", &heap);
    EXPECT_EQ(str.back(), 'o');
}

TEST_F(ElementAccessTest, Back_ModifyLastChar) {
    ustring str("Hello", &heap);
    str.back() = '!';
    EXPECT_STREQ(str.c_str(), "Hell!");
}

TEST_F(ElementAccessTest, Back_SingleChar) {
    ustring str("X", &heap);
    EXPECT_EQ(str.back(), 'X');
}

TEST_F(ElementAccessTest, Back_AfterPushBack) {
    ustring str("AB", &heap);
    str.push_back('C');
    EXPECT_EQ(str.back(), 'C');
}

// ==================== front() and back() consistency ====================

TEST_F(ElementAccessTest, FrontBack_SingleChar_SameElement) {
    ustring str("X", &heap);
    EXPECT_EQ(str.front(), str.back());
}

TEST_F(ElementAccessTest, FrontBack_TwoChars_Different) {
    ustring str("AB", &heap);
    EXPECT_EQ(str.front(), 'A');
    EXPECT_EQ(str.back(), 'B');
    EXPECT_NE(str.front(), str.back());
}
