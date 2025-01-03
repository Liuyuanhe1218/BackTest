#pragma once
#include "foundation.h"

class DataLoader{
public:
    DataLoader();
    ~DataLoader();
    static DataLoader& getInstance(){
        static DataLoader m_instance;
        return m_instance;
    }

    Result init(std::vector<std::string> dates, std::vector<std::string> symbols);
    void start();
    void stop();                    

private:
    std::atomic<bool> running_;
    std::set<std::string> dates_;
    std::vector<std::string> symbols_;
};