#include "replay.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace WSTR{



///
/// \brief Replay::getValidity
/// \return
///
bool Replay::getIsValidity() const
{
    return getValue<bool>("validity");
}

///
/// \brief Replay::getIsReplay
/// \return
///
bool Replay::getIsReplay() const
{
    return getValue<bool>("isReplay");
}

///
/// \brief Replay::getIsHasMods
/// \return
///
bool Replay::getIsHasMods() const
{
    return getValue<bool>("hasMods");
}

///
/// \brief Replay::getUserID
/// \return
///
size_t Replay::getUserID() const
{
    return getValue<size_t>("userID");
}

///
/// \brief Replay::getArenaCreateTime
/// \return
///
size_t Replay::getArenaCreateTime() const
{
    return getValue<size_t>("arenaCreateTime");
}

///
/// \brief Replay::getId
/// \return
///
size_t Replay::getId() const
{
    return getValue<size_t>("id");
}

///
/// \brief Replay::getSize
/// \return
///
size_t Replay::getSize() const
{
    return getValue<size_t>("size");
}

///
/// \brief Replay::getRespawn
/// \return
///
size_t Replay::getRespawn() const
{
    return getValue<size_t>("respawn");
}

///
/// \brief Replay::getDuration
/// \return
///
size_t Replay::getDuration() const
{
    return getValue<size_t>("duration");
}

///
/// \brief Replay::getWinnerTeam
/// \return
///
size_t Replay::getWinnerTeam() const
{
    return getValue<size_t>("winnerTeam");
}

///
/// \brief Replay::getDateTime
/// \return
///
std::string Replay::getDateTime() const
{
    return getValue<std::string>("dateTime");
}

///
/// \brief Replay::getVehicle
/// \return
///
std::string Replay::getVehicle() const
{
    return getValue<std::string>("vehicle");
}

///
/// \brief Replay::getMapName
/// \return
///
std::string Replay::getMapName() const
{
    return getValue<std::string>("vehicle");
}

///
/// \brief Replay::getUserName
/// \return
///
std::string Replay::getUserName() const
{
    return getValue<std::string>("userName");
}

///
/// \brief Replay::getTestName
/// \return
///
std::string Replay::getTestName() const
{
    return getValue<std::string>("testName");
}

///
/// \brief Replay::getReplayName
/// \return
///
std::string Replay::getReplayName() const
{
    return getValue<std::string>("replayName");
}

///
/// \brief Replay::getClientVersionFromXML
/// \return
///
std::string Replay::getClientVersionFromXML() const
{
    return getValue<std::string>("clientVersionFromXML");
}

///////////////////////////////////////////////////////////////////////////////////


///
/// \brief Replay::setValidity
/// \param newValidity
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
/// \brief Replay::setUserID
/// \param newUserID
///
bool Replay::setUserID(size_t newUserID){
    return setCheckValue("userID", newUserID);
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
    return setCheckValue("vehicle", newVehicle);
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
bool Replay::setUserName(const std::string &newUserName)
{
    return setCheckValue("userName", newUserName);
}

///
/// \brief Replay::setTestName
/// \param newTestName
///
bool Replay::setTestName(const std::string &newTestName)
{
    return setCheckValue("testName", newTestName);
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
    return setCheckValue("clientVersionFromXML", newClientVersionFromXML);
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
    replay.setMapName(json["mapDisplayName"].toString().toStdString());
    replay.setMapName(json["mapName"].toString().toStdString());
    replay.setUserID(json["playerID"].toInt());
    replay.setUserName(json["playerName"].toString().toStdString());
    replay.setVehicle(json["playerVehicle"].toString().toStdString());
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
