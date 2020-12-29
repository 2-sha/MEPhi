#include "Backend.h"
using namespace robots;
using namespace std;


QVariantMap Backend::getMap()
{
    QVariantMap jsMap;
    if (!map_)
        return jsMap;

    for (const auto &[coord, obj] : map_->getObjectsList())
    {
        QString type;
        if (typeid(*obj) == typeid(Obstacle))
            type = "obstacle";
        else if (typeid(*obj) == typeid(CommandCenter))
            type = "command-center";
        else if (typeid(*obj) == typeid(Robot))
            type = "robot";
        else if (typeid(*obj) == typeid(RobotCommander))
            type = "robot-commander";
        else if (typeid(*obj) == typeid(InterestPoint))
            type = "interest-point";
        jsMap.insert(QString("%1;%2").arg(coord.x).arg(coord.y), type);
    }

    return jsMap;
}

QVariant Backend::loadMap(const QString& path)
{
    try
    {
        qDebug() << path;
        // path содержит в начале file:/// и его приходится отрезать 
        map_ = MapLoader::loadFromFile(path.mid(8));
        isMapLoaded_ = true;
        emit mapStatusUpdated();
        emit mapUpdated();
    }
    catch (std::invalid_argument& ex)
    {
        isMapLoaded_ = false;
        return ex.what();
    }
    return true;
}