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


    std::vector<std::string> alliedTeam_{30};
    std::vector<std::string> opposingTeam_{30};

    static inline const std::string defaultStringEmpty_ = "-";
    static constexpr size_t defaultSize_tEmpty_{ 0 };
    static constexpr bool defaultBoolEmpty_{ false };

    std::map<std::string, std::string> dataString_{
        { "dateTime", defaultStringEmpty_ }
        , { "vehicle", defaultStringEmpty_ }
        , { "mapName", defaultStringEmpty_ }
        , { "userName", defaultStringEmpty_ }
        , { "testName", defaultStringEmpty_ }
        , { "replayName", defaultStringEmpty_ }
        , { "clientVersionFromXML", defaultStringEmpty_ }
    };
    std::map<std::string, size_t> dataSize_t_{
        { "userID", defaultSize_tEmpty_ }
        , { "arenaCreateTime", defaultSize_tEmpty_ }
        , { "id", defaultSize_tEmpty_ }
        , { "size", defaultSize_tEmpty_ }
        , { "respawn", defaultSize_tEmpty_ }
        , { "duration", defaultSize_tEmpty_ }
        , { "winnerTeam", defaultSize_tEmpty_ }
    };
    std::map<std::string, bool> dataBool_{
        { "validity", defaultBoolEmpty_ }
        , { "isReplay", defaultBoolEmpty_ }
        , { "hasMods", defaultBoolEmpty_ }
    };

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
    std::string getDateTime() const;
    std::string getVehicle() const;
    std::string getMapName() const;
    std::string getUserName() const;
    std::string getTestName() const;
    std::string getReplayName() const;
    std::string getClientVersionFromXML() const;


    bool setValidity(bool newValidity);
    bool setIsReplay(bool newIsReplay);
    bool setIsHasMods(bool newHasMods);
    bool setUserID(size_t newUserID);
    bool setArenaCreateTime(size_t newArenaCreateTime);
    bool setId(size_t newId);
    bool setSize(size_t newSize);
    bool setRespawn(size_t newRespawn);
    bool setDuration(size_t newDuration);
    bool setWinnerTeam(size_t newWinnerTeam);
    bool setDateTime(const std::string &newDateTime);
    bool setVehicle(const std::string &newVehicle);
    bool setMapName(const std::string &newMapName);
    bool setUserName(const std::string &newUserName);
    bool setTestName(const std::string &newTestName);
    bool setReplayName(const std::string &newReplayName);
    bool setClientVersionFromXML(const std::string &newClientVersionFromXML);


public:


    ///
    /// \brief getValue
    /// \param key
    /// \return
    ///
    template <typename Type>
    constexpr std::decay_t<Type> getValue(std::string_view key) const;

    ///
    /// \brief isGetValue
    /// \param key
    /// \return
    ///
    template <typename Type>
    constexpr std::pair<std::decay_t<Type>, bool> isGetValue(std::string_view key) const;

    ///
    /// \brief setValue
    /// \param key
    /// \param value
    /// \return
    ///
    template <typename Type>
    void setValue(const std::string& key, Type&& newValue);

    ///
    /// \brief checkValue
    /// \param key
    /// \param value
    /// \return
    ///
    template <typename Type>
    constexpr bool checkValue(const std::string& key) const;

    ///
    /// \brief setCheckValue
    /// \param key
    /// \param newValue
    ///
    template <typename Type>
    bool setCheckValue(const std::string& key, Type&& newValue);


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
    setIsReplay(true);

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

///
/// \brief Replay::getValue
/// \param key
/// \return
///
template <typename Type>
constexpr std::decay_t<Type> Replay::getValue(std::string_view key) const{
    using type = std::decay_t<Type>;
    auto&& [value, isValue] = isGetValue<type>(key);
    if(isValue) return value;
    return type();
}

///
/// \brief Replay::isGetValue
/// \param key
/// \return
///
template <typename Type>
constexpr std::pair<std::decay_t<Type>, bool> Replay::isGetValue(std::string_view key) const{
    using type = std::decay_t<Type>;

    constexpr bool typeIsBool   { std::is_same_v<type, bool>        };
    constexpr bool typeIsString { std::is_same_v<type, std::string> };
    constexpr bool typeIsSize_t { std::is_same_v<type, size_t>      };

    if constexpr (typeIsBool){
        try{
            return { dataBool_.at(key.data()), true };
        }
        catch(std::out_of_range ex){
            assert("ERROR !!! std::out_of_range!");
            return { type(), false };
        }
    }
    else if constexpr (typeIsString) {
        try{
            return { dataString_.at(key.data()), true };
        }
        catch(...){
            assert("ERROR !!! std::out_of_range!");
            return { type(), false };
        }
    }
    else if constexpr (typeIsSize_t) {
        try{
            return { dataSize_t_.at(key.data()), true };
        }
        catch(...){
            assert("ERROR !!! std::out_of_range!");
            return { type(), false };
        }
    }
    static_assert(typeIsBool || typeIsString || typeIsSize_t, "Invalid type specified!");
    return { type(), false };
}

///
/// \brief Replay::setValue
/// \param key
/// \param newValue
///
template <typename Type>
void Replay::setValue(const std::string& key, Type&& newValue){

    using type = std::decay_t<Type>;

    constexpr bool typeIsBool   { std::is_same_v<type, bool>        };
    constexpr bool typeIsString { std::is_same_v<type, std::string> };
    constexpr bool typeIsSize_t { std::is_same_v<type, size_t>      };

    assert(checkValue<type>(key));

    if constexpr (typeIsBool){
        dataBool_[key] = std::forward<Type>(newValue);
    }
    else if constexpr (typeIsString) {
        dataString_[key] = std::forward<Type>(newValue);
    }
    else if constexpr (typeIsSize_t) {
        dataSize_t_[key] = std::forward<Type>(newValue);
    }
}

///
/// \brief Replay::checkValue
/// \param key
/// \param value
/// \return
///
template <typename Type>
constexpr bool Replay::checkValue(const std::string& key) const {
    auto&& [_, isValue] = isGetValue<std::decay_t<Type>>(key);
    return isValue;
}

///
/// \brief Replay::setCheckValue
/// \param key
/// \param newValue
///
template <typename Type>
bool Replay::setCheckValue(const std::string& key, Type&& newValue){

    Type value{ std::forward<Type>(newValue) };

    setValue<Type>(key, std::forward<Type>(newValue));
    auto&&[afterValue, isValue] = isGetValue<Type>(key);

    if(!isValue) return false;
    return afterValue == value;
}
}
#endif // REPLAY_H
