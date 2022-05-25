#include "settings.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <iostream>    // for log test. std::cout

namespace WSTR {


bool Settings::saveToFile(std::stringstream& buffer)
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


std::pair<std::pair<std::string, std::string>, bool> Settings::parse(const std::string& str)
{
    size_t pos{ str.find(':') };
    if(pos == std::string::npos || (pos+1) == std::string::npos) return {{"", "" }, false};
    return { std::make_pair(str.substr(0, pos), str.substr(pos+1)) , true };
}


bool Settings::save()
{
    if(bd_.empty()) return false;
    std::stringstream ss;
    for(auto&& [key, value]: bd_){
        ss << key << ":" << value << "\n";
    }

    return saveToFile(ss);
}

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

        setValue(key, value);

        auto&& [newValue, isGet] = getValue(key);
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






}
