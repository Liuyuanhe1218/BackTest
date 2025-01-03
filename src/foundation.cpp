#include "foundation.h"


long getCurrentTimestampMillis() {
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();
    // 转换为 Unix 时间点
    auto duration = now.time_since_epoch();
    // 转换为毫秒
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return millis;
}

void toUpperCase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::toupper(c);
    });
}