#include "pch.h"
#include "../Algorithms/utils.h"

TEST(LinearStrophoid, testContructor) 
{
	LinearStrophoid ls1(10);
	ASSERT_EQ(ls1.getA(), 10);

	LinearStrophoid ls2;
	ASSERT_EQ(ls2.getA(), 5);

	LinearStrophoid ls3(5);
	ls3.setA(10);
	ASSERT_EQ(ls3.getA(), 10);
}

TEST(LinearStrophoid, testCalcInCartesian)
{
	LinearStrophoid ls(5);

	ASSERT_NEAR(ls.calcInCartesian(-3).first, -ls.calcInCartesian(-3).second, LinearStrophoid::ABS_ERROR);
	ASSERT_NEAR(ls.calcInCartesian(-5).second, 0, LinearStrophoid::ABS_ERROR);
	ASSERT_NEAR(ls.calcInCartesian(-4).second, 1.333333333333, LinearStrophoid::ABS_ERROR);
	ASSERT_NEAR(ls.calcInCartesian(-3).second, 1.5, LinearStrophoid::ABS_ERROR);
	ASSERT_NEAR(ls.calcInCartesian(-2).second, 1.309307341416, LinearStrophoid::ABS_ERROR);
	ASSERT_NEAR(ls.calcInCartesian(2).second, -3.0550504633039, LinearStrophoid::ABS_ERROR);
	ASSERT_NEAR(ls.calcInCartesian(3).second, -6, LinearStrophoid::ABS_ERROR);
}

TEST(LinearStrophoid, test—alcInPolar)
{
	LinearStrophoid ls(5);

	ASSERT_NEAR(ls.calcInPolar(M_PI), 5, LinearStrophoid::ABS_ERROR);
	ASSERT_NEAR(ls.calcInPolar(M_PI * 2), -5, LinearStrophoid::ABS_ERROR);
	ASSERT_NEAR(ls.calcInPolar(M_PI / 2), 0, LinearStrophoid::ABS_ERROR);
	ASSERT_NEAR(ls.calcInPolar(M_PI * 3 / 2), 0, LinearStrophoid::ABS_ERROR);
}

TEST(LinearStrophoid, testGetRadiusOfCurvature)
{
	LinearStrophoid ls(5);
	ASSERT_NEAR(ls.getRadiusOfCurvature(), 7.071067811865, LinearStrophoid::ABS_ERROR);
}

TEST(LinearStrophoid, testGetLoopArea)
{
	LinearStrophoid ls(5);
	ASSERT_NEAR(ls.getLoopArea(), 10.730091830127, LinearStrophoid::ABS_ERROR);
}

TEST(LinearStrophoid, testGetLoopVolume)
{
	LinearStrophoid ls(5);
	ASSERT_NEAR(ls.getLoopVolume(), 20.797746977601, LinearStrophoid::ABS_ERROR);
}

TEST(LinearStrophoid, testGetAsymptoteArea)
{
	LinearStrophoid ls(5);
	ASSERT_NEAR(ls.getAsymptoteArea(), 89.269908169872, LinearStrophoid::ABS_ERROR);
}