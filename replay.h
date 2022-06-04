#ifndef REPLAY_H
#define REPLAY_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <QString>

namespace WSTR {
class Replay
{

private:            // PRIVATE VARIABLES

    bool validity_{};
    bool isReplay_{};
    size_t id_{};
    size_t size_{};
    size_t respawn_{};
    size_t duration_{};
    size_t vinnerTeam_{};
    std::string dateTime_{};
    std::string vehicle_{};
    std::string mapName_{};
    std::string userName_{};
    std::string testName_{};
    std::string replayName_{};

    std::vector<std::string> alliedTeam_{30};
    std::vector<std::string> opposingTeam_{30};

public:             // PUBLIC FUNCTIONS
    Replay() = default;

    template <typename TypeFileName>
    Replay(TypeFileName&& filename);

    bool getValidity() const;
    bool getIsReplay() const;
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
    void setIsReplay(bool newIsReplay);
    void setId(size_t newId);
    void setSize(size_t newSize);
    void setRespawn(size_t newRespawn);
    void setDuration(size_t newDuration);
    void setVinnerTeam(size_t newVinnerTeam);
    void setDateTime(const std::string &newDateTime);
    void setVehicle(const std::string &newVehicle);
    void setMapName(const std::string &newMapName);
    void setUserName(const std::string &newUserName);
    void setTestName(const std::string &newTestName);
    void setReplayName(const std::string &newReplayName);


private:

    ///
    /// \brief parseFileWotreplay
    /// \param filename
    /// \return
    ///
    template <typename TypeFileName>
    bool parseFileWotreplay(TypeFileName&& filename);

    ///
    /// \brief replaceFileName
    /// \param filename
    /// \return
    ///
    template <typename TypeAllFileName>
    constexpr std::string_view replaceFileName(TypeAllFileName&& filename);

    ///
    /// \brief get
    /// \param value
    /// \return
    ///
    template <typename Type>
    constexpr std::decay_t<Type> get(std::istream& stream);
};




//////////////////////////// implementation of class functions /////////////////////////////////////////////


template <typename TypeFileName>
Replay
::Replay(TypeFileName&& filename){

    if(!parseFileWotreplay(std::forward<TypeFileName>(filename))) return;
    isReplay_ = true;

}




template <typename TypeFileName>
bool Replay
::parseFileWotreplay(TypeFileName&& filename){

    static size_t id{};

    WSTR::Replay newReplay{};
    newReplay.setId(++id);

    std::string fname{ std::forward<TypeFileName>(filename) };
    std::ifstream file{ fname, std::ios::binary };
    if(!file.is_open()) return false;
    newReplay.setReplayName(std::string{ replaceFileName(fname) });

    file.seekg(std::ios::end);
    auto size{ file.tellg() };
    file.seekg(std::ios::beg);
    size_ = size;

    auto isReplay{ get<int>(file) };
    if(isReplay != 288633362){
        file.close();
        return false;
    }
    newReplay.setIsReplay(true);

    if(int isValidity = get<int>(file) == 2) newReplay.setValidity(true);


    file.close();
    return true;
}



template <typename TypeAllFileName>
constexpr std::string_view Replay
::replaceFileName(TypeAllFileName&& filename){
    std::string_view str{ std::forward<TypeAllFileName>(filename) };

    auto const find_slesh{ str.find_last_of('/') };
    if(find_slesh == std::string::npos) return filename;
    str = str.substr(find_slesh + 1);

    auto const find_dot{ str.find_last_of('.') };
    if(find_dot == std::string::npos) return str;

    return str.substr(0, find_dot);
}



template <typename Type>
constexpr std::decay_t<Type> Replay
::get(std::istream& stream){
    std::decay_t<Type> value{};
    stream.read(reinterpret_cast<char*>(&value), sizeof value);
    return value;
}

}
#endif // REPLAY_H
