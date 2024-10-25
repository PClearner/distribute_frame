#include "tools/include/logger.h"
#include <unistd.h>

namespace star
{

    Logger::Logger()
    {
        state = true;
        m_model = Output::print;
        m_level_threshold = Logger::Level::Debug;
        std::thread thread(Logger::run, this);
        m_thread = std::move(thread);
    }

    Logger::Logger(std::string file, Output model, Level level_threshold)
    {
        state = true;
        m_file = file;
        m_model = model;
        m_level_threshold = level_threshold;
        std::thread thread(Logger::run, this);
        m_thread = std::move(thread);
        m_thread.detach();
    }

    std::string Logger::format(std::string str, Logger::Level level)
    {
        std::thread::id this_id = std::this_thread::get_id();

        time_t now = time(nullptr);

        // 将时间转换为本地时间结构
        tm *local_tm = localtime(&now);

        // 格式化时间为字符串
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_tm);

        //[INFO][2024/10/12] thread 234234:str
        std::string lstr;

        if (level == Logger::Level::Debug)
        {
            lstr = "DEBUG";
        }
        else if (level == Logger::Level::Info)
        {
            lstr = "INFO";
        }
        else if (level == Logger::Level::Error)
        {
            lstr = "ERROR";
        }
        else if (level == Logger::Level::Fatal)
        {
            lstr = "FATAL";
        }

        std::stringstream ss;
        ss << "[" << lstr << "]" << "[" << buffer << "]" << " thread " << this_id << ":" << str;
        return ss.str();
    }

    void Logger::logwork(std::string str, Logger::Level level)
    {
        if (m_level_threshold > level)
        {
            return;
        }
        str = format(str, level);
        {
            m_mutex = std::make_shared<Locker>(new std::mutex());
            logqueue.push(str);
        }
    }

    void Logger::run(Logger *obj)
    {
        while (obj->state)
        {
            if (obj->logqueue.empty())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            if (obj->m_model == Output::print)
            {
                obj->m_mutex = std::make_shared<Locker>(new std::mutex());
                std::cout << obj->logqueue.front() << std::endl;
                obj->logqueue.pop();
            }
            else if (obj->m_model == Output::write)
            {
                assert(obj->m_file != "");
                std::string ss;
                ss = obj->logqueue.front();
                {
                    obj->m_mutex = std::make_shared<Locker>(new std::mutex());
                    obj->logqueue.pop();
                }
                obj->ofs.open(obj->m_file, std::ios::out | std::ios::app);
                obj->ofs << ss << std::endl;
                obj->ofs.close();
            }
            else if (obj->m_model == Output::all)
            {
                assert(obj->m_file != "");
                std::string ss;
                ss = obj->logqueue.front();
                {
                    obj->m_mutex = std::make_shared<Locker>(new std::mutex());
                    std::cout << ss << std::endl;
                    obj->logqueue.pop();
                }
                obj->ofs.open(obj->m_file, std::ios::out | std::ios::app);
                obj->ofs << ss << std::endl;
                obj->ofs.close();
            }
        }
        while (!obj->logqueue.empty())
        {
            if (obj->m_model == Output::print)
            {
                obj->m_mutex = std::make_shared<Locker>(new std::mutex());
                std::cout << obj->logqueue.front() << std::endl;
                obj->logqueue.pop();
            }
            else if (obj->m_model == Output::write)
            {
                assert(obj->m_file != "");
                std::string ss;
                ss = obj->logqueue.front();
                {
                    obj->m_mutex = std::make_shared<Locker>(new std::mutex());
                    obj->logqueue.pop();
                }
                obj->ofs.open(obj->m_file, std::ios::out | std::ios::app);
                obj->ofs << ss << std::endl;
                obj->ofs.close();
            }
            else if (obj->m_model == Output::all)
            {
                assert(obj->m_file != "");
                std::string ss;
                ss = obj->logqueue.front();
                {
                    obj->m_mutex = std::make_shared<Locker>(new std::mutex());
                    std::cout << ss << std::endl;
                    obj->logqueue.pop();
                }
                obj->ofs.open(obj->m_file, std::ios::out | std::ios::app);
                obj->ofs << ss << std::endl;
                obj->ofs.close();
            }
        }
        obj->state = true;
    }

    Logger::~Logger()
    {
        killlogger();
    }

    void Logger::restart()
    {
        while (state == false)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        std::cout << "restart new" << std::endl;
        std::thread thread(Logger::run, this);
        m_thread = std::move(thread);
    }

    void Logger::killlogger()
    {
        state = false;
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

    LoggerWrapper::LoggerWrapper(Logger *logger, Logger::Level level_threshold)
    {
        m_logger = logger;
        m_level_threshold = level_threshold;
    }

    LoggerWrapper::~LoggerWrapper()
    {
        m_logger->logwork(ss.str(), m_level_threshold);
    }

    static Logger::ptr main_logger = std::make_shared<Logger>();

    Logger *getMainLogger()
    {
        return main_logger.get();
    }

}