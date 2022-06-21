#ifndef SETTINGS_H
#define SETTINGS_H

#include "logs.h"
#include <string_view>
#include <string>
#include <sstream>
#include <map>
#include <QDir>
#include <QCoreApplication>
#include <QComboBox>
#include <array>

namespace WSTR {


class Settings{

public:
    ///
    /// \brief getFieldName
    /// \param fn
    /// \return
    ///
    static const std::string getFieldName(WSTR::FieldNames fn);


private:
    struct Default{


        ///
        ///
        ///
        static inline std::map<WSTR::FieldNames, const std::string> fieldNames{

            { WSTR::FieldNames::ActiveColumn, "activeColumn" }
            , { WSTR::FieldNames::TypeSortColumns, "typeSortColumns" }
            , { WSTR::FieldNames::CurrentPathIndex, "currentPathIndex" }
            , { WSTR::FieldNames::CountOfPaths, "countOfPaths" }
            , { WSTR::FieldNames::WaitUpdateStatusBar_s, "waitUpdateStatusBar_s" }

        };

        ///
        ///
        ///
        static inline const std::map<const std::string, std::string> bd{
            { WSTR::Settings::getFieldName(WSTR::FieldNames::CountOfPaths), "0" }
            , { WSTR::Settings::getFieldName(WSTR::FieldNames::CurrentPathIndex), "0" }
            , { WSTR::Settings::getFieldName(WSTR::FieldNames::WaitUpdateStatusBar_s), "2" }
            , { WSTR::Settings::getFieldName(WSTR::FieldNames::ActiveColumn), "0" }
            , { WSTR::Settings::getFieldName(WSTR::FieldNames::TypeSortColumns), "0" }
        };

        ///
        ///
        ///
        static inline const std::map<const std::string, std::string> header{
            { "header_1", "id" }
            , { "header_2", "size" }
            , { "header_3", "validity" }
            , { "header_4", "hasMods" }
            , { "header_5", "respawn" }
            , { "header_6", "mapName" }
            , { "header_7", "mapDisplayName" }
            , { "header_8", "playerName" }
            , { "header_9", "playerVehicle" }
            , { "header_10", "playerID" }
            , { "header_11", "clientVersionFromXml" }
            , { "header_12", "battleType" }
        };

        ///
        ///
        ///
        static constexpr std::array<std::string_view, 17> headerArray{
            "id"
            , "size"
            , "validity"
            , "hasMods"
            , "respawn"
            , "duration"
            , "winnerTeam"
            , "dateTime"
            , "playerVehicle"
            , "mapName"
            , "playerName"
            , "mapDisplayName"
            , "replayName"
            , "playerID"
            , "clientVersionFromXml"
            , "arenaCreateTime"
            , "battleType"
        };

    };







private:            // PRIVATE VARIABLES

    ///
    ///
    ///
    static constexpr size_t minimumInSize_t{};

    ///
    ///
    ///
    static constexpr size_t maximumInSize_t_{ 100 };

    ///
    ///
    ///
    static inline std::map<const std::string, std::string> bd_{ WSTR::Settings::Default::bd };

    ///
    ///
    ///
    static inline std::map<const std::string, std::string> headerList_{ WSTR::Settings::Default::header };

    ///
    ///
    ///
    static inline std::map<const std::string, std::string> pathsList_{};


private:            // PRIVATE STATIC FUNCTIONS


    ///
    /// \brief checkDirExists
    /// \param pathDir
    /// \return
    ///
    static bool checkDirExists(std::string_view pathDir);

    ///
    /// \brief settingFileName
    /// \return
    ///
    static inline const std::string settingFileName();

    ///
    /// \brief saveToFile
    /// \param in
    /// \return
    ///
    static bool saveToFile(std::stringstream& buffer);

    ///
    /// \brief checkIsHeaderValue
    /// \param value
    /// \return
    ///
    static bool checkIsHeaderValue(std::string_view value);

    ///
    /// \brief checkHeaderItem
    /// \param key
    /// \param value
    /// \return
    ///
    static bool checkHeaderItem(std::string_view key, std::string_view value);

    ///
    /// \brief selectBase
    /// \param sb
    /// \return
    ///
    static std::map<const std::string, std::string>* selectBase(WSTR::SelectBase sb = WSTR::SelectBase::General);

private:            // PRIVATE FUNCTIONS

    ///
    /// \brief parse
    /// \param str
    /// \return
    ///
    static std::pair<std::pair<std::string, std::string>, bool> parseLine(const std::string& str);

    ///
    /// \brief parse
    /// \param configBuffer
    /// \return
    ///
    static bool parse(const std::vector<std::string>& configBuffer);

    ///
    /// \brief checkValue
    /// \param key
    /// \param value
    /// \param sb
    /// \return
    ///
    template <typename KeyType, typename ValueType>
    static bool checkValue(KeyType&& key, ValueType&& value, WSTR::SelectBase sb = WSTR::SelectBase::General);


public:             // PUBLIC FUNCTIONS

    Settings();
    ~Settings();


    ///
    /// \brief save
    /// \return
    ///
    static bool save();

    ///
    /// \brief load
    /// \return
    ///
    static bool load();

    ///
    /// \brief loadFromDefault
    ///
    static void loadFromDefault();

    ///
    /// \brief getValue
    /// \param key
    /// \param sb
    /// \return
    ///
    template <typename Type = std::string>
    static std::pair<std::decay_t<Type>, bool>
    getValue(const std::string& key, WSTR::SelectBase sb = WSTR::SelectBase::General);

    ///
    /// \brief setValue
    /// \param key
    /// \param value
    /// \param sb
    ///
    template <typename KeyType = std::string, typename ValueType = std::string>
    static bool
    setValue(KeyType&& key, ValueType&& value, WSTR::SelectBase sb = WSTR::SelectBase::General);

    ///
    /// \brief getDefaultPath
    /// \return
    ///
    static const QString getDefaultPath();

    ///
    /// \brief qListToSSBufer
    /// \param bufer
    /// \param list
    ///
    static void PathFromQComboBoxToPathsBufer(const QComboBox& list);

    ///
    /// \brief getVersionApp
    /// \return
    ///
    static const std::string getVersionApp();

    ///
    /// \brief toType
    /// \param str
    /// \return
    ///
    template <typename OutType, typename Type>
    static std::pair< OutType, bool > toType(Type&& str);

    ///
    /// \brief checkIsRange
    /// \param begin
    /// \param end
    /// \param item
    /// \return
    ///
    template <typename RangeBegin, typename RangeEnd, typename ItemType>
    static bool checkIsRange(RangeBegin&& begin, RangeEnd&& end, ItemType&& item);

    ///
    /// \brief getCountHeaderList
    /// \return
    ///
    static size_t getCountHeaderList();

};


//////////////////////////// implementation of class functions /////////////////////////////////////////////

///
/// \brief Settings::getValue
/// \param key
/// \param sb
/// \return
///
template <typename Type>
std::pair<std::decay_t<Type>, bool> Settings
::getValue(const std::string& key, WSTR::SelectBase sb){
    using type = std::decay_t<Type>;

    auto base{ selectBase(sb) };

    if(!base) return { type(), false };
    try{
        if constexpr(std::is_same_v<type, std::string>){
            return { base->at(key.data()), true };
        }
        else {
            std::stringstream ss;
            ss << base->at(key.data());
            type tmp{};
            if(ss >> tmp) return { tmp, true };
            return { type() , false };
        }
    }
    catch(std::out_of_range ex){
        return { type(), false };
    }

}

///
/// \brief Settings::setValue
/// \param key
/// \param value
/// \param sb
/// \return
///
template <typename KeyType, typename ValueType>
bool Settings
::setValue(KeyType&& key, ValueType&& value, WSTR::SelectBase sb ){
    std::map<const std::string, std::string>* base{ selectBase(sb) };
    if(!base) return false;

    auto&& [_, isKey] = getValue(key, sb);
    if(isKey){
        (*base)[std::forward<KeyType>(key)] = std::forward<ValueType>(value);
    }
    else{
        base->insert( {std::forward<KeyType>(key), std::forward<ValueType>(value)} );
    }
    return true;
}

///
/// \brief Settings::checkValue
/// \param key
/// \param value
/// \param sb
/// \return
///
template<typename KeyType, typename ValueType>
bool Settings::checkValue(KeyType&& key, ValueType&& value, SelectBase sb)
{
    auto&& [newValue, isGet] = getValue(std::forward<KeyType>(key), sb);

    if(!isGet){
        std::stringstream ss;
        ss << "bool Settings::checkValue(KeyType&& key, ValueType&& value, SelectBase sb)\n";
        ss << "auto&& [newValue, isGet] = getValue(key, sb);\n";
        ss << "isGet == FALSE\n";

        WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
        return false;
    }
    if(newValue != std::forward<ValueType>(value)) {
        std::stringstream ss;
        ss << "bool Settings::checkValue(KeyType&& key, ValueType&& value, SelectBase sb)\n";
        ss << "auto&& [newValue, isGet] = getValue(key, sb);\n";
        ss << "(newValue == value) == FALSE \n";

        WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
        return false;
    }
    return true;
}

///
/// \brief Settings::toType
/// \param str
/// \return
///
template <typename OutType, typename Type>
std::pair< OutType, bool > Settings
::toType(Type&& str){
    std::stringstream ss;
    ss << std::forward<Type>(str);
    if(ss.fail()) return { OutType(), false };

    OutType tmp{};
    ss >> tmp;
    if(ss.fail()) return { OutType(), false };

    return { tmp, true };
}



template <typename RangeBegin, typename RangeEnd, typename ItemType>
bool Settings
::checkIsRange(RangeBegin&& begin, RangeEnd&& end, ItemType&& item){
    using decay_begin = std::decay_t<RangeBegin>;
    using decay_end = std::decay_t<RangeEnd>;
    using decay_item = std::decay_t<ItemType>;

    static_assert(std::is_same_v<decay_begin, decay_end>
            && std::is_same_v<decay_begin, decay_item>, "RangeBegin && RangeEnd && ItemType --> Must be of the same type!");
    return item >= begin && item <= end;
}


}



#endif
