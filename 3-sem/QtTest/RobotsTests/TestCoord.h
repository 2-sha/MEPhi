#pragma once

#include <QtTest/QtTest>

#include "../Coord.h"
using namespace robots;

class TestCoord : public QObject
{
	Q_OBJECT
private slots:
	void calcDist()
	{
		QCOMPARE(Coord::calcDist({ 0, 0 }, { 0, 2 }), 2);
		QCOMPARE(Coord::calcDist({ 0, 0 }, { 3, 1 }), 4);
		QCOMPARE(Coord::calcDist({ 0, 0 }, { 1, 1 }), 2);
	}
};