#include "replay.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>

namespace WSTR{


///////////////////////////////////////////////////////////////////////////////////


///
/// \brief Replay::setValidity
/// \param newValidity
/// \return
///
bool Replay::setValidity(bool newValidity)
{
    return setCheckValue("validity", newValidity);
}

///
/// \brief Replay::setIsReplay
/// \param newIsReplay
///
bool Replay::setIsReplay(bool newIsReplay)
{
    return setCheckValue("isReplay", newIsReplay);
}

///
/// \brief Replay::setIsHasMods
/// \param newHasMods
///
bool Replay::setIsHasMods(bool newHasMods)
{
    return setCheckValue("hasMods", newHasMods);
}

///
/// \brief Replay::setplayerID
/// \param newplayerID
///
bool Replay::setplayerID(size_t newplayerID){
    return setCheckValue("playerID", newplayerID);
}

///
/// \brief Replay::setArenaCreateTime
/// \param newArenaCreateTime
///
bool Replay::setArenaCreateTime(size_t newArenaCreateTime){
    return setCheckValue("arenaCreateTime", newArenaCreateTime);
}

///
/// \brief Replay::setId
/// \param newId
///
bool Replay::setId(size_t newId)
{
    return setCheckValue("id", newId);
}

///
/// \brief Replay::setSize
/// \param newSize
///
bool Replay::setSize(size_t newSize)
{
    return setCheckValue("size", newSize);
}

///
/// \brief Replay::setRespawn
/// \param newRespawn
///
bool Replay::setRespawn(size_t newRespawn)
{
    return setCheckValue("respawn", newRespawn);
}

///
/// \brief Replay::setDuration
/// \param newDuration
///
bool Replay::setDuration(size_t newDuration)
{
    return setCheckValue("duration", newDuration);
}

///
/// \brief Replay::setWinnerTeam
/// \param newVinnerTeam
///
bool Replay::setWinnerTeam(size_t newWinnerTeam)
{
    return setCheckValue("winnerTeam", newWinnerTeam);
}

bool Replay::setBattleType(size_t newBattleType)
{
    WSTR::BattleType bt{ getBattleType(std::to_string(newBattleType)) };

    auto&& [value, isValue] = isGetValue<std::string>(bt);
    if(isValue) {
        return setCheckValue("battleType", value);
    }
    return false;
}

///
/// \brief Replay::setDateTime
/// \param newDateTime
///
bool Replay::setDateTime(const std::string &newDateTime)
{
    return setCheckValue("dateTime", newDateTime);
}

///
/// \brief Replay::setVehicle
/// \param newVehicle
///
bool Replay::setVehicle(const std::string &newVehicle)
{
    return setCheckValue("playerVehicle", newVehicle);
}

///
/// \brief Replay::setMapName
/// \param newMapName
///
bool Replay::setMapName(const std::string &newMapName)
{
    return setCheckValue("mapName", newMapName);
}

///
/// \brief Replay::setUserName
/// \param newUserName
///
bool Replay::setPlayerName(const std::string &newPlayerName)
{
    return setCheckValue("playerName", newPlayerName);
}

///
/// \brief Replay::setTestName
/// \param newTestName
///
bool Replay::setMapDisplayName(const std::string &newMapDisplayName)
{
    return setCheckValue("mapDisplayName", newMapDisplayName);
}

///
/// \brief Replay::setReplayName
/// \param newReplayName
///
bool Replay::setReplayName(const std::string &newReplayName)
{
    return setCheckValue("replayName", newReplayName);
}


bool Replay::setClientVersionFromXML(const std::string &newClientVersionFromXML)
{
    return setCheckValue("clientVersionFromXml", newClientVersionFromXML);
}


BattleType Replay::getBattleType(const std::string& index)
{
    auto&& [value, isValue] = Settings::toType<size_t>(index);
    if(!isValue) return WSTR::BattleType::Unknown;
    switch(value){
    case 1:
    return WSTR::BattleType::Random;
    case 2:
    return WSTR::BattleType::Workout;
    case 7:
    return WSTR::BattleType::TeamBattle;
    case 9:
    return WSTR::BattleType::FanMode;
    case 13:
    return WSTR::BattleType::GK;
    case 22:
    return WSTR::BattleType::RankedBattle;
    case 24:
    return WSTR::BattleType::PitchedBattle;
    case 27:
    return WSTR::BattleType::FrontLine;
    case 29:
    return WSTR::BattleType::Tournament;
    default:
    return WSTR::BattleType::Unknown;

    }
}

size_t Replay::getCount()
{
    return count_;
}

size_t Replay::getCountValidity()
{
    return countValidity_;
}

void Replay::clearCounts()
{
    mxCount_.lock();
    count_ = 0;
    mxCount_.unlock();

    mxCountValidity_.lock();
    countValidity_ = 0;
    mxCountValidity_.unlock();
}



/////////////////////////////////////////////////////////////////////////////////



/// \brief Replay::fstreamSize
/// \param fs
/// \return
///
size_t Replay::fstreamSize(std::ifstream &fs)
{
    fs.seekg(std::ios::beg);
    auto sizeBegin{ fs.tellg() };
    fs.seekg(0, std::ios::end);
    auto sizeEnd{ fs.tellg() };
    fs.seekg(std::ios::beg);
    return static_cast<size_t>(sizeEnd - sizeBegin);
}

///
/// \brief Replay::parseFirstBlock
/// \param block
/// \param replay
/// \return
///
bool Replay::parseFirstBlock(QString&& block, WSTR::Replay& replay)
{
    QJsonDocument json{ QJsonDocument::fromJson(block.toUtf8()) };
    if(json.isNull() || !json.isObject()) return false;


    replay.setClientVersionFromXML(json["clientVersionFromXml"].toString().toStdString());
    replay.setDateTime(json["dateTime"].toString().toStdString());
    replay.setIsHasMods(json["hasMods"].toBool());
    replay.setMapName(json["mapName"].toString().toStdString());
    replay.setMapDisplayName(json["mapDisplayName"].toString().toStdString());
    replay.setplayerID(json["playerID"].toInteger());
    replay.setBattleType(json["battleType"].toInteger());
    replay.setPlayerName(json["playerName"].toString().toStdString());
    replay.setVehicle(json["playerVehicle"].toString().toStdString());


    //QJsonDocument jsonVehicles{ QJsonDocument::fromVariant(json["vehicles"].toString().toUtf8()) };

    //auto n {json["vehicles"].toString().toStdString() };

   // if(jsonVehicles.isNull() || !jsonVehicles.isObject()) return false;


    return true;
}


bool Replay::parseSecondBlock(QString&& block, WSTR::Replay& replay)
{
    QJsonParseError parseError{};
    QJsonDocument json{ QJsonDocument::fromJson(block.toUtf8(), &parseError) };

    if(parseError.error != QJsonParseError::NoError) return false;
    if(!json.isArray()) return false;
    if(!json.array()[0].isObject()) return false;

    QJsonObject oJson{ json.array()[0].toObject() };

    auto it{ oJson.find("common") };
    if(it ==oJson.end()) return false;

    ParseCommonBlock(it.value().toObject(), replay);

    return true;
}


void Replay::ParseCommonBlock(QJsonObject &&data, Replay &replay)
{
    for(auto it{data.begin()}, ite{data.end()}; it != ite; ++it)
    {
        if(it.key() == "arenaCreateTime")   replay.setArenaCreateTime(std::move(it.value().toInteger()));
        if(it.key() == "duration")          replay.setDuration(std::move(it.value().toInteger()));
        if(it.key() == "finishReason")      replay.setWinnerTeam(std::move(it.value().toInteger()));
        if(it.key() == "winnerTeam")        replay.setWinnerTeam(std::move(it.value().toInteger()));
    }
}




}
