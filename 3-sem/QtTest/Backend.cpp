#include "Backend.h"
using namespace robots;
using namespace std;


Backend::Backend(QObject* parent)
    : QObject(parent)
{
    connect(timer_.get(), &QTimer::timeout, this, &Backend::work);
}

int Backend::getBudget()
{
    auto objects = map_->getObjectsList();
    return std::accumulate(objects.begin(), objects.end(), 0U, [](unsigned sum, auto obj) {
        auto platform = std::dynamic_pointer_cast<Platform>(obj.second);
        if (platform)
            return sum + platform->calcPrice();
        return sum;
    });
}

QVariantList Backend::getMap()
{
    QVariantList jsMap;
    if (!map_)
        return jsMap;

    for (const auto &[pos, obj] : map_->getObjectsList())
    {
        QVariantMap data;
        if (typeid(*obj) == typeid(Obstacle))
            data["type"] = "obstacle";
        else if (typeid(*obj) == typeid(CommandCenter))
            data["type"] = "command-center";
        else if (typeid(*obj) == typeid(Robot))
            data["type"] = "robot";
        else if (typeid(*obj) == typeid(RobotCommander))
            data["type"] = "robot-commander";
        else if (typeid(*obj) == typeid(InterestPoint))
            data["type"] = "interest-point";

        auto platform = std::dynamic_pointer_cast<Platform>(obj);
        if (platform)
        {
            for (auto mod : platform->getModules())
            {
                // QJsonObject doesn't accept unsigned int, only int
                if (typeid(*mod) == typeid(Controller))
                    data["controller_radius"] = (int)std::static_pointer_cast<Controller>(mod)->getActionRange();
                else if (typeid(*mod) == typeid(Sensor))
                    data["sensor_radius"] = (int)std::static_pointer_cast<Sensor>(mod)->getRadius();
            }
        }

        data["pos"] = QVariantMap{ {"x", pos.x}, {"y", pos.y } };
        jsMap.push_back(data);
    }

    return jsMap;
}

QVariantMap Backend::getExplored()
{
    QVariantMap jsMap;
    if (!map_)
        return jsMap;

    for (const auto& pos : map_->explored)
        jsMap[QString::number(pos.x) + ";" + QString::number(pos.y)] = true;

    return jsMap;
}

QVariant Backend::loadMap(const QString& path)
{
    try
    {
        qDebug() << path;
        // path contains file:/// in the begining, so we need to cut it
        map_ = MapLoader::loadFromFile(path.mid(8));

        for (const auto& [_, obj] : map_->getObjectsList())
        {
            auto cc = std::dynamic_pointer_cast<CommandCenter>(obj);
            if (cc)
            {
                commander_ = cc;
                break;
            }
        }
        if (commander_ == nullptr)
            throw std::invalid_argument("Comand center is requiered");
        // TODO: Place comand center search in Map class

        commander_->dispatch(FindControllersRequest());
        commander_->dispatch(InitExplorationRequest());

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

void Backend::changeGameStatus()
{
    isPlaying_ = !isPlaying_;
    if (isPlaying_)
        timer_->start(DELAY);
    else
        timer_->stop();
    emit gameStatusUpdated();
}

void Backend::work()
{
    ExploreRequest exploreRequest;
    commander_->dispatch(exploreRequest);
    emit mapUpdated();
}