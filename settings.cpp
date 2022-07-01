#include "settings.h"
#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <iostream>    // for log test. std::cout

namespace KHAS {


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

Settings::Settings()
{

}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

Settings::~Settings()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

bool Settings::saveToFile(std::stringstream& buffer)
{
    std::ofstream fs(KHAS::Settings::settingFileName().data());
    if(!fs.is_open()) return false;
    fs.clear();
    if(!(fs << buffer.str())){
        fs.close();
        return false;
    }
    fs.close();
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

std::pair<std::pair<std::string, std::string>, bool> Settings::parseLine(const std::string& str)
{
    size_t pos{ str.find(':') };
    if(pos == std::string::npos || (pos+1) == std::string::npos) return {{ "", "" }, false};
    return { std::make_pair(str.substr(0, pos), str.substr(pos+1)) , true };
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

bool Settings::parse(const std::vector<std::string> &configBuffer)
{
    using namespace std::literals;

    for(auto&& line: configBuffer){
        auto&& [pair, isParse] = parseLine(line);
        if(!isParse){
            KHAS::Logs::pushAndFlash("Parse(line) == false", KHAS::AppType::Debug);
            return false;
        }

        auto&& [key, value] = pair;
        KHAS::SelectBase sb{ KHAS::SelectBase::General };

        if(key.starts_with("path_"sv)) {
            if(!checkDirExists(value)){
                std::stringstream ss;
                ss << "if(!checkDirExists(value)) == false\n";
                ss << "value: " << value << std::endl;
                KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
                continue;
            }
            if(!setValue(key, value, KHAS::SelectBase::Paths)){
                std::stringstream ss{ "if(!setValue(key, value, KHAS::SelectBase::Paths)) == false\n" };
                KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
                return false;
            }
            sb = KHAS::SelectBase::Paths;
        }
        else if(key.starts_with("header_"sv)) {

            if(!checkHeaderItem(key, value)){
                headerList_.clear();
                headerList_ = KHAS::Settings::Default::header;

                std::stringstream ss;
                ss << "if(!checkHeaderItem(key, value)) == false\n";
                ss << "key: " << key << " value: " << value << std::endl;
                KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
                return false;
            }

            if(!setValue(key, value, KHAS::SelectBase::Headers)){
                std::stringstream ss{ "if(!setValue(key, value, KHAS::SelectBase::Headers)) == false\n" };
                KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
                return false;
            }
            sb = KHAS::SelectBase::Headers;
        }
        else {

            if(!setValue(key, value)){
                std::stringstream ss{ "if(!setValue(key, value)) == false\n" };
                KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
                return false;
            }

            auto&& [val, isType] = stringToType<size_t>(value);
            if(!isType) {
                std::stringstream ss{ "auto&& [val, isType] = stringToType<int>(value);\
                                      if(!isType) == false\n" };
                KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
                return false;
            }

            if(!checkIsRange(0ull, 100ull, val)){
                std::stringstream ss{ "if(!checkIsRange(0, 100, value)) == false\n" };
                KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
                return false;
            }
            sb = KHAS::SelectBase::General;
        }

        if(!checkValue(key, value, sb)){
            std::stringstream ss{ "if(!checkValue(key, value, sb)) == false\n" };
            KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
            return false;
        }

        KHAS::Logs::pushAndFlash("successful loading settings\nkey: " + key + "\nvalue: " + value, KHAS::AppType::Debug);
    }
    return true;
}

    //////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////

bool Settings::checkHeaderItem(std::string_view key, std::string_view value)
{
    using namespace std::literals;
    auto pos{ key.find_first_of('_') };
    if(pos == std::string_view::npos) return false;
    auto s{ key.substr(0, pos + 1) };
    if (s != "header_"sv) return false;
    auto&& [elem, isType] = stringToType<size_t>(key.substr(pos+1));
    if(!isType) return false;
    if(!checkIsRange(minimumInSize_t, maximumInSize_t_, elem)) return false;
    if(!checkIsHeaderValue(value)) return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

bool Settings::checkDirExists(std::string_view pathDir)
{
    QDir dir{ pathDir.data() };
    return dir.exists();
}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void Settings::PathFromQComboBoxToPathsBufer(const QComboBox& list)
{
    using namespace std::string_literals;
    pathsList_.clear();
    for(int it{}, end{ list.count() }; it < end; ++it){
        setValue("path_"s+std::to_string(it), list.itemText(it).toStdString(), KHAS::SelectBase::Paths);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

const std::string Settings::getVersionApp(){
    return "6.0.0.0";
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

size_t Settings::getCountHeaderList()
{
    return headerList_.size();
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

bool Settings::checkIsHeaderValue(std::string_view value)
{
    return Default::headerSet.find(value) != Default::headerSet.end();
}

///
/// \brief Settings::settingFileName
/// \return
///
const std::string Settings::settingFileName()
{
    return "./settings.dat";
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

bool Settings::save()
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

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

bool Settings::load()
{

    KHAS::Logs::pushAndFlash("Start Settings.load()", KHAS::AppType::Debug);

    std::ifstream fs(settingFileName());

    if(!fs.is_open()){
        std::stringstream ss;
        ss << "Can`t open settings file: ";
        ss << settingFileName();
        KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
        return false;
    }
    std::vector <std::string> tmpVecConfig;

    std::copy(std::istream_iterator<std::string>(fs), {}, std::back_inserter(tmpVecConfig));
    if(!tmpVecConfig.size()) {

        KHAS::Logs::pushAndFlash("Empty configuration file", KHAS::AppType::Debug);

        fs.close();
        return false;
    }

    if(!parse(tmpVecConfig)) {
        KHAS::Logs::pushAndFlash("if(!parse(tmpVecConfig)) == true", KHAS::AppType::Debug);
        fs.close();
        return false;
    }

    fs.close();
    KHAS::Logs::pushAndFlash("successful loading settings", KHAS::AppType::Debug);
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void Settings::loadFromDefault()
{
    bd_ = Default::bd;
    headerList_ = Default::header;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

const QString Settings::getDefaultPath()
{
    return QCoreApplication::applicationDirPath();
}






}
