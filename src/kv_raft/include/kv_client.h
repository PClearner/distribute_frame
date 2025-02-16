#pragma once

#include "tools/include/lock.h"
#include "rpc/include/init.h"
#include "kv_raft/include/raftmessage.pb.h"

#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/net/TcpClient.h"

namespace star
{
    class KVclient
    {
    public:
        KVclient();
        ~KVclient() {}

        bool send_command(std::string command);

        void clientonConnection(const muduo::net::TcpConnectionPtr &conn);
        void clientonMessage(const muduo::net::TcpConnectionPtr &conn,
                             muduo::net::Buffer *buffer, muduo::Timestamp time);

        inline void runEventLoop()
        {
            m_loop->loop(); // 启动事件循环
        }

        inline bool ifsuccess()
        {
            return success;
        }

    private:
        std::string leaderid = "";
        std::map<std::string, std::pair<std::string, std::string>> nodes_record;
        bool start = false;
        bool over = false;

        muduo::net::TcpConnectionPtr m_conn;
        muduo::net::TcpClient *m_connect;

        uint64_t index;
        std::string m_command;

        bool success;

        muduo::net::EventLoop *m_loop;
        std::thread *loopthread;
    };

}
