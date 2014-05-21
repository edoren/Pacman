#ifndef GAME_SETTINGS_HPP
#define GAME_SETTINGS_HPP

#include <unordered_map>
#include <typeindex>
#include <string>

#include "jansson.h"


class Settings {
 public:
    Settings(const std::string& file_path);
    ~Settings();

    // If the key does't exist in the settings file, save default value
    // Return value given a key, but if any error happen return the default value.
    int getSetting(const std::string& key, const int& default_value);
    double getSetting(const std::string& key, const double& default_value);
    std::string getSetting(const std::string& key, const std::string& default_value);

    // Add a new setting to the file, but if the file contains errors it ignore it.
    void setSetting(const std::string& key, const int& value);
    void setSetting(const std::string& key, const double& value);
    void setSetting(const std::string& key, const std::string& value);

    // Save only if the file hasn't any error.
    int save();

 private:
    json_t* root_;
    FILE* file_;
    std::string file_path_;

    std::unordered_map<std::string, json_t*> settings_;
};

#endif  // GAME_SETTINGS_HPP
