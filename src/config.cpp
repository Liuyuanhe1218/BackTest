#include "config.h"

const std::string key_https_url = "https_url";
const std::string key_https_port = "https_port";
const std::string key_https_target_prifix = "https_target_prifix";
const std::string key_wss_url = "wss_url";
const std::string key_wss_port = "wss_port";
const std::string key_wss_target_prifix = "wss_target_prifix";
const std::string key_io_thread_num = "io_thread_num";
const std::string key_depth_snapshot_limit = "depth_snapshot_limit";
const std::string key_data_path = "data_path";

const std::string key_data_type = "data_type";
const std::string key_symbols = "symbols";

const std::vector<std::string> KeysConfigSingle{
    key_https_url,
    key_https_port, 
    key_https_target_prifix, 
    key_wss_url,
    key_wss_port,
    key_wss_target_prifix,
    key_io_thread_num,
    key_depth_snapshot_limit,
    key_data_path
};

const std::vector<std::string> KeysConfigVector{
    key_data_type,
    key_symbols
};

Result ConfigManger::init(const std::string& config_path){
    std::ifstream ifs(config_path);
    if (!ifs.is_open()) {
        LOG_ERROR("Failed to open file : " << config_path);
        return Result::FAILURE;
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document document;
    if (document.ParseStream(isw).HasParseError()) {
        std::cerr << "JSON 解析失败！" << std::endl;
        return Result::FAILURE;
    }

    for(auto& singleKey : KeysConfigSingle){
        if(document.HasMember(singleKey.c_str()) == false){
            LOG_ERROR("Failed to get config : " << singleKey);
            return Result::FAILURE;
        }
        configSingle_[singleKey] = document[singleKey.c_str()].GetString();
        LOG_INFO("Insert single config : " << singleKey << ", " << document[singleKey.c_str()].GetString());
    }

    for(auto& vectorKey: KeysConfigVector){
        if(document.HasMember(vectorKey.c_str()) == false){
            LOG_ERROR("Failed to get config : " << vectorKey);
            return Result::FAILURE;
        }

        const auto& values = document[vectorKey.c_str()];
        for (rapidjson::SizeType i = 0; i < values.Size(); ++i) {
            if (values[i].IsString()) {
                configVector_[vectorKey].push_back(values[i].GetString());
                LOG_INFO(vectorKey << "Insert config vector "<< vectorKey <<  " " << i << " : " << values[i].GetString());
            }
        }         
    }

    LOG_INFO("Success to init ConfigManger.");
    return Result::SUCCESS;
}

std::string ConfigManger::getGonfig(const std::string& key){
    return this->configSingle_[key];
}
std::vector<std::string> ConfigManger::getGonfigs(const std::string& key){
    return this->configVector_[key];
}
