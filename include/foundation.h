#pragma once
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include "binary.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include "rapidjson/document.h"
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <set>
#include <atomic>

enum Result{
    FAILURE,
    SUCCESS
};

long getCurrentTimestampMillis();

void toUpperCase(std::string& str);