#include "settings.h"
#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <iostream>    // for log test. std::cout

namespace WSTR {



///
/// \brief Settings::Settings
///
Settings::Settings()
    : default_()
    , countOfPaths_()
    , bd_(default_.bd_)
    , pathsList_()
    , headerList_(default_.header_) {}


///
/// \brief Settings::~Settings
///
Settings::~Settings()
{
    save();
}

///
/// \brief Settings::saveToFile
/// \param buffer
/// \return
///
bool Settings::saveToFile(std::stringstream& buffer) const
{
    std::ofstream fs(settingFileName_.data());
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
std::pair<std::pair<std::string, std::string>, bool> Settings::parseLine(const std::string& str)
{
    size_t pos{ str.find(':') };
    if(pos == std::string::npos || (pos+1) == std::string::npos) return {{ "", "" }, false};
    return { std::make_pair(str.substr(0, pos), str.substr(pos+1)) , true };
}

bool Settings::parse(const std::vector<std::string> &configBuffer)
{
    using namespace std::literals;

    for(auto&& line: configBuffer){
        auto&& [pair, isParse] = parseLine(line);
        if(!isParse){
            WSTR::Logs::pushAndFlash("Parse(line) == false", WSTR::AppType::Debug);
            return false;
        }

        auto&& [key, value] = pair;
        WSTR::SelectBase sb{ WSTR::SelectBase::General };

        if(key.starts_with("path_"sv)) {
            if(!checkDirExists(value)){
                std::stringstream ss{ "if(!checkDirExists(value)) == false\n" };
                ss << "value: " << value << std::endl;
                WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
                continue;
            }
            if(!setValue(key, value, WSTR::SelectBase::Paths)){
                std::stringstream ss{ "if(!setValue(key, value, WSTR::SelectBase::Paths)) == false\n" };
                WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
                return false;
            }
            sb = WSTR::SelectBase::Paths;
        }
        else if(key.starts_with("header_"sv)) {

            if(!checkHeaderItem(key, value)){
                headerList_.clear();
                headerList_ = default_.header_;

                std::stringstream ss{ "if(!checkHeaderItem(key, value)) == false\n" };
                ss << "key: " << key << " value: " << value << std::endl;
                WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
                return false;
            }

            if(!setValue(key, value, WSTR::SelectBase::Headers)){
                std::stringstream ss{ "if(!setValue(key, value, WSTR::SelectBase::Headers)) == false\n" };
                WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
                return false;
            }
            sb = WSTR::SelectBase::Headers;
        }
        else {

            if(!setValue(key, value)){
                std::stringstream ss{ "if(!setValue(key, value)) == false\n" };
                WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
                return false;
            }
            auto&& [val, isType] = toType<size_t>(value);
            if(!isType) {
                std::stringstream ss{ "auto&& [val, isType] = toType<int>(value);\
                                      if(!isType) == false\n" };
                WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
                return false;
            }
            if(!checkIsRange(0ull, 100ull, val)){
                std::stringstream ss{ "if(!checkIsRange(0, 100, value)) == false\n" };
                WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
                return false;
            }
            sb = WSTR::SelectBase::General;
        }

        if(!checkValue(key, value, sb)){
            std::stringstream ss{ "if(!checkValue(key, value, sb)) == false\n" };
            WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
            return false;
        }

        WSTR::Logs::pushAndFlash("successful loading settings\nkey: " + key + " value: " + value, WSTR::AppType::Debug);
    }
    return true;
}

///
/// \brief Settings::selectBase
/// \param sb
/// \return
///
std::map<const std::string, std::string> *Settings::selectBase(SelectBase sb)
{
    switch(sb){
    case WSTR::SelectBase::General:
    return &bd_;
    break;
    case WSTR::SelectBase::Paths:
    return &pathsList_;
    break;
    case WSTR::SelectBase::Headers:
    return &headerList_;
    break;
    default:
        return nullptr;
    }
}

///
/// \brief Settings::checkHeaderItem
/// \param key
/// \param value
/// \return
///
bool Settings::checkHeaderItem(std::string_view key, std::string_view value)
{
    using namespace std::literals;
    auto pos{ key.find_first_of('_') };
    if(pos == std::string_view::npos) return false;
    auto s{ key.substr(0, pos + 1) };
    if (s != "header_"sv) return false;
    auto&& [elem, isType] = toType<size_t>(key.substr(pos+1));
    if(!isType) return false;
    if(!checkIsRange(minimumInSize_t, maximumInSize_t_, elem)) return false;
    if(!checkIsHeaderValue(value)) return false;

    return true;
}

///
/// \brief Settings::checkDirExists
/// \param pathDir
/// \return
///
bool Settings::checkDirExists(std::string_view pathDir)
{
    QDir dir{ pathDir.data() };
    return dir.exists();
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
/// \brief Settings::getVersionApp
/// \return
///
std::string_view Settings::getVersionApp(){
    return versionApplication_;
}

///
/// \brief Settings::getCountHeaderList
/// \return
///
size_t Settings::getCountHeaderList()
{
    return headerList_.size();
}

///
/// \brief Settings::checkIsHeaderValue
/// \param value
/// \return
///
bool Settings::checkIsHeaderValue(std::string_view value)
{
    return std::find(std::begin(default_.headerArray_), std::end(default_.headerArray_), value) != std::end(default_.headerArray_);
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

    for(auto&& [key, value]: headerList_){
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

    WSTR::Logs::pushAndFlash("Start Settings.load()", WSTR::AppType::Debug);

    std::ifstream fs(settingFileName_.data());
    if(!fs.is_open()){
        std::stringstream ss{"Can`t open settings file: "};
        ss << settingFileName_.data();
        WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
        return false;
    }
    std::vector <std::string> tmpVecConfig;

    std::copy(std::istream_iterator<std::string>(fs), {}, std::back_inserter(tmpVecConfig));
    if(!tmpVecConfig.size()) {

        WSTR::Logs::pushAndFlash("Empty configuration file", WSTR::AppType::Debug);

        fs.close();
        return false;
    }
    if(!parse(tmpVecConfig)) {
        fs.close();
        return false;
    }

    fs.close();
    WSTR::Logs::pushAndFlash("successful loading settings", WSTR::AppType::Debug);
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
