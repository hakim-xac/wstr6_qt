#ifndef REPLAY_H
#define REPLAY_H

#include <iostream>
#include <string>
#include <vector>

namespace WSTR {
class Replay
{

private:            // PRIVATE VARIABLES

    bool validity_;
    size_t id_;
    size_t size_;
    size_t respawn_;
    size_t duration_;
    size_t vinnerTeam_;
    std::string dateTime_;
    std::string vehicle_;
    std::string mapName_;
    std::string userName_;
    std::string testName_;
    std::string replayName_;

    std::vector<std::string> alliedTeam_;
    std::vector<std::string> opposingTeam_;

public:             // PUBLIC FUNCTIONS
    Replay();
    bool getValidity() const;
    size_t getId() const;
    size_t getSize() const;
    size_t getRespawn() const;
    size_t getDuration() const;
    size_t getVinnerTeam() const;
    std::string_view getDateTime() const;
    std::string_view getVehicle() const;
    std::string_view getMapName() const;
    std::string_view getUserName() const;
    std::string_view getTestName() const;
    std::string_view getReplayName() const;


    void setValidity(bool newValidity);
    void setId(size_t newId);
    void setSize(size_t newSize);
    void setRespawn(size_t newRespawn);
    void setDuration(size_t newDuration);
    void setVinnerTeam(size_t newVinnerTeam);
    void setDateTime(const std::string& newDateTime);
    void setVehicle(const std::string& newVehicle);
    void setMapName(const std::string& newMapName);
    void setUserName(const std::string& newUserName);
    void setTestName(const std::string& newTestName);
    void setReplayName(const std::string& newReplayName);


};


}
#endif // REPLAY_H
