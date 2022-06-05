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
    return validity_;
}

///
/// \brief Replay::getIsReplay
/// \return
///
bool Replay::getIsReplay() const
{
    return isReplay_;
}

///
/// \brief Replay::getIsHasMods
/// \return
///
bool Replay::getIsHasMods() const
{
    return hasMods_;
}

///
/// \brief Replay::getUserID
/// \return
///
size_t Replay::getUserID() const
{
    return userID_;
}

///
/// \brief Replay::getArenaCreateTime
/// \return
///
size_t Replay::getArenaCreateTime() const
{
    return arenaCreateTime_;
}

///
/// \brief Replay::getId
/// \return
///
size_t Replay::getId() const
{
    return id_;
}

///
/// \brief Replay::getSize
/// \return
///
size_t Replay::getSize() const
{
    return size_;
}

///
/// \brief Replay::getRespawn
/// \return
///
size_t Replay::getRespawn() const
{
    return respawn_;
}

///
/// \brief Replay::getDuration
/// \return
///
size_t Replay::getDuration() const
{
    return duration_;
}

///
/// \brief Replay::getWinnerTeam
/// \return
///
size_t Replay::getWinnerTeam() const
{
    return winnerTeam_;
}

///
/// \brief Replay::getDateTime
/// \return
///
std::string_view Replay::getDateTime() const
{
    return dateTime_;
}

///
/// \brief Replay::getVehicle
/// \return
///
std::string_view Replay::getVehicle() const
{
    return vehicle_;
}

///
/// \brief Replay::getMapName
/// \return
///
std::string_view Replay::getMapName() const
{
    return mapName_;
}

///
/// \brief Replay::getUserName
/// \return
///
std::string_view Replay::getUserName() const
{
    return userName_;
}

///
/// \brief Replay::getTestName
/// \return
///
std::string_view Replay::getTestName() const
{
    return testName_;
}

///
/// \brief Replay::getReplayName
/// \return
///
std::string_view Replay::getReplayName() const
{
    return replayName_;
}

///
/// \brief Replay::getClientVersionFromXML
/// \return
///
std::string_view Replay::getClientVersionFromXML() const
{
    return clientVersionFromXML_;
}

///////////////////////////////////////////////////////////////////////////////////


///
/// \brief Replay::setValidity
/// \param newValidity
///
void Replay::setValidity(bool newValidity)
{
    validity_ = newValidity;
}

///
/// \brief Replay::setIsReplay
/// \param newIsReplay
///
void Replay::setIsReplay(bool newIsReplay)
{
    isReplay_ = newIsReplay;
}

///
/// \brief Replay::setIsHasMods
/// \param newHasMods
///
void Replay::setIsHasMods(bool newHasMods)
{
    isReplay_ = newHasMods;
}

///
/// \brief Replay::setUserID
/// \param newUserID
///
void Replay::setUserID(long long newUserID){
    userID_ = newUserID;
}

///
/// \brief Replay::setArenaCreateTime
/// \param newArenaCreateTime
///
void Replay::setArenaCreateTime(long long newArenaCreateTime){
    arenaCreateTime_ = newArenaCreateTime;
}

///
/// \brief Replay::setId
/// \param newId
///
void Replay::setId(size_t newId)
{
    id_ = newId;
}

///
/// \brief Replay::setSize
/// \param newSize
///
void Replay::setSize(size_t newSize)
{
    size_ = newSize;
}

///
/// \brief Replay::setRespawn
/// \param newRespawn
///
void Replay::setRespawn(size_t newRespawn)
{
    respawn_ = newRespawn;
}

///
/// \brief Replay::setDuration
/// \param newDuration
///
void Replay::setDuration(size_t newDuration)
{
    duration_ = newDuration;
}

///
/// \brief Replay::setWinnerTeam
/// \param newVinnerTeam
///
void Replay::setWinnerTeam(size_t newWinnerTeam)
{
    winnerTeam_ = newWinnerTeam;
}

///
/// \brief Replay::setDateTime
/// \param newDateTime
///
void Replay::setDateTime(const std::string &newDateTime)
{
    dateTime_ = newDateTime;
}

///
/// \brief Replay::setVehicle
/// \param newVehicle
///
void Replay::setVehicle(const std::string &newVehicle)
{
    vehicle_ = newVehicle;
}

///
/// \brief Replay::setMapName
/// \param newMapName
///
void Replay::setMapName(const std::string &newMapName)
{
    mapName_ = newMapName;
}

///
/// \brief Replay::setUserName
/// \param newUserName
///
void Replay::setUserName(const std::string &newUserName)
{
    userName_ = newUserName;
}

///
/// \brief Replay::setTestName
/// \param newTestName
///
void Replay::setTestName(const std::string &newTestName)
{
    testName_ = newTestName;
}

///
/// \brief Replay::setReplayName
/// \param newReplayName
///
void Replay::setReplayName(const std::string &newReplayName)
{
    replayName_ = newReplayName;
}


void Replay::setClientVersionFromXML(const std::string &newClientVersionFromXML)
{
    clientVersionFromXML_ = newClientVersionFromXML;
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
