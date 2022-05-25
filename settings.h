#ifndef SETTINGS_H
#define SETTINGS_H

#include "./ui_mainwindow.h"
#include "./logs.h"
#include <string_view>
#include <string>
#include <sstream>
#include <queue>
#include <map>

namespace WSTR {

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Settings{

    ///
    ///
    ///
    size_t numberOfPaths_{};


    ///
    /// \brief base_
    ///
    ///
    std::queue<std::string> base_;

    ///
    ///
    ///
    static constexpr std::string_view settingFileName{ "./settings.dat" };

    ///
    ///
    ///
    std::map<std::string, std::string> bd_{
        { "numberOfPaths", "5" }
        , { "numberOfPa", "7" }
    };
private:
    ///
    /// \brief saveToFile
    /// \param in
    /// \return
    ///
    bool saveToFile(std::stringstream& buffer);

    ///
    /// \brief parse
    /// \param str
    /// \return
    ///
    std::pair<std::pair<std::string, std::string>, bool> parse(const std::string& str);


public:
    Logs logs{ std::cout };
public:
    Settings() = default;


    ///
    /// \brief save
    /// \return
    ///
    bool save();

    ///
    /// \brief load
    /// \return
    ///
    bool load();

    ///
    /// \brief getValue
    /// \param key
    /// \return
    ///
    template <typename Type = std::string>
    std::pair<Type, bool>
    getValue(std::string key);

    ///
    /// \brief setValue
    /// \param key
    /// \param value
    ///
    template <typename KeyType = std::string, typename ValueType = std::string>
    void
    setValue(KeyType&& key, ValueType&& value );
};


//////////////////////////// implementation of class functions /////////////////////////////////////////////


template <typename Type>
std::pair<Type, bool> Settings
::getValue(std::string key){
    try{
        if constexpr(std::is_same_v<Type, std::string>){
            return { bd_.at(key.data()), true };
        }
        else {
            std::stringstream ss;
            ss << bd_.at(key.data());
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
void Settings
::setValue(KeyType&& key, ValueType&& value ){
    auto&& [_, isKey] = getValue(key);
    if(isKey){
        bd_[std::forward<KeyType>(key)] = std::forward<ValueType>(value);
    }
    else{
        bd_.insert( {std::forward<KeyType>(key), std::forward<ValueType>(value)} );
    }
}


}



#endif
