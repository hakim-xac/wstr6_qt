#ifndef REPLAY_H
#define REPLAY_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <QString>
#include <cassert>
#include <QJsonObject>

namespace WSTR {
class Replay
{

private:            // PRIVATE VARIABLES

    bool validity_{};
    bool isReplay_{};
    bool hasMods_{};
    size_t userID_{};
    size_t arenaCreateTime_{};
    size_t id_{};
    size_t size_{};
    size_t respawn_{};
    size_t duration_{};
    size_t winnerTeam_{};
    std::string dateTime_{};
    std::string vehicle_{};
    std::string mapName_{};
    std::string userName_{};
    std::string testName_{};
    std::string replayName_{};
    std::string clientVersionFromXML_{};


    std::vector<std::string> alliedTeam_{30};
    std::vector<std::string> opposingTeam_{30};

public:             // PUBLIC FUNCTIONS
    Replay() = default;

    template <typename TypeFileName>
    Replay(TypeFileName&& filename, size_t id);

    bool getIsValidity() const;
    bool getIsReplay() const;
    bool getIsHasMods() const;
    size_t getUserID() const;
    size_t getArenaCreateTime() const;
    size_t getId() const;
    size_t getSize() const;
    size_t getRespawn() const;
    size_t getDuration() const;
    size_t getWinnerTeam() const;
    std::string_view getDateTime() const;
    std::string_view getVehicle() const;
    std::string_view getMapName() const;
    std::string_view getUserName() const;
    std::string_view getTestName() const;
    std::string_view getReplayName() const;
    std::string_view getClientVersionFromXML() const;


    void setValidity(bool newValidity);
    void setIsReplay(bool newIsReplay);
    void setIsHasMods(bool newHasMods);
    void setUserID(long long newUserID);
    void setArenaCreateTime(long long newArenaCreateTime);
    void setId(size_t newId);
    void setSize(size_t newSize);
    void setRespawn(size_t newRespawn);
    void setDuration(size_t newDuration);
    void setWinnerTeam(size_t newWinnerTeam);
    void setDateTime(const std::string &newDateTime);
    void setVehicle(const std::string &newVehicle);
    void setMapName(const std::string &newMapName);
    void setUserName(const std::string &newUserName);
    void setTestName(const std::string &newTestName);
    void setReplayName(const std::string &newReplayName);
    void setClientVersionFromXML(const std::string &newClientVersionFromXML);


private:

    ///
    /// \brief fstreamSize
    /// \param fs
    /// \return
    ///
    size_t fstreamSize(std::ifstream& fs);

    ///
    /// \brief parseFirstBlock
    /// \param block
    /// \param replay
    /// \return
    ///
    bool parseFirstBlock(QString&& block, WSTR::Replay& replay);

    ///
    /// \brief parseSecondBlock
    /// \param block
    /// \param replay
    /// \return
    ///
    bool parseSecondBlock(QString&& block, WSTR::Replay& replay);

    ///
    /// \brief ParseCommonBlock
    /// \param data
    /// \param replay
    ///
    void ParseCommonBlock(QJsonObject&& data, WSTR::Replay& replay);

    ///
    /// \brief parseFileWotreplay
    /// \param filename
    /// \param id
    /// \return
    ///
    template <typename TypeFileName>
    bool parseFileWotreplay(TypeFileName&& filename, size_t id);

    ///
    /// \brief replaceFileName
    /// \param filename
    /// \return
    ///
    template <typename TypeAllFileName>
    constexpr std::string_view replaceFileName(TypeAllFileName&& filename);

    ///
    /// \brief get
    /// \param stream
    /// \param size
    /// \return
    ///
    template <typename Type>
    constexpr std::decay_t<Type> get(std::istream& stream, size_t size = sizeof(Type));


};




//////////////////////////// implementation of class functions /////////////////////////////////////////////

///
/// \brief Replay::Replay
/// \param filename
///
template <typename TypeFileName>
Replay
::Replay(TypeFileName&& filename, size_t id){

    if(!parseFileWotreplay(std::forward<TypeFileName>(filename), id)) return;
    isReplay_ = true;

}

///
/// \brief Replay::parseFileWotreplay
/// \param filename
/// \return
///
template <typename TypeFileName>
bool Replay
::parseFileWotreplay(TypeFileName&& filename, size_t id){

    WSTR::Replay newReplay{};
    newReplay.setId(id);

    std::string fname{ std::forward<TypeFileName>(filename) };
    std::ifstream file{ fname, std::ios::binary };
    if(!file.is_open()) return false;
    newReplay.setReplayName(std::string{ replaceFileName(fname) });

    newReplay.setSize( fstreamSize(file) );

    auto isReplay{ get<int>(file) };
    if(isReplay != 288633362){
        file.close();
        return false;
    }
    newReplay.setIsReplay(true);    

    auto isValidity{ get<int>(file) == 2 };
    newReplay.setValidity(isValidity);

    auto sizeFirstBlock{ get<int>(file) };

    auto firstBlock{ QString::fromStdString(get<std::string>(file, sizeFirstBlock)) };

    if(firstBlock.size() == 0) {
        file.close();
        return false;
    }

    if(parseFirstBlock(std::move(firstBlock), newReplay) && isValidity){

        auto sizeSecondBlock{ get<int>(file) };
        auto secondBlock{ QString::fromStdString(get<std::string>(file, sizeSecondBlock)) };
        parseSecondBlock(std::move(secondBlock), newReplay);
    }

    file.close();
    std::swap(newReplay, *this);
    return true;
}


///
/// \brief Replay::replaceFileName
/// \param filename
/// \return
///
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


///
/// \brief Replay::get
/// \param stream
/// \param size
/// \return
///
template <typename Type>
constexpr std::decay_t<Type> Replay
::get(std::istream& stream, size_t size){
    if constexpr(std::is_same_v<std::string, std::decay_t<Type>>){
        std::decay_t<Type> value{};
        value.resize(size);
        stream.read(reinterpret_cast<char*>(&value[0]), size);
        return value;
    }
    else{
        std::decay_t<Type> value{};
        stream.read(reinterpret_cast<char*>(&value), sizeof value);
        return value;
    }
}

}
#endif // REPLAY_H
