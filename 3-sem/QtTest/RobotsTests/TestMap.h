#pragma once

#include <QtTest/QtTest>

#include "../Map.h"
#include "../MapObject.h"
#include "../Coord.h"
using namespace robots;

#include <memory>
#include <stdexcept>

class MockMapObject : public MapObject
{
public:
	MockMapObject() : MapObject() {};
};

class TestMap : public QObject
{
	Q_OBJECT
private slots:
	void addObject_success()
	{
		auto map = std::make_shared<Map>(5, 5);
		auto obj = map->addObject<MockMapObject>(Coord{ 1, 1 });
		QCOMPARE(map->getObject({ 1, 1 }), obj);
		QCOMPARE(typeid(*obj), typeid(MockMapObject));
	}
	void addObject_outOfMap()
	{
		auto map = std::make_shared<Map>(5, 5);
		QVERIFY_EXCEPTION_THROWN(map->addObject<MockMapObject>(Coord{ 7, 6 }), std::invalid_argument);
	}
	void addObject_busyField()
	{
		auto map = std::make_shared<Map>(5, 5);
		map->addObject<MockMapObject>(Coord{ 1, 1 });
		QVERIFY_EXCEPTION_THROWN(map->addObject<MockMapObject>(Coord{ 1, 1 }), std::invalid_argument);
	}

	void isOnMap()
	{
		auto map = std::make_shared<Map>(5, 5);
		QCOMPARE(map->isOnMap(Coord{ 4, 1 }), true);
		QCOMPARE(map->isOnMap(Coord{ 6, 5 }), false);
	}
};
