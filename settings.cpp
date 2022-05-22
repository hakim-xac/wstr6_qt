#include "settings.h"
#include <string>
#include <sstream>
#include <fstream>

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

}
