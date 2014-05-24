#include "Engine/Settings.hpp"

Settings::Settings(const std::string& file_path) {
    json_error_t error;

    file_path_ = file_path;
    file_ = fopen(file_path.c_str(), "a+");
    if (!file_) {  // Check if it can open the file.
        fprintf(stderr, "Can't open the settings file.\n");
    } else if (fgetc(file_) == EOF) {  // Check if the file is empty
        fputs("{}\0", file_);
    }

    rewind(file_);
    root_ = json_loadf(file_, 0, &error);
    fclose(file_);

    if (!root_) {
        fprintf(stderr, "%s: error on line %d: %s\n", file_path_.c_str(), error.line, error.text);
    } else {
        const char* name;
        json_t* json_value;
        json_object_foreach(root_, name, json_value) {
            settings_[name] = json_value;
        }
    }
}

Settings::~Settings() {
    this->save();
    json_decref(root_);
}

int Settings::getSetting(const std::string& key, const int& default_value) {
    if (!root_) return default_value;
    else if (settings_.find(key) == settings_.end()) {
        this->setSetting(key, default_value);
        return default_value;
    }

    json_t* json_value = settings_[key];
    int ret_value = json_integer_value(json_value);

    return ret_value;
}

double Settings::getSetting(const std::string& key, const double& default_value) {
    if (!root_) return default_value;
    else if (settings_.find(key) == settings_.end()) {
        this->setSetting(key, default_value);
        return default_value;
    }

    json_t* json_value = settings_[key];
    double ret_value = json_number_value(json_value);

    return ret_value;
}

std::string Settings::getSetting(const std::string& key, const std::string& default_value) {
    if (!root_) return default_value;
    else if (settings_.find(key) == settings_.end()) {
        this->setSetting(key, default_value);
        return default_value;
    }

    json_t* json_value = settings_[key];
    std::string ret_value = json_string_value(json_value);

    return ret_value;
}

void Settings::setSetting(const std::string& key, const int& value) {
    if (!root_) return;

    json_t* json_value = json_integer(value);

    json_object_set_new(root_, key.c_str(), json_value);
    settings_[key] = json_value;
}

void Settings::setSetting(const std::string& key, const double& value) {
    if (!root_) return;

    json_t* json_value = json_real(value);

    json_object_set_new(root_, key.c_str(), json_value);
    settings_[key] = json_value;
}

void Settings::setSetting(const std::string& key, const std::string& value) {
    if (!root_) return;

    json_t* json_value = json_string(value.c_str());

    json_object_set_new(root_, key.c_str(), json_value);
    settings_[key] = json_value;
}

int Settings::save() {
    if (!root_) return -1;
    return json_dump_file(root_, file_path_.c_str(), JSON_INDENT(4) | JSON_PRESERVE_ORDER);
}
