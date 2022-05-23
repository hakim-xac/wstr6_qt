#ifndef SETTINGS_H
#define SETTINGS_H

#include "./ui_mainwindow.h"
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
    size_t numberOfPaths_{};


    bool isLoad_{};
    std::queue<std::string> base_;
    static constexpr std::string_view settingFileName{ "./settings.dat" };
    std::map<std::string, std::string> bd_{
        { "numberOfPaths", "5" }
        , { "numberOfPa", "7" }
    };
private:

    bool saveToFile(std::stringstream& in);
    std::pair<std::pair<std::string, std::string>, bool> parse(const std::string& str);
public:
    Settings() = default;
    bool isLoad() const;
    bool save();
    bool load();

    template <typename Type = std::string>
    std::pair<Type, bool> getValue(std::string key);

    template <typename Type = std::string>
    void setValue(Type&& key, Type&& value );
};



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


template <typename Type> void Settings
::setValue(Type&& key, Type&& value ){
    auto&& [_, isKey] = getValue(key);
    if(isKey){
        bd_[std::forward<Type>(key)] = std::forward<Type>(value);
    }
    else{
        bd_.insert( {std::forward<Type>(key), std::forward<Type>(value)} );
    }
}


}



#endif
