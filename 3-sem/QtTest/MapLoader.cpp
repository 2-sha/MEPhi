#include "MapLoader.h"
#include "Obstacle.h"
#include "InterestPoint.h"
#include "CommandCenter.h"
#include "Robot.h"
#include "Coord.h"
#include "Generator.h"
#include "Controller.h"
#include "Sensor.h"
#include "RobotCommander.h"
using namespace robots;
        
std::shared_ptr<Map> MapLoader::load(const QJsonObject& json)
{
    if (json.find("width") == json.end() || json.find("height") == json.end())
        throw std::invalid_argument("Field size doesn't specified");
    int width = json.value("width").toInt();
    int height = json.value("height").toInt();
    if (width < 1 || height < 1)
        throw std::invalid_argument("Incorrect field size");

    auto map = std::make_shared<Map>(width, height);

    for (const auto& item : json.value("objects").toArray())
        loadMapObject(item.toObject(), map);

    return map;
}

std::shared_ptr<Map> MapLoader::loadFromFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::invalid_argument("Unable to open file");
    QJsonObject json = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    return load(json);
}

unsigned MapLoader::loadUInt(const QJsonObject& json, const QString&& val, bool required)
{
    if (required && json.find(val) == json.end())
        throw std::invalid_argument("Value \"" + val.toStdString() + "\" not found");
    int res = json.value(val).toInt();
    if (res < 0)
        throw std::invalid_argument("Incorrect filed value \"" + val.toStdString() + "\"");
    return static_cast<unsigned>(res);
}

std::string MapLoader::loadStr(const QJsonObject& json, const QString&& val, bool required)
{
    if (required && json.find(val) == json.end())
        throw std::invalid_argument("Value \"" + val.toStdString() + "\" not found");
    return json.value(val).toString().toStdString();
}

void MapLoader::loadMapObject(const QJsonObject& json, std::shared_ptr<Map> map)
{
    auto type = loadStr(json, "type");

    try
    {
        std::shared_ptr<MapObject> obj;
        if (type == "obstacle")
            obj = map->addObject<Obstacle>(Coord{ loadUInt(json, "x"), loadUInt(json, "y") });
        else if (type == "interest-point")
            obj = map->addObject<InterestPoint>(Coord{ loadUInt(json, "x"), loadUInt(json, "y") });
        else if (type == "command-center")
            obj = map->addObject<CommandCenter>(
                Coord{ loadUInt(json, "x"), loadUInt(json, "y") },
                loadUInt(json, "consumption"),
                loadUInt(json, "slots_num"),
                loadUInt(json, "price")
            );
        else if (type == "robot")
            obj = map->addObject<Robot>(
                Coord{ loadUInt(json, "x"), loadUInt(json, "y") },
                loadUInt(json, "consumption"),
                loadUInt(json, "slots_num"),
                loadUInt(json, "price")
            );
        else if (type == "robot-commander")
            obj = map->addObject<RobotCommander>(
                Coord{ loadUInt(json, "x"), loadUInt(json, "y") },
                loadUInt(json, "consumption"),
                loadUInt(json, "slots_num"),
                loadUInt(json, "price")
            );
        else
            throw std::invalid_argument("Unknown block encountered");

        auto platform = std::dynamic_pointer_cast<Platform>(obj);
        if (platform)
        {
            std::vector<std::shared_ptr<Module>> modules;
            for (const auto& item : json.value("modules").toArray())
                loadModule(item.toObject(), platform);
        }
    }
    catch (std::invalid_argument& ex)
    {
        throw std::invalid_argument(type + ": " + ex.what());
    }
}

void robots::MapLoader::loadModule(const QJsonObject& json, std::shared_ptr<Platform> platform)
{
    auto type = loadStr(json, "type");

    try
    {
        if (type == "generator")
            platform->addModule<Generator>(loadUInt(json, "priority"), loadUInt(json, "energy"), loadUInt(json, "price"));
        else if (type == "controller")
            platform->addModule<Controller>(loadUInt(json, "priority"), loadUInt(json, "consumption"), loadUInt(json, "price"), loadUInt(json, "max_robots_num"), loadUInt(json, "range"));
        else if (type == "sensor")
            platform->addModule<Sensor>(loadUInt(json, "priority"), loadUInt(json, "consumption"), loadUInt(json, "price"), loadUInt(json, "radius"));
        else
            throw std::invalid_argument("Unknown module encountered");
    }
    catch (std::invalid_argument& ex)
    {
        throw std::invalid_argument(type + ": " + ex.what());
    }
}