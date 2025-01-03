#include "binary.h"
#include <memory>
#include <algorithm>

std::shared_ptr<BtcBinaryHeader> generateBinaryHeaderFromJson(std::shared_ptr<MsgStruct> msg){

    return nullptr;
    rapidjson::Document document;
    document.Parse(msg->msg->c_str());

    // 检查是否成功解析
    if (document.HasParseError()) {
        LOG_ERROR("JSON 解析失败!");
        return nullptr;
    }

    std::shared_ptr<BtcBinaryHeader> ptr = nullptr;
    if(document.HasMember("e")){
        std::string typeStr = document["e"].GetString();

        if(typeStr == "trade"){
            auto derived = std::make_shared<BtcBinaryTrade>();
            derived->Maker = document["m"].GetDouble();;
            derived->TradeID;
            derived->Market = document["M"].GetDouble();
            derived->Price = document["p"].GetDouble();
            derived->Volume = document["q"].GetDouble();
            derived->E = document["E"].GetInt64();;
            derived->Symbol;
            derived->Time = msg->recv;
            derived->T = document["T"].GetInt64();;

        }else if(typeStr == "depthUpdate"){

        }
    }else{// bookTicker消息
        auto derived = std::make_shared<BtcBinaryBookTicker>();
        //{"e":"trade","E":1735570463915,"s":"BTCUSDT","t":4353981756,"p":"92250.01000000","q":"0.00419000","T":1735570463914,"m":false,"M":true}
        derived->E = 0;
        std::string symbol = document["s"].GetString();
        std::copy(symbol.begin(), symbol.end(), derived->Symbol);
        derived->Symbol[symbol.size()] = '\0'; 
        derived->u = document["u"].GetInt64();
        derived->AskPrice = document["a"].GetDouble();
        derived->AskVolume = document["A"].GetDouble();
        derived->BidPrice = document["b"].GetDouble();
        derived->BidVolume = document["B"].GetDouble();
        derived->Time = msg->recv;

        ptr = derived;
    }
    //{"u":57519699855,"s":"BTCUSDT","b":"92250.00000000","B":"6.56234000","a":"92250.01000000","A":"4.93016000"}
    return ptr;
}