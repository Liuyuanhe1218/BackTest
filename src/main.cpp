
#include "log.hpp"
#include "foundation.h"
#include "config.h"

void signalHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        // LOG_INFO("\nReceived SIGINT or SIGTERM. Exiting gracefully...");
        // CrawlerManager::getInstance().stop();
        // sleep(1);
        // NetWorkManager::getInstance().stop();
    } 
}

int main(int argc, char* argv[]) {
    utils::logging::config("./log/", "./conf/log4cxx.properties");
    LOG_INFO("Start init backtest ...");

    // // 处理信号
    // std::signal(SIGINT, signalHandler);
    // std::signal(SIGTERM, signalHandler);

    // // 初始化配置管理器
    // if (ConfigManger::getInstance().init(configPath.c_str()) != Result::SUCCESS) {
    //     std::cerr << "Failed to initialize config manager with path: " << configPath << std::endl;
    //     return -1;
    // }

    // // 初始化网络管理器
    // if (NetWorkManager::getInstance().init(std::stoi(ConfigManger::getInstance().getGonfig(key_io_thread_num))) != Result::SUCCESS) {
    //     std::cerr << "Failed to initialize network manager." << std::endl;
    //     return -1;
    // }

    // // 初始化爬虫管理器
    // if (CrawlerManager::getInstance().init(ConfigManger::getInstance().getGonfigs(key_symbols)) != Result::SUCCESS) {
    //     std::cerr << "Failed to initialize crawler manager." << std::endl;
    //     return -1;
    // }

    // // 启动爬虫管理器
    // CrawlerManager::getInstance().start(); // 阻塞住，等待信号退出
    return 0;
}
