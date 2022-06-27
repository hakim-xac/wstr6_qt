#ifndef REPLAY_H
#define REPLAY_H

#include <iostream>
#include "settings.h"
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <QString>
#include <cassert>
#include <mutex>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "enums.h"

namespace WSTR {
class Replay
{

private:            // PRIVATE VARIABLES


    static inline const std::string defaultStringEmpty_ { "-" };
    static constexpr size_t defaultSize_tEmpty_         { 0 };
    static constexpr bool defaultBoolEmpty_             { false };
    static inline size_t count_                         {};
    static inline size_t countValidity_                 {};
    static inline std::mutex mxCount_                   {};
    static inline std::mutex mxCountValidity_           {};
    ///
    ///
    ///
    static inline std::map<WSTR::BattleType, std::string> battleList_{
        { WSTR::BattleType::BattleForTheFortifiedArea   , "Битва за укреп район"    }
        , { WSTR::BattleType::CombatTraining            , "Боевое обучение"         }
        , { WSTR::BattleType::CompanyBattle             , "Ротный бой"              }
        , { WSTR::BattleType::FanMode                   , "Фан режим"               }
        , { WSTR::BattleType::HistoryBattles            , "Исторические бои"        }
        , { WSTR::BattleType::Random                    , "Случайный бой"           }
        , { WSTR::BattleType::Sortie                    , "Вылазка"                 }
        , { WSTR::BattleType::TeamBattle                , "Командный бой"           }
        , { WSTR::BattleType::Unknown                   , "Неизвестный режим"       }
        , { WSTR::BattleType::Workout                   , "Тренировочный бой"       }
        , { WSTR::BattleType::RankedBattle              , "Ранговый бой"            }
        , { WSTR::BattleType::PitchedBattle             , "Генеральное сражение"    }
        , { WSTR::BattleType::FrontLine                 , "Линия фронта"            }
        , { WSTR::BattleType::Tournament                , "Турнир или против ИИ"    }
        , { WSTR::BattleType::GK                        , "ГК"                      }
    };

    ///
    ///
    ///
    std::map<std::string, std::string> dataString_{
        { "dateTime"                , defaultStringEmpty_ }
        , { "vehicle"               , defaultStringEmpty_ }
        , { "mapName"               , defaultStringEmpty_ }
        , { "mapDisplayName"        , defaultStringEmpty_ }
        , { "replayName"            , defaultStringEmpty_ }
        , { "clientVersionFromXml"  , defaultStringEmpty_ }
        , { "playerName"            , defaultStringEmpty_ }
        , { "playerVehicle"         , defaultStringEmpty_ }
        , { "battleType"            , defaultStringEmpty_ }
    };

    ///
    ///
    ///
    std::map<std::string, size_t> dataSize_t_{
        { "playerID"            , defaultSize_tEmpty_ }
        , { "arenaCreateTime"   , defaultSize_tEmpty_ }
        , { "id"                , defaultSize_tEmpty_ }
        , { "size"              , defaultSize_tEmpty_ }
        , { "respawn"           , defaultSize_tEmpty_ }
        , { "duration"          , defaultSize_tEmpty_ }
        , { "winnerTeam"        , defaultSize_tEmpty_ }
    };

    ///
    ///
    ///
    std::map<std::string, bool> dataBool_{
        { "validity"    , defaultBoolEmpty_ }
        , { "isReplay"  , defaultBoolEmpty_ }
        , { "hasMods"   , defaultBoolEmpty_ }
    };

    ///
    ///
    ///
    std::vector<std::string> alliedTeam_{30};

    ///
    ///
    ///
    std::vector<std::string> opposingTeam_{30};


public:             // PUBLIC FUNCTIONS
    Replay() = default;
    ~Replay() = default;



    template <typename TypeFileName>
    Replay(TypeFileName&& filename, size_t id);

    bool setValidity(bool newValidity)                                          noexcept;
    bool setIsReplay(bool newIsReplay)                                          noexcept;
    bool setIsHasMods(bool newHasMods)                                          noexcept;
    bool setplayerID(size_t newplayerID)                                        noexcept;
    bool setArenaCreateTime(size_t newArenaCreateTime)                          noexcept;
    bool setId(size_t newId)                                                    noexcept;
    bool setSize(size_t newSize)                                                noexcept;
    bool setRespawn(size_t newRespawn)                                          noexcept;
    bool setDuration(size_t newDuration)                                        noexcept;
    bool setWinnerTeam(size_t newWinnerTeam)                                    noexcept;
    bool setBattleType(size_t newBattleType)                                    noexcept;
    bool setDateTime(const std::string &newDateTime)                            noexcept;
    bool setVehicle(const std::string &newVehicle)                              noexcept;
    bool setMapName(const std::string &newMapName)                              noexcept;
    bool setPlayerName(const std::string &newPlayerName)                        noexcept;
    bool setMapDisplayName(const std::string &newMapDisplayName)                noexcept;
    bool setReplayName(const std::string &newReplayName)                        noexcept;
    bool setClientVersionFromXML(const std::string &newClientVersionFromXML)    noexcept;


    static WSTR::BattleType getBattleType(const std::string& index)             noexcept;
    static size_t getCount()                                                    noexcept;
    static size_t getCountValidity()                                            noexcept;
    static void clearCounts()                                                   noexcept;

public:


    ///
    /// \brief getValue
    /// \param key
    /// \return
    ///
    template <typename Type, typename KeyType>
    constexpr std::decay_t<Type> getValue(KeyType&& key)                        const noexcept;

    ///
    /// \brief isGetValue
    /// \param key
    /// \return
    ///
    template <typename Type, typename KeyType>
    constexpr std::pair<std::decay_t<Type>, bool> isGetValue(KeyType&& key)     const noexcept;

    ///
    /// \brief setValue
    /// \param key
    /// \param value
    /// \return
    ///
    template <typename Type>
    void setValue(const std::string& key, Type&& newValue)                      noexcept;

    ///
    /// \brief checkValue
    /// \param key
    /// \param value
    /// \return
    ///
    template <typename Type, typename KeyType>
    constexpr bool checkValue(KeyType&& key)                                    const noexcept;

    ///
    /// \brief setCheckValue
    /// \param key
    /// \param newValue
    ///
    template <typename Type>
    bool setCheckValue(const std::string& key, Type&& newValue)                 noexcept;


private:

    ///
    /// \brief fstreamSize
    /// \param fs
    /// \return
    ///
    size_t fstreamSize(std::ifstream& fs) const noexcept;

    ///
    /// \brief parseFirstBlock
    /// \param block
    /// \param replay
    /// \return
    ///
    template <typename BlockType, typename ReplayType, typename = std::enable_if_t<
                  std::is_same_v<std::decay_t<BlockType>, QString>
                  && std::is_same_v<std::decay_t<ReplayType>, WSTR::Replay>>>
    bool parseFirstBlock(BlockType&& block, ReplayType&& replay)  const noexcept;

    ///
    /// \brief parseSecondBlock
    /// \param block
    /// \param replay
    /// \return
    ///    
    template <typename BlockType, typename ReplayType, typename = std::enable_if_t<
                  std::is_same_v<std::decay_t<BlockType>, QString>
                  && std::is_same_v<std::decay_t<ReplayType>, WSTR::Replay>>>
    bool parseSecondBlock(BlockType&& block, ReplayType&& replay);

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

template<typename BlockType, typename ReplayType, typename T3>
bool Replay::parseSecondBlock(BlockType &&block,ReplayType &&replay)
{
    QJsonParseError parseError{};
    QJsonDocument json{ QJsonDocument::fromJson(std::forward<BlockType>(block).toUtf8(), &parseError) };

    if(parseError.error != QJsonParseError::NoError) return false;
    if(!json.isArray()) return false;
    if(!json.array()[0].isObject()) return false;

    QJsonObject oJson{ json.array()[0].toObject() };

    auto it{ oJson.find("common") };
    if(it ==oJson.end()) return false;

    ParseCommonBlock(it.value().toObject(), replay);

    return true;
}




//////////////////////////// implementation of class functions /////////////////////////////////////////////


template <typename TypeFileName>
Replay
::Replay(TypeFileName&& filename, size_t id){

    if(!parseFileWotreplay(std::forward<TypeFileName>(filename), id)) return;
    setIsReplay(true);

}

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

template <typename TypeFileName>
bool Replay
::parseFileWotreplay(TypeFileName&& filename, size_t id){

    Replay newReplay{};

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

    mxCountValidity_.lock();
    if(isValidity) ++countValidity_;
    mxCountValidity_.unlock();

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

    mxCount_.lock();
    ++count_;
    mxCount_.unlock();

    std::swap(*this, newReplay);
    return true;
}

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

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

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

template <typename Type>
constexpr std::decay_t<Type> Replay
::get(std::istream& stream, size_t size){

    std::decay_t<Type> value{};

    if constexpr(std::is_same_v<std::string, std::decay_t<Type>>){
        value.resize(size);
        stream.read(reinterpret_cast<char*>(&value[0]), size);
    }
    else{
        stream.read(reinterpret_cast<char*>(&value), sizeof value);
    }

    return value;
}

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

template <typename Type, typename KeyType>
constexpr std::decay_t<Type> Replay::getValue(KeyType&& key) const noexcept
{
    using type = std::decay_t<Type>;
    auto&& [value, isValue] = isGetValue<type>(std::forward<KeyType>(key));
    if(isValue) return value;
    return type();
}

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

template <typename Type, typename KeyType>
constexpr std::pair<std::decay_t<Type>, bool> Replay::isGetValue(KeyType&& key) const noexcept{

    using type = std::decay_t<Type>;
    using keyType = std::decay_t<KeyType>;

    constexpr bool typeIsBool       { std::is_same_v<type, bool>                };
    constexpr bool typeIsString     { std::is_same_v<type, std::string>         };
    constexpr bool typeIsSize_t     { std::is_same_v<type, size_t>              };
    constexpr bool isStringView { std::is_convertible_v<keyType, std::string_view>    };

    constexpr bool typeIsBattleType { std::is_same_v<type, std::string> &&  std::is_same_v<keyType, WSTR::BattleType>   };


    if constexpr (typeIsBool && isStringView){
        try{
            return { dataBool_.at(key.data()), true };
        }
        catch(std::out_of_range ex){
            assert("ERROR !!! std::out_of_range!");
            return { type(), false };
        }
    }
    else if constexpr (typeIsString && isStringView) {
        try{
            return { dataString_.at(key.data()), true };
        }
        catch(...){
            assert("ERROR !!! std::out_of_range!");
            return { type(), false };
        }
    }
    else if constexpr (typeIsSize_t && isStringView) {
        try{
            return { dataSize_t_.at(key.data()), true };
        }
        catch(...){
            assert("ERROR !!! std::out_of_range!");
            return { type(), false };
        }
    }
    else if constexpr (typeIsBattleType) {
        try{
            return { battleList_.at(key), true };
        }
        catch(...){
            assert("ERROR !!! std::out_of_range!");
            return { type(), false };
        }
    }
    static_assert(((typeIsBool || typeIsString || typeIsSize_t) &&  isStringView )
            || typeIsBattleType, "Invalid type specified!");
    return { type(), false };
}

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

template <typename Type>
void Replay::setValue(const std::string& key, Type&& newValue) noexcept {

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

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

template <typename Type, typename KeyType>
constexpr bool Replay::checkValue(KeyType&& key) const noexcept {
    auto&& [_, isValue] = isGetValue<std::decay_t<Type>>(std::forward<KeyType>(key));
    return isValue;
}

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

template <typename Type>
bool Replay::setCheckValue(const std::string& key, Type&& newValue) noexcept{

    Type value{ std::forward<Type>(newValue) };

    setValue<Type>(key, std::forward<Type>(newValue));
    auto&&[afterValue, isValue] = isGetValue<Type>(key);

    if(!isValue) return false;
    return afterValue == value;
}

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

template<typename BlockType, typename ReplayType, typename T3>
bool Replay::parseFirstBlock(BlockType&& block, ReplayType&& replay) const noexcept
{
    QJsonDocument json{ QJsonDocument::fromJson(std::forward<BlockType>(block).toUtf8()) };
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

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////



}
#endif // REPLAY_H
