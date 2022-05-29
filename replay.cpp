#include "replay.h"

namespace WSTR{

///
/// \brief replay::Replay
///
Replay::Replay()
    : validity_     ()
    , id_           ()
    , size_         ()
    , respawn_      ()
    , duration_     ()
    , vinnerTeam_   ()
    , dateTime_     ()
    , vehicle_      ()
    , mapName_      ()
    , userName_     ()
    , testName_     ()
    , replayName_   ()
    , alliedTeam_   (30)
    , opposingTeam_ (30)
{
    static size_t id{};
    id_ = id++;
}

///
/// \brief Replay::getValidity
/// \return
///
bool Replay::getValidity() const
{
    return validity_;
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
/// \brief Replay::getVinnerTeam
/// \return
///
size_t Replay::getVinnerTeam() const
{
    return vinnerTeam_;
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
/// \brief Replay::setValidity
/// \param newValidity
///
void Replay::setValidity(bool newValidity)
{
    validity_ = newValidity;
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
/// \brief Replay::setVinnerTeam
/// \param newVinnerTeam
///
void Replay::setVinnerTeam(size_t newVinnerTeam)
{
    vinnerTeam_ = newVinnerTeam;
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




}
