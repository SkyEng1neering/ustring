/**
 * @file test_edge_cases.cpp
 * @brief Edge case and boundary tests for ustring
 */

#include "ustring_test_config.h"

class EdgeCasesTest : public UstringTestFixture {};
class StressTest : public UstringLargeHeapFixture {};

// ==================== Empty String Edge Cases ====================

TEST_F(EdgeCasesTest, EmptyString_CStr_NotNull) {
    ustring str(&heap);
    // c_str() may return nullptr for empty string, but shouldn't crash
    const char* cstr = str.c_str();
    (void)cstr;  // Just verify no crash
}

TEST_F(EdgeCasesTest, EmptyString_Size_Zero) {
    ustring str(&heap);
    EXPECT_EQ(str.size(), 0u);
    EXPECT_EQ(str.length(), 0u);
}

TEST_F(EdgeCasesTest, EmptyString_PopBack_ReturnsFalse) {
    ustring str(&heap);
    EXPECT_FALSE(str.pop_back());
}

TEST_F(EdgeCasesTest, EmptyString_Resize_ToZero) {
    ustring str(&heap);
    EXPECT_TRUE(str.resize(0));
    EXPECT_TRUE(str.empty());
}

// ==================== Single Character Edge Cases ====================

TEST_F(EdgeCasesTest, SingleChar_FrontEqualsBack) {
    ustring str("X", &heap);
    EXPECT_EQ(str.front(), str.back());
}

TEST_F(EdgeCasesTest, SingleChar_PopBack_BecomesEmpty) {
    ustring str("X", &heap);
    EXPECT_TRUE(str.pop_back());
    EXPECT_TRUE(str.empty());
}

TEST_F(EdgeCasesTest, SingleChar_At_ValidIndex) {
    ustring str("A", &heap);
    EXPECT_EQ(str.at(0), 'A');
}

// ==================== Special Characters ====================

TEST_F(EdgeCasesTest, SpecialChars_Newline) {
    ustring str("Line1\nLine2", &heap);
    EXPECT_EQ(str.size(), 11u);
    EXPECT_EQ(str.at(5), '\n');
}

TEST_F(EdgeCasesTest, SpecialChars_Tab) {
    ustring str("A\tB", &heap);
    EXPECT_EQ(str.size(), 3u);
    EXPECT_EQ(str.at(1), '\t');
}

TEST_F(EdgeCasesTest, SpecialChars_Carriage) {
    ustring str("Test\r\n", &heap);
    EXPECT_EQ(str.size(), 6u);
}

TEST_F(EdgeCasesTest, SpecialChars_Binary) {
    ustring str(&heap);
    str.push_back('\x01');
    str.push_back('\x02');
    str.push_back('\xFF');
    EXPECT_EQ(str.size(), 3u);
    EXPECT_EQ(static_cast<uint8_t>(str.at(2)), 0xFF);
}

// ==================== Boundary Tests ====================

TEST_F(EdgeCasesTest, Resize_FromZeroToOne) {
    ustring str(&heap);
    EXPECT_TRUE(str.resize(1));
    EXPECT_EQ(str.size(), 1u);
}

TEST_F(EdgeCasesTest, Resize_FromOneToZero) {
    ustring str("X", &heap);
    EXPECT_TRUE(str.resize(0));
    EXPECT_TRUE(str.empty());
}

TEST_F(EdgeCasesTest, PushPopSequence) {
    ustring str(&heap);
    str.push_back('A');
    EXPECT_EQ(str.size(), 1u);
    str.pop_back();
    EXPECT_EQ(str.size(), 0u);
    str.push_back('B');
    EXPECT_EQ(str.size(), 1u);
    EXPECT_EQ(str.at(0), 'B');
}

// ==================== Comparison Edge Cases ====================

TEST_F(EdgeCasesTest, Compare_EmptyStrings) {
    ustring str1(&heap);
    ustring str2(&heap);
    EXPECT_TRUE(str1 == str2);
    EXPECT_FALSE(str1 != str2);
}

TEST_F(EdgeCasesTest, Compare_EmptyAndNonEmpty) {
    ustring empty(&heap);
    ustring nonEmpty("A", &heap);
    EXPECT_FALSE(empty == nonEmpty);
    EXPECT_TRUE(empty != nonEmpty);
}

TEST_F(EdgeCasesTest, Compare_SameLengthDifferentContent) {
    ustring str1("AAA", &heap);
    ustring str2("AAB", &heap);
    EXPECT_FALSE(str1 == str2);
}

// ==================== Memory Edge Cases ====================

TEST_F(EdgeCasesTest, ReserveZero_NoEffect) {
    ustring str("Test", &heap);
    EXPECT_TRUE(str.reserve(0));
    EXPECT_STREQ(str.c_str(), "Test");
}

TEST_F(EdgeCasesTest, ShrinkToFit_EmptyString) {
    ustring str(&heap);
    str.reserve(100);
    EXPECT_TRUE(str.shrink_to_fit());
    EXPECT_EQ(str.capacity(), 0u);
}

// ==================== Stress Tests ====================

TEST_F(StressTest, RapidPushPop_100Cycles) {
    ustring str(&heap);

    for (int cycle = 0; cycle < 100; cycle++) {
        // Push 10 chars
        for (int i = 0; i < 10; i++) {
            EXPECT_TRUE(str.push_back('A' + (i % 26)));
        }
        // Pop 5 chars
        for (int i = 0; i < 5; i++) {
            EXPECT_TRUE(str.pop_back());
        }
    }

    EXPECT_EQ(str.size(), 500u);  // 100 * (10 - 5)
}

TEST_F(StressTest, MultipleStrings_SameHeap) {
    ustring str1(&heap);
    ustring str2(&heap);
    ustring str3(&heap);

    for (int i = 0; i < 50; i++) {
        str1.push_back('A');
        str2.push_back('B');
        str3.push_back('C');
    }

    EXPECT_EQ(str1.size(), 50u);
    EXPECT_EQ(str2.size(), 50u);
    EXPECT_EQ(str3.size(), 50u);
}

TEST_F(StressTest, GrowShrinkPattern) {
    ustring str(&heap);

    // Grow
    for (int i = 0; i < 100; i++) {
        str.push_back('X');
    }
    EXPECT_EQ(str.size(), 100u);

    // Shrink
    str.resize(50);
    EXPECT_EQ(str.size(), 50u);

    // Grow again
    str.resize(150, 'Y');
    EXPECT_EQ(str.size(), 150u);

    // Verify first 50 are 'X'
    for (uint32_t i = 0; i < 50; i++) {
        EXPECT_EQ(str.at(i), 'X');
    }
}

TEST_F(StressTest, AppendManyTimes) {
    ustring str(&heap);

    for (int i = 0; i < 100; i++) {
        EXPECT_TRUE(str.append("A"));
    }

    EXPECT_EQ(str.size(), 100u);
}

TEST_F(StressTest, ClearAndRefill) {
    ustring str(&heap);

    for (int cycle = 0; cycle < 10; cycle++) {
        // Fill
        for (int i = 0; i < 50; i++) {
            str.push_back('X');
        }
        EXPECT_EQ(str.size(), 50u);

        // Clear
        str.clear();
        EXPECT_TRUE(str.empty());
    }
}

TEST_F(StressTest, AssignManyTimes) {
    ustring str(&heap);

    for (int i = 0; i < 50; i++) {
        EXPECT_TRUE(str.assign("Test String"));
        EXPECT_STREQ(str.c_str(), "Test String");
    }
}

// ==================== Long String Tests ====================

TEST_F(StressTest, LongString_256Chars) {
    ustring str(&heap);
    for (int i = 0; i < 256; i++) {
        str.push_back('A' + (i % 26));
    }
    EXPECT_EQ(str.size(), 256u);
}

TEST_F(StressTest, LongString_1000Chars) {
    ustring str(&heap);
    for (int i = 0; i < 1000; i++) {
        str.push_back('0' + (i % 10));
    }
    EXPECT_EQ(str.size(), 1000u);
}
