#include "settings.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <iostream>    // for log test. std::cout

namespace WSTR {

///
/// \brief Settings::saveToFile
/// \param buffer
/// \return
///
bool Settings::saveToFile(std::stringstream& buffer) const
{
    std::ofstream fs(settingFileName.data());
    if(!fs.is_open()) return false;
    fs.clear();
    if(!(fs << buffer.str())){
        fs.close();
        return false;
    }
    fs.close();
    return true;
}

///
/// \brief Settings::parse
/// \param str
/// \return
///
std::pair<std::pair<std::string, std::string>, bool> Settings::parse(const std::string& str)
{
    size_t pos{ str.find(':') };
    if(pos == std::string::npos || (pos+1) == std::string::npos) return {{"", "" }, false};
    return { std::make_pair(str.substr(0, pos), str.substr(pos+1)) , true };
}

///
/// \brief Settings::selectBase
/// \param sb
/// \return
///
std::map<std::string, std::string> *Settings::selectBase(SelectBase sb)
{
    switch(sb){
    case WSTR::SelectBase::General:
    return &bd_;
    break;
    case WSTR::SelectBase::Paths:
    return &pathsList_;
    break;
    default:
        return nullptr;
    }
}

///
/// \brief Settings::PathFromQComboBoxToPathsBufer
/// \param list
///
void Settings::PathFromQComboBoxToPathsBufer(const QComboBox& list)
{
    using namespace std::string_literals;
    pathsList_.clear();
    for(int it{}, end{ list.count() }; it < end; ++it){
        setValue("path_"s+std::to_string(it), list.itemText(it).toStdString(), WSTR::SelectBase::Paths);
    }
}

///
/// \brief Settings::Settings
///
Settings::Settings()
    : default_()
    , countOfPaths_()
    , base_()
    , bd_(default_.bd_)
    , pathsList_() {}


///
/// \brief Settings::~Settings
///
Settings::~Settings()
{
    save();
}


///
/// \brief Settings::save
/// \return
///
bool Settings::save() const
{
    if(bd_.empty()) return false;
    std::stringstream ss;
    for(auto&& [key, value]: bd_){
        ss << key << ":" << value << "\n";
    }

    for(auto&& [key, value]: pathsList_){
        ss << key << ":" << value << "\n";
    }

    return saveToFile(ss);
}

///
/// \brief Settings::load
/// \return
///
bool Settings::load()
{

    logs.pushAndFlash("Start Settings.load()", WSTR::AppType::Debug);

    std::ifstream fs(settingFileName.data());
    if(!fs.is_open()){
        std::stringstream ss{"Can`t open settings file: "};
        ss << settingFileName.data();
        logs.pushAndFlash(ss.str(), WSTR::AppType::Debug);
        return false;
    }
    std::vector <std::string> tmpVecConfig;

    std::copy(std::istream_iterator<std::string>(fs), {}, std::back_inserter(tmpVecConfig));
    if(!tmpVecConfig.size()) {

        logs.pushAndFlash("Empty configuration file", WSTR::AppType::Debug);

        fs.close();
        return false;
    }
    for(auto&& line: tmpVecConfig){
        auto&& [pair, isParse] = parse(line);
        if(!isParse){

            logs.pushAndFlash("Parse(line) == false", WSTR::AppType::Debug);

            fs.close();
            return false;
        }
        auto&& [key, value] = pair;

        using namespace std::string_literals;
        WSTR::SelectBase sb{ WSTR::SelectBase::General };

        if(key.substr(0, 5) == "path_"s) {
            if(!setValue(key, value, WSTR::SelectBase::Paths)){
                std::stringstream ss{ "if(!setValue(key, value, WSTR::SelectBase::Paths)) == false\n" };
                logs.pushAndFlash(ss.str(), WSTR::AppType::Debug);
                return false;
            }
            sb = WSTR::SelectBase::Paths;
        }
        else {
            if(!setValue(key, value)){
                std::stringstream ss{ "if(!setValue(key, value)) == false\n" };
                logs.pushAndFlash(ss.str(), WSTR::AppType::Debug);
                return false;
            }
            sb = WSTR::SelectBase::General;
        }
        auto&& [newValue, isGet] = getValue(key, sb);
        if(!isGet){

            logs.pushAndFlash("not load after saveToBase", WSTR::AppType::Debug);

            fs.close();
            return false;
        }
        if(newValue != value) {

            logs.pushAndFlash("error saved after saveToBase", WSTR::AppType::Debug);

            fs.close();
            return false;
        }
        logs.pushAndFlash("successful loading settings\nkey: " + key + " value: " + value, WSTR::AppType::Debug);
    }
    fs.close();
    logs.pushAndFlash("successful loading settings", WSTR::AppType::Debug);
    return true;
}

///
/// \brief Settings::getDefaultPath
/// \return
///
QString Settings::getDefaultPath() const
{
    return default_.defaultPath_;
}






}
