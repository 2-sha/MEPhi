#pragma once

#include <QtTest/QtTest>
#include <QJsonObject>
#include <QTemporaryFile>

#include "../MapLoader.h"
using namespace robots;

#include <memory>
#include <string>
#include <memory>

class TestMapLoader : public QObject
{
	Q_OBJECT
private slots:
	void load_success()
	{
		auto json = QJsonObject{ {"width", 10}, {"height", 10}, {"objects", QJsonArray{
			QJsonObject{ { "type", "obstacle" }, {"x", 1}, {"y", 1} },
			QJsonObject{ { "type", "command-center" }, {"x", 1}, {"y", 2}, { "consumption", 1}, { "slots_num", 5}, { "price", 1000}, { "description", "Вполне себе обычный командный центр"} }
		}} };

		std::shared_ptr<Map> map;
		try
		{
			map = MapLoader::load(json);
		}
		catch (std::exception& ex)
		{
			QVERIFY(false);
			return;
		}
		QCOMPARE(map->getWidth(), 10);
		QCOMPARE(map->getHeight(), 10);
		for (const auto& elem : json.value("objects").toArray())
		{
			auto jsonObj = elem.toObject();
			auto mapObj = map->getObject({ MapLoader::loadUInt(jsonObj, "x"), MapLoader::loadUInt(jsonObj, "y") });
			auto type = MapLoader::loadStr(jsonObj, "type");
			if (type == "obstacle")
				QCOMPARE(typeid(Obstacle), typeid(*mapObj));
			else if (type == "robot-commander")
				QCOMPARE(typeid(RobotCommander), typeid(*mapObj));
		}
	}
	void load_widthNotSet()
	{
		auto json = QJsonObject{ {"height", 10}, {"objects", QJsonArray{
			QJsonObject{ { "type", "Obstacle" }, {"x", 1}, {"y", 1} },
		}} };
		QVERIFY_EXCEPTION_THROWN(MapLoader::load(json), std::invalid_argument);
	}
	void load_incorrectHeightStr()
	{
		auto json = QJsonObject{ {"height", "aaa"}, {"width", 10}, {"objects", QJsonArray{
			QJsonObject{ { "type", "Obstacle" }, {"x", 1}, {"y", 1} },
		}} };
		QVERIFY_EXCEPTION_THROWN(MapLoader::load(json), std::invalid_argument);
	}
	void load_incorrectHeightNum()
	{
		auto json = QJsonObject{ {"height", -1}, {"width", 10}, {"objects", QJsonArray{
			QJsonObject{ { "type", "Obstacle" }, {"x", 1}, {"y", 1} },
		}} };
		QVERIFY_EXCEPTION_THROWN(MapLoader::load(json), std::invalid_argument);
	}
	void load_incorrectObjectType()
	{
		auto json = QJsonObject{ {"height", 10}, {"width", 10}, {"objects", QJsonArray{
			QJsonObject{ { "type", "Popa" }, {"x", 1}, {"y", 1} },
		}} };
		QVERIFY_EXCEPTION_THROWN(MapLoader::load(json), std::invalid_argument);
	}
	void load_incorrectObjectProps()
	{
		auto json = QJsonObject{ {"height", 10}, {"width", 10}, {"objects", QJsonArray{
			QJsonObject{ { "type", "Obstacle" }, {"a", 1}, {"y", 1} },
		}} };
		QVERIFY_EXCEPTION_THROWN(MapLoader::load(json), std::invalid_argument);
	}
};