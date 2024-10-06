#pragma once

#include <iostream>
#include <queue>
#include "tools/include/lock.h"
#include <sstream>
#include <chrono>
#include <cassert>
#include <fstream>
#include <functional>
#include <atomic>

namespace star
{

    enum Output
    {
        print,
        write,
        all
    };

    class Logger
    {
    public:
        typedef std::shared_ptr<Logger> ptr;

        enum Level
        {
            Debug,
            Info,
            Error,
            Fatal
        };

        // 默认构造只生成打印选择
        Logger();
        Logger(std::string file, Output model = Output::print, Level level_threshold = Level::Debug);
        ~Logger();

        std::string format(std::string str, Logger::Level level);
        void logwork(std::string str, Logger::Level level);

        inline void set_model(Output o)
        {
            m_model = o;
        }

        inline std::string get_model(Output o) const
        {
            if (o == Output::all)
            {
                return "all";
            }
            else if (o == Output::print)
            {
                return "print";
            }
            else if (o == Output::write)
            {
                return "write";
            }
        }

        inline void set_file(std::string file)
        {
            m_file = file;
        }

        inline std::string get_file() const
        {
            return m_file;
        }

        void restart();
        void killlogger();

        static void run(Logger *obj);

    private:
        std::string m_file = "";
        std::queue<std::string> logqueue;
        Output m_model;
        Level m_level_threshold;
        std::ofstream ofs;
        std::thread m_thread;
        std::atomic<bool> state;
    };
    class LoggerWrapper
    {
    public:
        LoggerWrapper(Logger *logger, Logger::Level level_threshold);

        template <typename T>
        LoggerWrapper &operator<<(const T &value)
        {
            ss << value;
            return *this;
        }

        ~LoggerWrapper();

    private:
        Logger *m_logger;
        std::stringstream ss;
        Logger::Level m_level_threshold;
    };

    Logger *getMainLogger();

}

#define LOG_LEVEL(logger, level) star::LoggerWrapper(logger, level)
#define LOG_INFO(logger) LOG_LEVEL(logger, star::Logger::Level::Info)
#define LOG_DEBUG(logger) LOG_LEVEL(logger, star::Logger::Level::Debug)
#define LOG_ERROR(logger) LOG_LEVEL(logger, star::Logger::Level::Error)
#define LOG_FATAL(logger) LOG_LEVEL(logger, star::Logger::Level::Fatal)

#define MAIN_LOGGER() star::getMainLogger()
#define LOG_MAIN_INFO LOG_INFO(MAIN_LOGGER())
#define LOG_MAIN_DEBUG LOG_DEBUG(MAIN_LOGGER())
#define LOG_MAIN_ERROR LOG_ERROR(MAIN_LOGGER())
#define LOG_MAIN_FATAL LOG_FATAL(MAIN_LOGGER())
