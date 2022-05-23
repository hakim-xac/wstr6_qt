#include "settings.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <iostream>    // for log test. std::cout

namespace WSTR {


bool Settings::saveToFile(std::stringstream& in)
{
    std::ofstream fs(settingFileName.data());
    if(!fs.is_open()) return false;
    fs.clear();
    if(!(fs << in.str())){
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


bool Settings::isLoad() const
{
    return isLoad_;
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
    std::ifstream fs(settingFileName.data());
    if(!fs.is_open()) return false;
    std::vector <std::string> tmpVecConfig;

    std::copy(std::istream_iterator<std::string>(fs), {}, std::back_inserter(tmpVecConfig));
    if(!tmpVecConfig.size()) {
        std::cout << "Empty configuration file" << std::endl;
        fs.close();
        return false;
    }
    for(auto&& line: tmpVecConfig){
        auto&& [pair, isParse] = parse(line);
        if(!isParse){
            std::cout << "Parse(line) == false" << std::endl;
            fs.close();
            return false;
        }
        auto&& [key, value] = pair;

        setValue(key, value);

        auto&& [newValue, isGet] = getValue(key);
        if(!isGet){
            std::cout << "not load after saveToBase" << std::endl;
            fs.close();
            return false;
        }
        if(newValue != value) {
            std::cout << "error saved after saveToBase" << std::endl;
            fs.close();
            return false;
        }
        std::cout << "all good"<< std::endl;
    }
    fs.close();
    return true;
}






}
