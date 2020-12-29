#pragma once

#include <QtTest/QtTest>

#include "../Robot.h"
#include "../Map.h"
#include "../Coord.h"
using namespace robots;

#include <queue>

class TestRobot : public QObject
{
	Q_OBJECT
private slots:
	//void move_success()
	//{
	//	auto map = std::make_shared<Map>(5, 5);
	//	auto robot = map->addObject<Robot>({ 1, 1 }, 0, 2, 100);
	//	robot->setRoute(std::stack<Coord>({ {1, 2}, {1, 3} }));
	//	robot->move();
	//	QCOMPARE(robot->getPos(), Coord(1, 2));
	//	robot->move();
	//	QCOMPARE(robot->getPos(), Coord(1, 3));
	//	robot->move();
	//	QCOMPARE(robot->getPos(), Coord(1, 3));
	//}

	//void move_clearAfterError()
	//{
	//	auto map = std::make_shared<Map>(5, 5);
	//	auto robot = map->addObject<Robot>({ 1, 1 }, 0, 2, 100);
	//	robot->setRoute(std::queue<Coord>({ {6, 2}, {1, 3} }));
	//	QCOMPARE(robot->hasTask(), true);
	//	QVERIFY_EXCEPTION_THROWN(robot->move(), std::exception);
	//	QCOMPARE(robot->hasTask(), false);
	//}
};