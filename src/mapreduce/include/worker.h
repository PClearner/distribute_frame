#pragma once

#include "rpc/include/init.h"
#include "rpc/include/zookeeperutil.h"
#include "include/muduo/net/TcpServer.h"
#include "include/muduo/net/TcpClient.h"
#include "include/muduo/net/EventLoop.h"
#include <functional>
#include <tuple>

namespace star
{

    class work
    {
    public:
        work();
        ~work();
        virtual void deal(const muduo::net::TcpConnectionPtr &conn) {}

        muduo::net::EventLoop *m_loop;
        ZkClient m_zkclient;
        std::string m_buffer;
        std::string m_ip;
        std::string m_port;

        void onConnection(const muduo::net::TcpConnectionPtr &conn);
        void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time);
    };

    class map : public work
    {
    public:
        map(std::string id, std::function<std::vector<std::string>(std::string)> func);
        ~map();

        void deal(const muduo::net::TcpConnectionPtr &conn) override;

        inline void set_map(std::function<std::vector<std::string>(std::string)> func)
        {
            m_func = func;
        }

    private:
        std::function<std::vector<std::string>(std::string)> m_func;
        // record reduce ip and port
        std::unordered_map<std::string, std::tuple<std::string, std::string>> rc_record;
        std::unordered_map<std::string, std::vector<std::string>> rc_data;
        std::unordered_map<std::string, std::string> rc_log;

        std::mutex m_mtx;
        std::string m_id;
        muduo::net::TcpServer *m_server;
        bool complete = false;
        bool over = false;
    };

    class reduce : public work
    {
    public:
        reduce(std::string id, std::function<std::unordered_map<std::string, uint64_t>(std::string)> m_func);
        ~reduce();

        void deal(const muduo::net::TcpConnectionPtr &conn) override;

        inline void set_reduce(std::function<std::unordered_map<std::string, uint64_t>(std::string)> func)
        {
            m_func = func;
        }

        void masteronConnection(const muduo::net::TcpConnectionPtr &conn);
        void masteronMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time);

    private:
        std::function<std::unordered_map<std::string, uint64_t>(std::string)> m_func;
        std::unordered_map<std::string, muduo::net::TcpClient *> connection;

        std::unordered_map<std::string, uint64_t> m_data;
        bool over = false;

        std::mutex m_mtx;
        std::string m_id;
    };

}