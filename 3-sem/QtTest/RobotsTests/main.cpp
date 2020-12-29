#include <QtTest/QtTest>

#include "TestMapLoader.h"
#include "TestCoord.h"
#include "TestMap.h"
#include "TestPlatform.h"
#include "TestRobot.h"
#include "TestController.h"
#include "TestCoord.h"

int main(int argc, char** argv)
{
	int status = 0;
	{
		TestCoord tc;
		status |= QTest::qExec(&tc, argc, argv);
	}
	{
		TestMap tc;
		status |= QTest::qExec(&tc, argc, argv);
	}
	{
		TestMapLoader tc;
		status |= QTest::qExec(&tc, argc, argv);
	}
	{
		TestPlatform tc;
		status |= QTest::qExec(&tc, argc, argv);
	}
	{
		TestRobot tc;
		status |= QTest::qExec(&tc, argc, argv);
	}
	{
		TestController tc;
		status |= QTest::qExec(&tc, argc, argv);
	}
	{
		TestCoord tc;
		status |= QTest::qExec(&tc, argc, argv);
	}
	return status;
}