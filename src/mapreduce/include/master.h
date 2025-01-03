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

    // you can make a subclass to rewrite cut for your cut method
    class default_cut
    {
    public:
        default_cut(uint64_t volum = 2000)
        {
            m_volum = volum;
        }

        virtual std::string cut(std::string file_path)
        {
            std::string line;
            std::ifstream file(file_path);
            std::string res;
            file.seekg(index, std::ios::beg);
            while (std::getline(file, line))
            {
                res.append(line);
                if (res.size() >= m_volum)
                {
                    break;
                }
            }

            if (res.size() == 0)
            {
                index = 0;
                return res;
            }
            index += res.size();

            return res;
        }

        uint64_t m_volum;
        uint64_t index = 0;
    };

    class master
    {
    public:
        master(std::string outfile, default_cut *cut_method = nullptr);
        ~master();

        void work();

        void connect();

        void onConnection(const muduo::net::TcpConnectionPtr &conn);
        void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time);

        void serverConnection(const muduo::net::TcpConnectionPtr &conn);
        void serverMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time);

        // check map and reduce
        void reduce_zk();

    private:
        muduo::net::EventLoop *m_loop;
        muduo::net::TcpServer *m_server;
        ZkClient m_zkclient;
        std::string m_buffer;
        std::unordered_map<std::string, muduo::net::TcpClient *> connection;

        std::thread *reduce_check;
        std::vector<std::string> reduces;
        std::vector<std::pair<std::string, uint32_t>> error_reduces;

        std::thread *data_work;
        std::vector<std::string> files;
        std::vector<std::pair<uint32_t, std::string>> cut_data;

        uint32_t send_index = 0;

        // std::function<std::string(std::string)> m_func; //(file_path)

        default_cut *m_cut_method;

        std::string m_outfile;

        bool over = false;
    };

}
