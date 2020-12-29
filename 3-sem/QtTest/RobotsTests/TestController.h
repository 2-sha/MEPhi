#pragma once

#include <QtTest/QtTest>

#include "../Controller.h"
#include "../ControllerRequest.h"
#include "../Map.h"
#include "../Generator.h"
#include "../Obstacle.h"
#include "../CommandCenter.h"
#include "../RobotCommander.h"
#include "../Robot.h"
using namespace robots;

#include <memory>

class TestController : public QObject
{
	Q_OBJECT
private slots:
	void scan()
	{
		auto map = std::make_shared<Map>(10, 10);

		// Главный командный центр
		auto cc = map->addObject<CommandCenter>({ 0, 0 }, 100, 5, 500);
		cc->addModule<Generator>(1, 500, 50);
		auto cc_c = cc->addModule<Controller>(1, 50, 100, 5, 5);
		cc_c->disableManagment();

		// Робот-командир, подчиняющийся командному центру и управляющий r1 и rc3
		auto rc1 = map->addObject<RobotCommander>({ 3, 1 }, 100, 2, 350);
		rc1->addModule<Generator>(1, 200, 50);
		auto rc1_c = rc1->addModule<Controller>(1, 50, 100, 2, 3);

		// Робот-командир, подчиняющийся командному центру
		auto rc2 = map->addObject<RobotCommander>({ 1, 2 }, 100, 2, 350);
		rc2->addModule<Generator>(1, 200, 50);
		auto rc2_c = rc2->addModule<Controller>(1, 50, 100, 1, 1);

		// Робот-командир, подчиняющийся rc1
		auto rc3 = map->addObject<RobotCommander>({ 3, 4 }, 100, 2, 350);
		rc3->addModule<Generator>(1, 200, 50);
		auto rc3_c = rc3->addModule<Controller>(1, 50, 100, 1, 3);

		auto r1 = map->addObject<Robot>({ 9, 0 }, 0, 0, 100);
		auto r2 = map->addObject<Robot>({ 5, 1 }, 0, 0, 100);

		QCOMPARE(cc->dispatch(ScanRequest()), true);

		QCOMPARE(cc_c->getManager().lock(), nullptr);
		QCOMPARE(rc1_c->getManager().lock(), cc_c);
		QCOMPARE(rc2_c->getManager().lock(), cc_c);
		QCOMPARE(rc3_c->getManager().lock(), rc1_c);

		QCOMPARE(rc1->getManager().lock(), cc_c);
		QCOMPARE(rc2->getManager().lock(), cc_c);
		QCOMPARE(rc3->getManager().lock(), rc1_c);
		QCOMPARE(r2->getManager().lock(), rc1_c);
		QCOMPARE(r1->getManager().lock(), nullptr);
	}

	void makeRoute_simple()
	{
		auto map = std::make_shared<Map>(10, 10);
		auto route = Controller::makeRoute(map, 5, { 0, 0 }, { 2, 1 });
		QCOMPARE(route.top(), Coord(0, 0));
		route.pop();
		QCOMPARE(route.top(), Coord(1, 0));
		route.pop();
		QCOMPARE(route.top(), Coord(2, 0));
		route.pop();
		QCOMPARE(route.top(), Coord(2, 1));
		route.pop();
		QCOMPARE(route.empty(), true);
	}
	void makeRoute_outOfRange()
	{
		auto map = std::make_shared<Map>(10, 10);
		auto route = Controller::makeRoute(map, 3, { 0, 0 }, { 3, 1 });
		QCOMPARE(route.empty(), true);
	}
	void makeRoute_unattainable()
	{
		auto map = std::make_shared<Map>(10, 10);
		map->addObject<Obstacle>({ 2, 0 });
		map->addObject<Obstacle>({ 1, 1 });
		map->addObject<Obstacle>({ 2, 2 });
		auto route = Controller::makeRoute(map, 3, { 0, 0 }, { 2, 1 });
		QCOMPARE(route.empty(), true);
	}
};