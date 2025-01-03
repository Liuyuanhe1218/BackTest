#pragma once
#include <fstream>
#include <unordered_map>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include "log.hpp"
#include "foundation.h"

extern const std::string key_https_url;
extern const std::string key_https_port;
extern const std::string key_https_target_prifix;
extern const std::string key_wss_url;
extern const std::string key_wss_port;
extern const std::string key_wss_target_prifix;
extern const std::string key_io_thread_num;
extern const std::string key_depth_snapshot_limit;
extern const std::string key_data_path;
extern const std::string key_data_type;
extern const std::string key_symbols;

extern const std::vector<std::string> KeysConfigSingle;
extern const std::vector<std::string> KeysConfigVector;


class ConfigManger{
public:
    static ConfigManger& getInstance(){
        static ConfigManger m_instance;  //局部静态变量
        return m_instance;
    }

    Result init(const std::string& config_path);

    std::string getGonfig(const std::string& key);
    std::vector<std::string> getGonfigs(const std::string& key);

    std::unordered_map<std::string, std::string> configSingle_;
    std::unordered_map<std::string, std::vector<std::string>> configVector_;
};
