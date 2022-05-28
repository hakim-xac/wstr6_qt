#ifndef SETTINGS_H
#define SETTINGS_H

#include "./ui_mainwindow.h"
#include "./logs.h"
#include <string_view>
#include <string>
#include <sstream>
#include <queue>
#include <map>
#include <QDir>

namespace WSTR {

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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


private:            // PRIVATE STATIC VARIABLES

    ///
    ///
    ///
    static constexpr std::string_view settingFileName{ "./settings.dat" };

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
    std::pair<std::pair<std::string, std::string>, bool> parse(const std::string& str);

    ///
    /// \brief selectBase
    /// \param sb
    /// \return
    ///
    std::map<std::string, std::string>* selectBase(WSTR::SelectBase sb = WSTR::SelectBase::General);


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
    getValue(std::string key, WSTR::SelectBase sb = WSTR::SelectBase::General);

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
};


//////////////////////////// implementation of class functions /////////////////////////////////////////////


template <typename Type>
std::pair<Type, bool> Settings
::getValue(std::string key, WSTR::SelectBase sb){
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


}



#endif
