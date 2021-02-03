#pragma once

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariantMap>
#include <QTimer>
#include <QDebug>

#include "MapLoader.h"
#include "Map.h"
#include "Obstacle.h"
#include "CommandCenter.h"
#include "Robot.h"
#include "RobotCommander.h"
#include "Coord.h"
#include "InterestPoint.h"
#include "Controller.h"
#include "Sensor.h"
#include "ControllerRequest.h"

#include <memory>
#include <algorithm>

/**
* @defgroup Backend
*
* @brief Links Map and front-end
*/

class Backend : public QObject {
private:
	Q_OBJECT
	Q_PROPERTY(QVariantList map READ getMap NOTIFY mapUpdated)
    Q_PROPERTY(QVariantMap explored READ getExplored NOTIFY mapUpdated)
    Q_PROPERTY(bool isMapLoaded READ getMapStatus NOTIFY mapStatusUpdated)
    Q_PROPERTY(bool isPlaying READ getIsPlaying NOTIFY gameStatusUpdated)
    Q_PROPERTY(int budget READ getBudget NOTIFY mapStatusUpdated)

    const unsigned DELAY = 500;

    bool isMapLoaded_ = false;
    bool isPlaying_ = false;
    std::unique_ptr<QTimer> timer_ = std::make_unique<QTimer>();

    std::shared_ptr<robots::Map> map_;
    std::shared_ptr<robots::CommandCenter> commander_;

    void work();

public:
    Backend(QObject* parent = 0);
    ~Backend() {}

    QVariantList getMap();
    QVariantMap getExplored();
    bool getMapStatus() { return isMapLoaded_; };
    bool getIsPlaying() { return isPlaying_; }
    int getBudget();


    Q_INVOKABLE QVariant loadMap(const QString &path);
    Q_INVOKABLE void changeGameStatus();

signals:
    void mapUpdated();
    void mapStatusUpdated();
    void gameStatusUpdated();
};
