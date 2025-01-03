#include "foundation.h"


class Instrument{

};
using InstrumentPtr = std::shared_ptr<Instrument>;

class InstrumentManager{
public:
    InstrumentManager();
    ~InstrumentManager();
    static InstrumentManager& getInstance(){
        static InstrumentManager m_instance;
        return m_instance;
    }

    Result init(const std::vector<std::string> symbols);
    void stop();
    void submit(const std::string& symbol, std::shared_ptr<BtcBinaryHeader> binaryHeader);

private:
    std::unordered_map<std::string, InstrumentPtr> instruments_map_;
};