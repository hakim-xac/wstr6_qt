#ifndef SETTINGS_H
#define SETTINGS_H
#include <string_view>
#include <string>
#include <sstream>
#include <queue>
#include <map>

namespace WSTR {

class Settings{
    size_t numberOfPaths_{};


    bool isLoad_{};
    std::queue<std::string> base_;
    static constexpr std::string_view settingFileName{ "./settings.dat" };
    std::map<std::string_view, std::string_view> bd_{
        { "numberOfPaths", "5" }
        , { "numberOfPa", "7" }
    };
private:

    bool saveToFile(std::stringstream& in);
public:
    Settings() = default;
    bool isLoad() const;
    bool save();
};



}



#endif
