#include "pch.h"
#include "../Algorithms/utils.h"
using namespace lab3;

TEST(Bone, testContructor)
{
    Bone b1;
    ASSERT_EQ(b1.get(), std::make_pair(0, 0));

    Bone b2(Bone::MAX_POINTS, Bone::MAX_POINTS);
    ASSERT_EQ(b2.get(), std::make_pair(Bone::MAX_POINTS, Bone::MAX_POINTS));

    ASSERT_THROW(Bone(Bone::MAX_POINTS + 1, 0), std::invalid_argument);
}

TEST(Bone, testGetSet)
{
    Bone b;

    b.set(Bone::MAX_POINTS, Bone::MAX_POINTS);
    ASSERT_EQ(b.get(), std::make_pair(Bone::MAX_POINTS, Bone::MAX_POINTS));

    ASSERT_THROW(b.set(Bone::MAX_POINTS + 1, 0), std::invalid_argument);
}

TEST(Bone, testEqualOperator)
{
    Bone b1(0, 1), b2(0, 1);

    ASSERT_TRUE(b1 == b2);
    ASSERT_FALSE(b1 != b2);

    b1.set(1, 1);
    ASSERT_FALSE(b1 == b2);

    b1.set(1, 0);
    ASSERT_FALSE(b1 == b2);
}

TEST(Bone, testLessOperator)
{
    Bone b1(1, 1), b2(2, 2);
    ASSERT_TRUE(b1 < b2);
}

TEST(Bone, testGetSum)
{
    Bone b(5, 5);
    ASSERT_EQ(b.getSum(), 10);
}

::testing::AssertionResult isBonesUnique(const Domino& domino)
{
    bool isUnique = true;
    for (int i = 0; i < domino.size(); i++)
    {
        Bone bone = domino[i];
        for (int j = 0; j < domino.size(); j++)
        {
            if (i == j)
                continue;
            if (bone == domino[j])
            {
                isUnique = false;
                goto END;
            }
        }
    }

END:
    if (isUnique)
        return ::testing::AssertionSuccess();
    return ::testing::AssertionFailure();
}

TEST(Domino, testContructor)
{
    Domino d1(5);
    ASSERT_TRUE(isBonesUnique(d1));
    ASSERT_EQ(d1.size(), 5);
    ASSERT_THROW(Domino(Domino::MAX_BONES + 1), std::overflow_error);

    Domino d4(Domino::MAX_BONES);
    ASSERT_TRUE(isBonesUnique(d4));
    ASSERT_EQ(d4.size(), Domino::MAX_BONES);

    Domino d5(Bone(1, 1));
    ASSERT_EQ(d5.size(), 1);
    ASSERT_TRUE(d5[0] == Bone(1, 1));

    Domino d2;
    ASSERT_EQ(d2.size(), 0);

    Domino d3{ Bone(0, Bone::MAX_POINTS), Bone(0, Bone::MAX_POINTS - 1), Bone(Bone::MAX_POINTS, 0) };
    ASSERT_EQ(d3.size(), 3);
    ASSERT_TRUE(d3[1] == Bone(0, Bone::MAX_POINTS - 1));
    ASSERT_TRUE(d3[2] == Bone(Bone::MAX_POINTS, 0));
    ASSERT_THROW(Domino({ Bone(0, 0), Bone(0, 0) }), std::invalid_argument);
}

TEST(Domino, testBracketsOperator)
{
    Domino d1;
    ASSERT_THROW(d1[0], std::out_of_range);

    Domino d2{ Bone(0, Bone::MAX_POINTS), Bone(0, Bone::MAX_POINTS - 1), Bone(Bone::MAX_POINTS, 0) };
    ASSERT_EQ(d2.size(), 3);
    ASSERT_TRUE(d2[0] == Bone(0, Bone::MAX_POINTS));
    ASSERT_TRUE(d2[1] == Bone(0, Bone::MAX_POINTS - 1));
    ASSERT_TRUE(d2[2] == Bone(Bone::MAX_POINTS, 0));
    ASSERT_THROW(d2[3], std::out_of_range);

    Domino d3{ Bone(0, 1), Bone(1, 1) };
    d3[0] = Bone(2, 2);
    ASSERT_TRUE(d3[0] == Bone(2, 2));
}

TEST(Domino, testIncrement)
{
    Domino d1;
    ASSERT_EQ((d1++).size(), 0);
    ASSERT_EQ((++d1).size(), 2);

    Domino d2(Domino::MAX_BONES);
    ASSERT_THROW(d2++, std::overflow_error);
}

TEST(Domino, testPlus)
{
    Domino d1{ Bone(0, 0), Bone(0, 2) };
    Domino d2{ Bone(1, 0), Bone(2, 0) };
    
    d1 += d2;
    ASSERT_EQ(d1.size(), 4);
    ASSERT_TRUE(d1[0] == Bone(0, 0));
    ASSERT_TRUE(d1[1] == Bone(0, 2));
    ASSERT_TRUE(d1[2] == Bone(1, 0));
    ASSERT_TRUE(d1[3] == Bone(2, 0));

    ASSERT_THROW(d1 += d2, std::out_of_range);

    Domino d3(Domino::MAX_BONES);
    ASSERT_THROW(d1 += d3, std::overflow_error);

    d1 -= Bone(0, 0);
    ASSERT_EQ(d1.size(), 3);
    ASSERT_TRUE(d1[0] == Bone(0, 2));
    ASSERT_TRUE(d1[1] == Bone(1, 0));
    ASSERT_TRUE(d1[2] == Bone(2, 0));
}

TEST(Domino, testMinus)
{
    Domino d1{ Bone(0, 0), Bone(0, 1), Bone(0, 2), Bone(0, 3) };
    Domino d2{ Bone(0, 0), Bone(0, 1) };

    d1 -= d2;
    ASSERT_EQ(d1.size(), 2);
    ASSERT_TRUE(d1[0] == Bone(0, 2));
    ASSERT_TRUE(d1[1] == Bone(0, 3));

    ASSERT_THROW(d1 -= d2, std::out_of_range);

    d1 -= Bone(0, 2);
    ASSERT_EQ(d1.size(), 1);
    ASSERT_TRUE(d1[0] == Bone(0, 3));
}

TEST(Domino, testSort)
{
    Domino d{ Bone(5, 5), Bone(1, 1), Bone(3, 4), Bone(2, 2) };
    d.sort();
    ASSERT_TRUE(d[0] == Bone(1, 1));
    ASSERT_TRUE(d[1] == Bone(2, 2));
    ASSERT_TRUE(d[2] == Bone(3, 4));
    ASSERT_TRUE(d[3] == Bone(5, 5));
}

TEST(Domino, testFindScore)
{
    Domino d1{ Bone(5, 5), Bone(1, 1), Bone(3, 4), Bone(4, 3) };

    Domino d2 = d1.findScore(7);
    ASSERT_EQ(d2.size(), 2);
    ASSERT_TRUE(d2[0] == Bone(3, 4));
    ASSERT_TRUE(d2[1] == Bone(4, 3));

    Domino d3 = d1.findScore(1);
    ASSERT_EQ(d3.size(), 0);
}