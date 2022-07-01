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
#include <set>

namespace KHAS {


class Settings{

public:
    ///
    /// \brief getFieldName
    /// \param fn
    /// \return
    ///
    template <typename TypeInMap = std::string>
    static const TypeInMap getFieldName(KHAS::FieldNames fn);


private:
    struct Default{


        ///
        ///
        ///
        static inline std::map<KHAS::FieldNames, const std::string> fieldNames{
            { KHAS::FieldNames::ActiveColumn,               "activeColumn"          }
            , { KHAS::FieldNames::TypeSortColumns,          "typeSortColumns"       }
            , { KHAS::FieldNames::CurrentPathIndex,         "currentPathIndex"      }
            , { KHAS::FieldNames::CountOfPaths,             "countOfPaths"          }
            , { KHAS::FieldNames::WaitUpdateStatusBar_s,    "waitUpdateStatusBar_s" }
        };

        ///
        ///
        ///
        static inline const std::map<const std::string, std::string> bd{
            { KHAS::Settings::getFieldName(KHAS::FieldNames::CountOfPaths),             "0" }
            , { KHAS::Settings::getFieldName(KHAS::FieldNames::CurrentPathIndex),       "0" }
            , { KHAS::Settings::getFieldName(KHAS::FieldNames::WaitUpdateStatusBar_s),  "2" }
            , { KHAS::Settings::getFieldName(KHAS::FieldNames::ActiveColumn),           "0" }
            , { KHAS::Settings::getFieldName(KHAS::FieldNames::TypeSortColumns),        "0" }
        };

        ///
        ///
        ///
        static inline const std::map<const std::string, std::string> header{
            { "header_1", "id"                      }
            , { "header_2", "size"                  }
            , { "header_3", "validity"              }
            , { "header_4", "hasMods"               }
            , { "header_5", "respawn"               }
            , { "header_6", "mapName"               }
            , { "header_7", "mapDisplayName"        }
            , { "header_8", "playerName"            }
            , { "header_9", "playerVehicle"         }
            , { "header_10", "playerID"             }
            , { "header_11", "clientVersionFromXml" }
            , { "header_12", "battleType"           }
        };

        ///
        ///
        ///
        static inline const std::set<std::string_view> headerSet{
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
    static inline std::map<const std::string, std::string> bd_{ KHAS::Settings::Default::bd };

    ///
    ///
    ///
    static inline std::map<const std::string, std::string> headerList_{ KHAS::Settings::Default::header };

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
    template <typename OutType>
    static std::map<const OutType, OutType>* selectBase(KHAS::SelectBase sb = KHAS::SelectBase::General);

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
    static bool checkValue(KeyType&& key, ValueType&& value, KHAS::SelectBase sb = KHAS::SelectBase::General) noexcept;


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
    template <typename Type = std::string, typename KeyType>
    static std::pair<std::decay_t<Type>, bool>
    getValue(KeyType&& key, KHAS::SelectBase sb = KHAS::SelectBase::General);

    ///
    /// \brief setValue
    /// \param key
    /// \param value
    /// \param sb
    ///
    template <typename KeyType = std::string, typename ValueType = std::string>
    static bool
    setValue(KeyType&& key, ValueType&& value, KHAS::SelectBase sb = KHAS::SelectBase::General);

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
    /// \brief stringToType
    /// \param str
    /// \return
    ///
    template <typename OutType, typename InType>
    static std::pair< OutType, bool > stringToType(InType&& str);

    ///
    /// \brief checkIsRange
    /// \param begin
    /// \param end
    /// \param item
    /// \return
    ///
    template <typename RangeBegin, typename RangeEnd, typename ItemType>
    static bool checkIsRange(RangeBegin&& begin, RangeEnd&& end, ItemType&& item) noexcept;

    ///
    /// \brief getCountHeaderList
    /// \return
    ///
    static size_t getCountHeaderList();

};


//////////////////////////// implementation of class functions /////////////////////////////////////////////


template <typename TypeInMap>
inline const TypeInMap Settings::getFieldName(FieldNames fn)
{
    using mapped_type = decltype(Default::fieldNames)::mapped_type;
    static_assert(std::is_same_v<mapped_type, const TypeInMap>, "gfjgfjfgjf");

    return Default::fieldNames[fn];
}

template<typename OutType>
std::map<const OutType, OutType>*Settings
::selectBase(SelectBase sb)
{
    using bdKey             = decltype(bd_)::key_type;
    using bdValue           = decltype(bd_)::mapped_type;
    using pathsListKey      = decltype(pathsList_)::key_type;
    using pathsListValue    = decltype(pathsList_)::mapped_type;
    using headerListKey     = decltype(headerList_)::key_type;
    using headerListValue   = decltype(headerList_)::mapped_type;

    using outType           = std::decay_t<OutType>;

    static_assert(std::is_same_v<bdKey, const outType>
            && std::is_same_v<bdValue, outType>
            && std::is_same_v<pathsListKey, const outType>
            && std::is_same_v<pathsListValue, outType>
            && std::is_same_v<headerListKey, const outType>
            && std::is_same_v<headerListValue, outType>
            , "Map keys must be type T constants, values must be type T");

    switch(sb){
    case KHAS::SelectBase::General:
    return &bd_;
    break;
    case KHAS::SelectBase::Paths:
    return &pathsList_;
    break;
    case KHAS::SelectBase::Headers:
    return &headerList_;
    break;

    default:
        return nullptr;
    }

}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

template <typename Type, typename KeyType>
std::pair<std::decay_t<Type>, bool> Settings
::getValue(KeyType&& key, KHAS::SelectBase sb){

    using type = std::decay_t<Type>;

    auto base{ selectBase<std::string>(sb) };

    if(!base) return { type(), false };
    try{
        if constexpr(std::is_same_v<type, std::string>){
            return { base->at(std::forward<KeyType>(key)), true };
        }
        else {
            std::stringstream ss;
            ss << base->at(std::forward<KeyType>(key));
            type tmp{};
            if(ss >> tmp) return { tmp, true };
            return { type() , false };
        }
    }
    catch(std::out_of_range ex){
        return { type(), false };
    }

}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

template <typename KeyType, typename ValueType>
bool Settings
::setValue(KeyType&& key, ValueType&& value, KHAS::SelectBase sb ){
    auto base{ selectBase<std::string>(sb) };
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

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

template<typename KeyType, typename ValueType>
bool Settings::checkValue(KeyType&& key, ValueType&& value, SelectBase sb) noexcept
{
    auto&& [newValue, isGet] = getValue(std::forward<KeyType>(key), sb);

    if(!isGet){
        std::stringstream ss;
        ss << "bool Settings::checkValue(KeyType&& key, ValueType&& value, SelectBase sb)\n";
        ss << "auto&& [newValue, isGet] = getValue(key, sb);\n";
        ss << "isGet == FALSE\n";

        KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
        return false;
    }
    if(newValue != std::forward<ValueType>(value)) {
        std::stringstream ss;
        ss << "bool Settings::checkValue(KeyType&& key, ValueType&& value, SelectBase sb)\n";
        ss << "auto&& [newValue, isGet] = getValue(key, sb);\n";
        ss << "(newValue == value) == FALSE \n";

        KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

template <typename OutType, typename InType>
std::pair< OutType, bool > Settings
::stringToType(InType&& str){

    std::stringstream ss;
    ss << std::forward<InType>(str);
    if(ss.fail()) return { OutType(), false };

    OutType tmp{};
    ss >> tmp;
    if(ss.fail()) return { OutType(), false };

    return { tmp, true };
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

template <typename RangeBegin, typename RangeEnd, typename ItemType>
bool Settings
::checkIsRange(RangeBegin&& begin, RangeEnd&& end, ItemType&& item) noexcept
{

    using decay_begin = std::decay_t<RangeBegin>;
    using decay_end = std::decay_t<RangeEnd>;
    using decay_item = std::decay_t<ItemType>;

    static_assert(std::is_same_v<decay_begin, decay_end>
            && std::is_same_v<decay_begin, decay_item>
            , "RangeBegin && RangeEnd && ItemType --> Must be of the same type!");

    auto begin__{ std::forward<RangeBegin>(begin) };
    auto end__{ std::forward<RangeBegin>(end) };
    auto item__{ std::forward<RangeBegin>(item) };

    return item__ >= begin__ && item__ <= end__;
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////





}



#endif
