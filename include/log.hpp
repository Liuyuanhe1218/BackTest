#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__
#include <cstdarg>

#include <fstream>
#include <iostream>

#include <log4cxx/logger.h>
#include <log4cxx/stream.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/ndc.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/net/xmlsocketappender.h>
#include <log4cxx/net/socketappenderskeleton.h>
#include <iomanip>
#include <filesystem>
#include <boost/algorithm/string.hpp>


#include <cstdarg>
#include <fstream>
#include <iostream>
#include <log4cxx/logger.h>
#include <log4cxx/stream.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <iomanip>
#include <filesystem>

namespace utils
{
  namespace logging
  {
    using namespace log4cxx;
    using namespace log4cxx::helpers;

    // Define the root logger for general logging
    static LoggerPtr logger_(Logger::getRootLogger());

    // Simple log function for general logging
    static const char* log_format(const char* fmt, ...)
    {
      va_list va;
      thread_local static char formatted[4096];
      va_start(va, fmt);
      vsnprintf(formatted, sizeof(formatted), fmt, va);
      va_end(va);
      return formatted;
    }

    // Configure log4cxx (main log file only)
    inline void config(const std::string& logFilePath, const std::string& logFileConfig)
    {
      // Read the configuration file
      std::ifstream is(logFileConfig);
      if (!is.good())
      {
        throw std::runtime_error(std::string("Warning: failed to read log configure file: ") + logFileConfig);
      }
      else
      {
        // Ensure log directory exists
        const std::string Path = "./logs/";
        std::filesystem::path log_directory(Path);
        if (!std::filesystem::exists(log_directory))
        {
          std::filesystem::create_directory(log_directory);
        }

        // Prepare temp file for configuring log4cxx
        char tmpf[] = "./logs/fileXXXXXX";
        int fd = mkstemp(tmpf);
        if (fd == -1)
          throw std::runtime_error("failed to create tmp file");

        FILE* fh = fdopen(fd, "wt");
        std::string line;
        while (std::getline(is, line))
        {
          if (line.find("<LOG_FILE_PATH>") != std::string::npos)
          {
            fprintf(fh, "%s\n", boost::replace_all_copy(line, "<LOG_FILE_PATH>", logFilePath).c_str());
          }
          else
          {
            fprintf(fh, "%s\n", line.c_str());
          }
        }
        fclose(fh);

        // Configure log4cxx using the temporary configuration file
        log4cxx::PropertyConfigurator::configure(tmpf);

        // Clean up
        remove(tmpf);
      }
    }
  }
}

// Define simple logging macros
#define LOG_INFO(msg)  LOG4CXX_INFO(utils::logging::logger_, std::fixed << std::setprecision(4) << msg);
#define LOG_WARN(msg)  LOG4CXX_WARN(utils::logging::logger_, "\33[0;33m" << std::fixed << std::setprecision(4) << msg << "\33[0m");
#define LOG_ERROR(msg) LOG4CXX_ERROR(utils::logging::logger_, "\33[0;31m" << std::fixed << std::setprecision(4) << msg << "\33[0m");
#define LOG_FATAL(msg) { LOG4CXX_FATAL(utils::logging::logger_, "\33[0;31m" << msg << "\33[0m"); usleep(100); abort();}


#endif

