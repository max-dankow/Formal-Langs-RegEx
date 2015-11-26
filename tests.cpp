#include <gtest/gtest.h>
#include "Solution.h"

TEST(ManualTests, FromStatement) {
    size_t maxLength;
    ASSERT_TRUE(Solution::runSolution("bba.ab.+*b..*", "bababb", maxLength));
    ASSERT_EQ(6, maxLength);

    ASSERT_TRUE(Solution::runSolution("aab.1+.a*.b.", "bbab", maxLength));
    ASSERT_EQ(3, maxLength);
}


TEST(ManualTests, Different) {
    size_t maxLength;
    ASSERT_TRUE(Solution::runSolution("ac+*", "acaacccacaaacacacac", maxLength));
    ASSERT_EQ(19, maxLength);

    ASSERT_TRUE(Solution::runSolution("ac.*", "acaacccacaaacacccac", maxLength));
    ASSERT_EQ(3, maxLength);

    ASSERT_TRUE(Solution::runSolution("a*b*c*..", "abbbabbbabbcaabcc", maxLength));
    ASSERT_EQ(5, maxLength);
}

TEST(ManualTests, EpsilonTests) {
    size_t maxLength;
    ASSERT_TRUE(Solution::runSolution("b1+", "ab", maxLength));
    ASSERT_EQ(1, maxLength);

    ASSERT_TRUE(Solution::runSolution("bb1*..", "cbb", maxLength));
    ASSERT_EQ(2, maxLength);

    ASSERT_TRUE(Solution::runSolution("11.*", "cbb", maxLength));
    ASSERT_EQ(0, maxLength);

    ASSERT_TRUE(Solution::runSolution("11b1...", "cbb", maxLength));
    ASSERT_EQ(1, maxLength);

    ASSERT_TRUE(Solution::runSolution("ab1++", "bacc", maxLength));
    ASSERT_EQ(0, maxLength);
}

TEST(ManualTests, CorrectnessTests) {
    size_t maxLength;
    ASSERT_FALSE(Solution::runSolution("bb1*.", "abaac", maxLength));
    ASSERT_FALSE(Solution::runSolution("ca", "acabc", maxLength));
    ASSERT_FALSE(Solution::runSolution("bc**", "aacab", maxLength));
    ASSERT_TRUE(Solution::runSolution("bc*.", "abacac", maxLength));
    ASSERT_FALSE(Solution::runSolution("a*b*c*.", "abbbabbbabbcaabcc", maxLength));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}