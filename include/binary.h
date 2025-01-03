#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdint.h>
#include "log.hpp"
#include <memory>
#include <fstream>
#include <rapidjson/document.h>
struct MsgStruct{
    MsgStruct(uint64_t time, std::shared_ptr<std::string> jsonMsg):recv(time), msg(jsonMsg){}
    uint64_t recv;
    std::shared_ptr<std::string> msg;
};

enum BtcBinaryType : uint8_t {
    BIN_DEPTH = 1,              // 深度行情更新(100ms)
    BIN_DEPTH_SNAPSHOT = 2,     // 深度行情快照(http请求)
    BIN_TRADE = 3,              // 逐笔交易(实时)
    BIN_BOOK_TICKER = 4,        // 当前最优买卖价格(实时)

    BIN_TYPE_CNT
};


struct BtcBinaryHeader {
    char Symbol[32];    // 币对名称
    uint64_t Time;      // 接收时间
    uint64_t E;         //  事件时间
    uint64_t T;         //  交易时间
    uint8_t Type;       // 类型代码

    ~BtcBinaryHeader(){}

    virtual void serialize(std::ostream& os){
        os.write(Symbol, sizeof(Symbol));
        os.write(reinterpret_cast<const char*>(&Time), sizeof(Time));
        os.write(reinterpret_cast<const char*>(&E), sizeof(E));
        os.write(reinterpret_cast<const char*>(&T), sizeof(T));
        os.write(reinterpret_cast<const char*>(&Type), sizeof(Type));
    }

    // 基类虚函数，用于反序列化
    virtual void deserialize(std::istream& is) {
        is.read(Symbol, sizeof(Symbol));
        is.read(reinterpret_cast<char*>(&Time), sizeof(Time));
        is.read(reinterpret_cast<char*>(&E), sizeof(E));
        is.read(reinterpret_cast<char*>(&T), sizeof(T));
        is.read(reinterpret_cast<char*>(&Type), sizeof(Type));
    }
};

struct BtcBinaryDepth : BtcBinaryHeader {// 成交
    uint64_t U;    // 
    uint64_t u;   // 
    uint64_t pu;   // 

    uint64_t BidCount;
    uint64_t AskCount;
    double* BidPrice;
    double* BidVol;
    double* AskPrice;
    double* AskVol;


    ~BtcBinaryDepth(){
        delete[] BidPrice;
        delete[] BidVol;
        delete[] AskPrice;
        delete[] AskVol;
    }

    BtcBinaryDepth(){ this->Type = 1; }

    void serialize(std::ostream& os) const {
        os.write(Symbol, sizeof(Symbol));
        os.write(reinterpret_cast<const char*>(&Time), sizeof(Time));
        os.write(reinterpret_cast<const char*>(&E), sizeof(E));
        os.write(reinterpret_cast<const char*>(&T), sizeof(T));
        os.write(reinterpret_cast<const char*>(&Type), sizeof(Type));
        os.write(reinterpret_cast<const char*>(&U), sizeof(U));
        os.write(reinterpret_cast<const char*>(&u), sizeof(u));
        os.write(reinterpret_cast<const char*>(&pu), sizeof(pu));
        os.write(reinterpret_cast<const char*>(&BidCount), sizeof(BidCount));
        os.write(reinterpret_cast<const char*>(&AskCount), sizeof(AskCount));
        os.write(reinterpret_cast<const char*>(BidPrice), BidCount * sizeof(double));
        os.write(reinterpret_cast<const char*>(BidVol), BidCount * sizeof(double));
        os.write(reinterpret_cast<const char*>(AskPrice), AskCount * sizeof(double));
        os.write(reinterpret_cast<const char*>(AskVol), AskCount * sizeof(double));
    }

    void deserialize(std::istream& is) {
        is.read(Symbol, sizeof(Symbol));
        is.read(reinterpret_cast<char*>(&Time), sizeof(Time));
        is.read(reinterpret_cast<char*>(&E), sizeof(E));
        is.read(reinterpret_cast<char*>(&T), sizeof(T));
        is.read(reinterpret_cast<char*>(&Type), sizeof(Type));
        is.read(reinterpret_cast<char*>(&U), sizeof(U));
        is.read(reinterpret_cast<char*>(&u), sizeof(u));
        is.read(reinterpret_cast<char*>(&pu), sizeof(pu));
        is.read(reinterpret_cast<char*>(&BidCount), sizeof(BidCount));
        is.read(reinterpret_cast<char*>(&AskCount), sizeof(AskCount));

        BidPrice = new double[BidCount];
        BidVol = new double[BidCount];
        AskPrice = new double[AskCount];
        AskVol = new double[AskCount];

        is.read(reinterpret_cast<char*>(BidPrice), BidCount * sizeof(double));
        is.read(reinterpret_cast<char*>(BidVol), BidCount * sizeof(double));
        is.read(reinterpret_cast<char*>(AskPrice), AskCount * sizeof(double));
        is.read(reinterpret_cast<char*>(AskVol), AskCount * sizeof(double));
    }
};

struct BtcBinaryDepthSnapShot : BtcBinaryHeader {// 成交
    uint64_t lastUpdateId;

    uint64_t BidCount;
    uint64_t AskCount;
    double* BidPrice;
    double* BidVol;
    double* AskPrice;
    double* AskVol;

    ~BtcBinaryDepthSnapShot(){
        delete[] BidPrice;
        delete[] BidVol;
        delete[] AskPrice;
        delete[] AskVol;
    }

    BtcBinaryDepthSnapShot(){ this->Type = 2; }

    void serialize(std::ostream& os) const {
        os.write(Symbol, sizeof(Symbol));
        os.write(reinterpret_cast<const char*>(&Time), sizeof(Time));
        os.write(reinterpret_cast<const char*>(&E), sizeof(E));
        os.write(reinterpret_cast<const char*>(&T), sizeof(T));
        os.write(reinterpret_cast<const char*>(&Type), sizeof(Type));
        os.write(reinterpret_cast<const char*>(&lastUpdateId), sizeof(lastUpdateId));
        os.write(reinterpret_cast<const char*>(&BidCount), sizeof(BidCount));
        os.write(reinterpret_cast<const char*>(&AskCount), sizeof(AskCount));
        os.write(reinterpret_cast<const char*>(BidPrice), BidCount * sizeof(double));
        os.write(reinterpret_cast<const char*>(BidVol), BidCount * sizeof(double));
        os.write(reinterpret_cast<const char*>(AskPrice), AskCount * sizeof(double));
        os.write(reinterpret_cast<const char*>(AskVol), AskCount * sizeof(double));
    }

    void deserialize(std::istream& is) {
        is.read(Symbol, sizeof(Symbol));
        is.read(reinterpret_cast<char*>(&Time), sizeof(Time));
        is.read(reinterpret_cast<char*>(&E), sizeof(E));
        is.read(reinterpret_cast<char*>(&T), sizeof(T));
        is.read(reinterpret_cast<char*>(&Type), sizeof(Type));
        is.read(reinterpret_cast<char*>(&lastUpdateId), sizeof(lastUpdateId));
        is.read(reinterpret_cast<char*>(&BidCount), sizeof(BidCount));
        is.read(reinterpret_cast<char*>(&AskCount), sizeof(AskCount));

        BidPrice = new double[BidCount];
        BidVol = new double[BidCount];
        AskPrice = new double[AskCount];
        AskVol = new double[AskCount];

        is.read(reinterpret_cast<char*>(BidPrice), BidCount * sizeof(double));
        is.read(reinterpret_cast<char*>(BidVol), BidCount * sizeof(double));
        is.read(reinterpret_cast<char*>(AskPrice), AskCount * sizeof(double));
        is.read(reinterpret_cast<char*>(AskVol), AskCount * sizeof(double));
    }
};

struct BtcBinaryTrade : BtcBinaryHeader {// 成交
    uint64_t TradeID;// 交易ID
    double Price;    // 价格 
    double Volume;   // 数量
    bool Market;     // True为市价单(MARKET)，False为限价单(LIMIT)
    bool Maker;      // True为买方是做市商， False是非做市商

    BtcBinaryTrade(){ this->Type = 3; }

    void serialize(std::ostream& os) const {
        os.write(Symbol, sizeof(Symbol));
        os.write(reinterpret_cast<const char*>(&Time), sizeof(Time));
        os.write(reinterpret_cast<const char*>(&E), sizeof(E));
        os.write(reinterpret_cast<const char*>(&T), sizeof(T));
        os.write(reinterpret_cast<const char*>(&Type), sizeof(Type));
        os.write(reinterpret_cast<const char*>(&TradeID), sizeof(TradeID));
        os.write(reinterpret_cast<const char*>(&Price), sizeof(Price));
        os.write(reinterpret_cast<const char*>(&Volume), sizeof(Volume));
        os.write(reinterpret_cast<const char*>(&Market), sizeof(Market));
        os.write(reinterpret_cast<const char*>(&Maker), sizeof(Maker));
    }

    void deserialize(std::istream& is) {
        is.read(Symbol, sizeof(Symbol));
        is.read(reinterpret_cast<char*>(&Time), sizeof(Time));
        is.read(reinterpret_cast<char*>(&E), sizeof(E));
        is.read(reinterpret_cast<char*>(&T), sizeof(T));
        is.read(reinterpret_cast<char*>(&Type), sizeof(Type));
        is.read(reinterpret_cast<char*>(&TradeID), sizeof(TradeID));
        is.read(reinterpret_cast<char*>(&Price), sizeof(Price));
        is.read(reinterpret_cast<char*>(&Volume), sizeof(Volume));
        is.read(reinterpret_cast<char*>(&Market), sizeof(Market));
        is.read(reinterpret_cast<char*>(&Maker), sizeof(Maker));
    }
};


struct BtcBinaryBookTicker : BtcBinaryHeader {// 成交
    uint64_t u;             //更新IO

    double AskPrice;      //卖单最低报价
    double BidPrice;      //卖单最低报价量
    double AskVolume;     //买单最高报价
    double BidVolume;     //买单最高报价量
    BtcBinaryBookTicker(){ this->Type = 4; }


    void serialize(std::ostream& os) const {
        os.write(Symbol, sizeof(Symbol));
        os.write(reinterpret_cast<const char*>(&Time), sizeof(Time));
        os.write(reinterpret_cast<const char*>(&E), sizeof(E));
        os.write(reinterpret_cast<const char*>(&T), sizeof(T));
        os.write(reinterpret_cast<const char*>(&Type), sizeof(Type));
        os.write(reinterpret_cast<const char*>(&u), sizeof(u));
        os.write(reinterpret_cast<const char*>(&AskPrice), sizeof(AskPrice));
        os.write(reinterpret_cast<const char*>(&BidPrice), sizeof(BidPrice));
        os.write(reinterpret_cast<const char*>(&AskVolume), sizeof(AskVolume));
        os.write(reinterpret_cast<const char*>(&BidVolume), sizeof(BidVolume));
    }


    void deserialize(std::istream& is) {
        is.read(Symbol, sizeof(Symbol));
        is.read(reinterpret_cast<char*>(&Time), sizeof(Time));
        is.read(reinterpret_cast<char*>(&E), sizeof(E));
        is.read(reinterpret_cast<char*>(&T), sizeof(T));
        is.read(reinterpret_cast<char*>(&Type), sizeof(Type));
        is.read(reinterpret_cast<char*>(&u), sizeof(u));
        is.read(reinterpret_cast<char*>(&AskPrice), sizeof(AskPrice));
        is.read(reinterpret_cast<char*>(&BidPrice), sizeof(BidPrice));
        is.read(reinterpret_cast<char*>(&AskVolume), sizeof(AskVolume));
        is.read(reinterpret_cast<char*>(&BidVolume), sizeof(BidVolume));
    }
};


std::shared_ptr<BtcBinaryHeader> generateBinaryHeaderFromJson(std::shared_ptr<MsgStruct> msg);