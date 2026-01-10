#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <iostream>

enum class LogLevel
{
    Debug,
    Info,
    Warning,
    Error
};

class Logger
{
public:
    static Logger& instance()
    {
        static Logger inst;
        return inst;
    }

    void enableConsole(bool enable) { logToConsole = enable; }
    void enableFile(const std::string& filename)
    {
        fileStream.open(filename, std::ios::out | std::ios::app);
        logToFile = fileStream.is_open();
    }

    template<typename... Args>
    void log(LogLevel level, Args&&... args)
    {
        std::scoped_lock lock(mutex);

        std::string prefix = levelToString(level);

        std::ostringstream oss;
        (oss << ... << args); // fold expression

        if (logToConsole)
            std::cout << prefix << oss.str() << std::endl;

        if (logToFile)
            fileStream << prefix << oss.str() << std::endl;
    }

private:
    Logger() = default;
    ~Logger()
    {
        if (fileStream.is_open())
            fileStream.close();
    }

    std::string levelToString(LogLevel level)
    {
        switch(level)
        {
            case LogLevel::Debug: return "[DEBUG] ";
            case LogLevel::Info: return "[INFO] ";
            case LogLevel::Warning: return "[WARN] ";
            case LogLevel::Error: return "[ERROR] ";
        }
        return "";
    }

    bool logToConsole = true;
    bool logToFile = false;
    std::ofstream fileStream;
    std::mutex mutex;
};

#define LOG_DEBUG(...) Logger::instance().log(LogLevel::Debug, __VA_ARGS__)
#define LOG_INFO(...)  Logger::instance().log(LogLevel::Info,  __VA_ARGS__)
#define LOG_WARN(...)  Logger::instance().log(LogLevel::Warning, __VA_ARGS__)
#define LOG_ERROR(...) Logger::instance().log(LogLevel::Error, __VA_ARGS__)