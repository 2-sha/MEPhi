#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include "Coord.h"
#include "Map.h"
#include "Obstacle.h"
#include "InterestPoint.h"
#include "CommandCenter.h"
#include "Robot.h"
#include "RobotCommander.h"
#include "Module.h"

#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

namespace robots
{
    /**
    * @ingroup Backend
    * 
    * @brief Loads map from file or QJson
    * @details I separed this class from Map, because I don't want to user Qt in this lib
    */
    class MapLoader
    {
    public:
        /**
        * @brief Loads environment and objects from json
        *
        * @param json Json object, which describes map
        * @return shared_ptr to loaded map
        * @throw std::invalid_argument If incorrect config line is met
        */
        static std::shared_ptr<Map> load(const QJsonObject& json);

        /**
        * @brief Loads environment and objects from file
        *
        * @param path Path to the file with configuration
        * @return shared_ptr to loaded map
        * @throw std::invalid_argument If incorrect config line is met
        */
        static std::shared_ptr<Map> loadFromFile(const QString& path);

        /**
        * @brief Loads module from json and attachs it to specified platform
        *
        * @param json Json with module configuration
        * @param platform shared_ptr to platform, in which module will load
        * @throw std::invalid_argument If incorrect config line is met
        */
        static void loadModule(const QJsonObject& json, std::shared_ptr<Platform> platform);

        /**
        * @brief Loads object from json and attachs it to specified map
        *
        * @param json Json with object configuration
        * @param map shared_ptr to map, in which module will load
        * @throw std::invalid_argument If incorrect config line is met
        */
        static void loadMapObject(const QJsonObject& json, std::shared_ptr<Map> map);

        /**
        * @brief Loads unsigned int from json
        * @details Native function provide only int retriving
        *
        * @param json Json with number
        * @param val Name of the field with number
        * @param required If value is required and not found, std::invalid_argument will be thrown
        * @throw std::invalid_argument If incorrect config line is met
        */
        static unsigned loadUInt(const QJsonObject& json, const QString&& val, bool required = true);

        /**
        * @brief Loads std::string from json
        * @details Native function provide only QString retriving
        *
        * @param json Json with string
        * @param val Name of the field with string
        * @param required If value is required and not found, std::invalid_argument will be thrown
        * @throw std::invalid_argument If incorrect config line is met
        */
        static std::string loadStr(const QJsonObject& json, const QString&& val, bool required = true);
    };
};