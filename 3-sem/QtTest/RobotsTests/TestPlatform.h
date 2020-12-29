#pragma once

#include <QtTest/QtTest>

#include "../Platform.h"
#include "../BaseRequest.h"
#include "../Generator.h"
using namespace robots;

#include <utility>
#include <optional>
#include <any>
#include <string>

class MockRequest : public BaseRequest
{
public:
	MockRequest() : data("") {};

	std::string data;
};

class MockModule : public Module
{
public:
	MockModule(const unsigned consumption, const unsigned price, std::shared_ptr<Platform> host = nullptr)
		: Module(consumption, price, host) {}

	bool work(BaseRequest& request) override
	{
		if (typeid(request) == typeid(MockRequest))
		{
			static_cast<MockRequest&>(request).data = "Test";
			return true;
		}
		return false;
	}
};

// Ѕез using макросы QT не распознают тип из-за зап€той
using uPair = std::pair<unsigned, unsigned>;

class TestPlatform : public QObject
{
	Q_OBJECT
private slots:
	void addModule_success()
	{
		auto platform = std::make_shared<Platform>(100, 5, 500);
		platform->addModule<Generator>(1, 100, 50);
		platform->addModule<Generator>(5, 150, 50);
		platform->addModule<MockModule>(0, 100, 50);
		platform->addModule<MockModule>(2, 100, 50);
		platform->addModule<MockModule>(2, 100, 50);
		auto list = platform->getModules();
		QCOMPARE(platform->getEnergyStatus(), uPair(200, 250));
		QCOMPARE(typeid(*list[0]), typeid(MockModule));
		QCOMPARE(typeid(*list[1]), typeid(Generator));
		QCOMPARE(typeid(*list[2]), typeid(MockModule));
		QCOMPARE(typeid(*list[3]), typeid(MockModule));
		QCOMPARE(typeid(*list[4]), typeid(Generator));
		QCOMPARE(list[4]->isOn(), true);
		QCOMPARE(list[3]->isOn(), false);
	}
	void addModule_overflow()
	{
		auto platform = std::make_shared<Platform>(100, 1, 500);
		platform->addModule<Generator>(1, 100, 50);
		QVERIFY_EXCEPTION_THROWN(platform->addModule<Generator>(5, 150, 50), std::overflow_error);
	}

	void dispatch_success()
	{
		auto platform = std::make_shared<Platform>(0, 2, 500);
		platform->addModule<MockModule>(1, 0, 50);
		MockRequest req;
		platform->dispatch(req);
		QCOMPARE(req.data, "Test");
	}
	void dispatch_outOfEnergy()
	{
		auto platform = std::make_shared<Platform>(0, 2, 500);
		platform->addModule<MockModule>(1, 50, 50);
		QCOMPARE(platform->dispatch(MockRequest()), false);
	}
	void dispatch_noHandler()
	{
		auto platform = std::make_shared<Platform>(0, 2, 500);
		platform->addModule<MockModule>(1, 0, 50);
		QCOMPARE(platform->dispatch(BaseRequest()), false);
	}
};