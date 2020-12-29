#pragma once

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariantMap>
#include <QDebug>

#include "MapLoader.h"
#include "Map.h"
#include "Obstacle.h"
#include "CommandCenter.h"
#include "Robot.h"
#include "RobotCommander.h"
#include "Coord.h"

#include <memory>

/**
* @defgroup Backend
*
* @brief Links Map and front-end
*/

class Backend : public QObject {
	Q_OBJECT
	Q_PROPERTY(QVariantMap map READ getMap NOTIFY mapUpdated)
    Q_PROPERTY(bool isMapLoaded READ getMapStatus NOTIFY mapStatusUpdated)

public:
    Backend(QObject* parent = 0) : QObject(parent), isMapLoaded_(false) {};
    ~Backend() {}

    QVariantMap getMap();

    bool getMapStatus() { return isMapLoaded_; };

    Q_INVOKABLE QVariant loadMap(const QString &path);

signals:
    void mapUpdated();
    void mapStatusUpdated();

private:
    bool isMapLoaded_;
    std::shared_ptr<robots::Map> map_;
};
