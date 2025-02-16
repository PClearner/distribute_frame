#pragma once

#include "tools/include/lock.h"
#include "rpc/include/init.h"
#include "kv_raft/include/raftmessage.pb.h"

#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/net/TcpClient.h"

#include <atomic>

namespace star
{

    class KVserver
    {
    public:
        KVserver();
        ~KVserver() {}

        void work();

        inline void runEventLoop()
        {
            m_loop->loop(); // 启动事件循环
        }

        void heardbeat();

        void serveronConnection(const muduo::net::TcpConnectionPtr &conn);
        void serveronMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time);

        void clientonConnection(const muduo::net::TcpConnectionPtr &conn);
        void clientonMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time);

        bool log_input(uint64_t term, uint64_t index, std::string command);

        // check commit logs and write them to file
        bool write_commit();

        inline uint64_t get_commitIndex()
        {
            return commitIndex;
        }

        inline uint64_t get_last_log_term()
        {
            return last_log_term;
        }

        bool deletefile();

        void loadlog();

        std::tuple<bool, std::string> do_command(std::string command);

    private:
        enum state
        {
            Leader,
            Candidate,
            Follower
        };

        // name,ip,port,connnection,ifconnect,client
        std::vector<std::tuple<std::string, std::string, std::string, muduo::net::TcpConnectionPtr,
                               bool, muduo::net::TcpClient *>>
            nodes_information;
        std::string m_ip;
        std::string m_port;

        std::unordered_map<std::string, uint64_t> nextIndex; // follower log update
        bool Leaderstart = false;
        uint64_t m_term;

        std::atomic<bool> acceptheartbeat;

        std::string m_id;

        std::string leaderid;
        state m_state;
        bool votefor;
        std::unordered_map<std::string, uint64_t> matchIndex; // follower commit log record
        uint16_t timeout;
        muduo::net::TcpServer *m_server;
        std::thread *loopthread;

        // 1 success 2 failed 3 unknown
        std::unordered_map<std::string, uint8_t> votesituation;

        // log
        uint64_t commitIndex = 0;
        uint64_t writeIndex = 0;
        uint64_t last_log_term = 0;
        // term index command
        std::vector<std::tuple<uint64_t, uint64_t, std::string>> m_log;
        std::string m_logfile;

        // kv work
        std::queue<std::string> command_queue;
        std::unordered_map<std::string, std::string> kv_map;
        muduo::net::EventLoop *m_loop;
        std::mutex m_mtx;
        std::string snapfile;
    };

}

// follower日志比leader日志多的情况
// 先看term，若term比自己高，则看prev_log_index，若prev_log_index比自己小，则删除日志直到和prev_log_index对齐