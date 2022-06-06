#ifndef SETTINGS_H
#define SETTINGS_H

#include "./logs.h"
#include <string_view>
#include <string>
#include <sstream>
#include <queue>
#include <map>
#include <QDir>
#include <QCoreApplication>
#include <QComboBox>
#include <array>

namespace WSTR {


class Settings{

    struct Default{

        ///
        ///
        ///
        const QString defaultPath_{ QCoreApplication::applicationDirPath() };

        ///
        ///
        ///
        const std::map<std::string, std::string> bd_{
            { "countOfPaths", "0" }
            , { "currentPathIndex", "0" }
        };

        const std::map<std::string, std::string> header_{
            { "header_1", "id" }
            , { "header_2", "size" }
            , { "header_3", "validity" }
            , { "header_4", "hasmods" }
            , { "header_5", "respawn" }
            , { "header_6", "mapname" }
            , { "header_7", "testname" }
        };

        static constexpr std::array<std::string_view, 17> headerArray_{
            "id"
            , "size"
            , "validity"
            , "hasmods"
            , "respawn"
            , "duration"
            , "winnerteam"
            , "datetime"
            , "vehicle"
            , "mapname"
            , "username"
            , "testname"
            , "replayname"
            , "clientversion"
        };


    };







private:            // PRIVATE VARIABLES


    ///
    /// \brief default_
    ///
    Default default_;


    ///
    ///
    ///
    size_t countOfPaths_;

    ///
    /// \brief minimumInSize_t_
    ///
    static constexpr size_t minimumInSize_t{};

    ///
    /// \brief maximumInSize_t_
    ///
    static constexpr size_t maximumInSize_t_{ 100 };

    ///
    /// \brief base_
    ///
    ///
    std::queue<std::string> base_;

    ///
    ///
    ///
    std::map<std::string, std::string> bd_;

    ///
    /// \brief pathsList_
    ///
    std::map<std::string, std::string> pathsList_;

    ///
    /// \brief headerList_
    ///
    std::map<std::string, std::string> headerList_;


private:            // PRIVATE STATIC VARIABLES AND FUNCTIONS

    ///
    ///
    ///
    static constexpr std::string_view settingFileName_{ "./settings.dat" };

    ///
    ///
    static constexpr std::string_view versionApplication_{ "6.0.0.0" };

    ///
    /// \brief checkDirExists
    /// \param pathDir
    /// \return
    ///
    static bool checkDirExists(std::string_view pathDir);

private:            // PRIVATE FUNCTIONS
    ///
    /// \brief saveToFile
    /// \param in
    /// \return
    ///
    bool saveToFile(std::stringstream& buffer) const;

    ///
    /// \brief parse
    /// \param str
    /// \return
    ///
    std::pair<std::pair<std::string, std::string>, bool> parseLine(const std::string& str);

    ///
    /// \brief parse
    /// \param configBuffer
    /// \return
    ///
    bool parse(const std::vector<std::string>& configBuffer);

    ///
    /// \brief checkValue
    /// \param key
    /// \param value
    /// \param sb
    /// \return
    ///
    template <typename KeyType, typename ValueType>
    bool checkValue(KeyType&& key, ValueType&& value, WSTR::SelectBase sb = WSTR::SelectBase::General);

    ///
    /// \brief selectBase
    /// \param sb
    /// \return
    ///
    std::map<std::string, std::string>* selectBase(WSTR::SelectBase sb = WSTR::SelectBase::General);

    ///
    /// \brief checkHeaderItem
    /// \param key
    /// \param value
    /// \return
    ///
    bool checkHeaderItem(std::string_view key, std::string_view value);

    ///
    /// \brief checkIsHeaderValue
    /// \param value
    /// \return
    ///
    bool checkIsHeaderValue(std::string_view value);


public:             // PUBLIC VARIABLES
    Logs logs{ std::cout };

public:             // PUBLIC FUNCTIONS

    Settings();
    ~Settings();


    ///
    /// \brief save
    /// \return
    ///
    bool save() const;

    ///
    /// \brief load
    /// \return
    ///
    bool load();

    ///
    /// \brief getValue
    /// \param key
    /// \param sb
    /// \return
    ///
    template <typename Type = std::string>
    std::pair<Type, bool>
    getValue(const std::string& key, WSTR::SelectBase sb = WSTR::SelectBase::General);

    ///
    /// \brief setValue
    /// \param key
    /// \param value
    /// \param sb
    ///
    template <typename KeyType = std::string, typename ValueType = std::string>
    bool
    setValue(KeyType&& key, ValueType&& value, WSTR::SelectBase sb = WSTR::SelectBase::General);

    ///
    /// \brief getDefaultPath
    /// \return
    ///
    QString getDefaultPath() const;

    ///
    /// \brief qListToSSBufer
    /// \param bufer
    /// \param list
    ///
    void PathFromQComboBoxToPathsBufer(const QComboBox& list);

    ///
    /// \brief getVersionApp
    /// \return
    ///
    std::string_view getVersionApp();

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
    size_t getCountHeaderList();

};


//////////////////////////// implementation of class functions /////////////////////////////////////////////

///
/// \brief Settings::getValue
/// \param key
/// \param sb
/// \return
///
template <typename Type>
std::pair<Type, bool> Settings
::getValue(const std::string& key, WSTR::SelectBase sb){
    auto base{ selectBase(sb) };
    if(!base) return { Type(), false };
    try{
        if constexpr(std::is_same_v<Type, std::string>){
            return { base->at(key.data()), true };
        }
        else {
            std::stringstream ss;
            ss << base->at(key.data());
            Type tmp{};
            if(ss >> tmp) return { tmp, true };
            return { Type() , false };
        }
    }
    catch(std::out_of_range ex){
        return { Type(), false };
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
    std::map<std::string, std::string>* base{ selectBase(sb) };
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
        std::stringstream ss{ "bool Settings::checkValue(KeyType&& key, ValueType&& value, SelectBase sb)\n" };
        ss << "auto&& [newValue, isGet] = getValue(key, sb);\n";
        ss << "isGet == FALSE\n";
        logs.pushAndFlash(ss.str(), WSTR::AppType::Debug);
        return false;
    }
    if(newValue != std::forward<ValueType>(value)) {
        std::stringstream ss{ "bool Settings::checkValue(KeyType&& key, ValueType&& value, SelectBase sb)\n" };
        ss << "auto&& [newValue, isGet] = getValue(key, sb);\n";
        ss << "(newValue == value) == FALSE \n";
        logs.pushAndFlash(ss.str(), WSTR::AppType::Debug);
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
