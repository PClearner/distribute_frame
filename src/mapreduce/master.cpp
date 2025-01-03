#include "mapreduce/include/master.h"

namespace star
{
    bool checkBit(uint32_t num, uint32_t n)
    {
        return (num & (1 << n)) != 0; // 将 num 和掩码做按位与运算，如果该位是 1，则返回 true，否则返回 false
    }

    uint32_t setBit(uint32_t num, uint32_t n)
    {
        return (num | (1 << n)); // 将 num 和掩码做按位与运算，如果该位是 1，则返回 true，否则返回 false
    }

    static std::string header_make(std::string input)
    {
        int32_t size = static_cast<int32_t>(input.size());
        std::string packet(sizeof(size) + input.size(), '\0');
        return packet;
    }

    void master::onConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:online";
            conn->send(header_make("master_ack"));
        }
        else
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:offline";
            conn->shutdown(); // 连接断开将socket资源释放
            // 或者调用_loop->quit()退出epoll;
        }
    }

    void master::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time)
    {
        int64_t size = buffer->readInt32();
        LOG_MAIN_DEBUG << "message size:" << size;

        m_buffer = buffer->retrieveAsString(size);

        std::string buf = m_buffer.substr(0, 4);

        uint32_t id = static_cast<uint32_t>(buf[3]);

        // if reduce error

        for (auto iter = error_reduces.begin(); iter != error_reduces.end();)
        {
            if (!checkBit(iter->second, id))
            {
                conn->send(header_make(std::string(iter->first + "error")));
                iter->second = setBit(iter->second, id);
                return;
            }
        }

        // deal with map
        if (m_buffer == std::string(buf + "yes"))
        {
            for (auto iter = cut_data.begin(); iter != cut_data.end();)
            {
                if (iter->first == buf[3])
                {
                    iter = cut_data.erase(iter);
                    send_index--;
                }
            }

            if (send_index > cut_data.size() - 1)
            {
                if (over)
                {
                    conn->send(header_make("master_over"));
                    conn->disconnected();
                    LOG_MAIN_DEBUG << buf << " disconnect";
                    return;
                }
                else
                {
                    while (send_index > cut_data.size() - 1)
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
                }
            }

            conn->send(header_make(cut_data[send_index].second));
            cut_data[send_index].first = id;
            send_index++;
        }
        else if (m_buffer == std::string(buf + "no"))
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            conn->send(header_make("master_ack"));
        }
    }

    void master::serverConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:online";
            conn->send(header_make("master_ready"));
        }
        else
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:offline";
            conn->shutdown(); // 连接断开将socket资源释放
            // 或者调用_loop->quit()退出epoll;
        }
    }

    void master::serverMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time)
    {
        int64_t size = buffer->readInt32();
        LOG_MAIN_DEBUG << "message size:" << size;

        m_buffer = buffer->retrieveAsString(size);

        std::string buf = m_buffer.substr(0, 7);

        uint32_t id = static_cast<uint32_t>(buf[3]);

        if (m_buffer == std::string(buf + "over"))
        {
            conn->disconnected();
            LOG_MAIN_DEBUG << "master disconnect"<<buf;
        }
        else
        {
            std::ofstream file(m_outfile);
            file<<m_buffer;
            m_buffer.clear();
            file.close();
        }
    }

    void master::connect()
    {
        uint32_t index = 1;

        while (1)
        {
            std::string map = "map" + std::to_string(index);
            std::string host_data = m_zkclient.GetData(map);

            if (host_data == "")
            {
                LOG_MAIN_DEBUG << map << " is not exist";
                break;
            }

            int idx = host_data.find(":");
            if (idx == -1)
            {
                LOG_MAIN_DEBUG << map << " address is not exist";
                return;
            }

            std::string ip = host_data.substr(0, idx);
            uint16_t port = atoi(host_data.substr(idx + 1, host_data.length() - idx).c_str());

            muduo::net::InetAddress addr(ip, port);

            muduo::net::TcpClient *mapconnect = new muduo::net::TcpClient(m_loop, addr, map);
            mapconnect->setConnectionCallback(std::bind(&master::onConnection, this, std::placeholders::_1));
            mapconnect->setMessageCallback(std::bind(&master::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            LOG_MAIN_DEBUG << "add connection:" << map;
            connection[map] = mapconnect;
            index++;
        }
    }

    master::master(std::string outfile, default_cut *cut_method)
    {
        if (cut_method == nullptr)
        {
            m_cut_method = new default_cut;
        }
        else
        {
            m_cut_method = cut_method;
        }

        m_outfile = outfile;
        m_loop = new muduo::net::EventLoop();

        connect();

        // reduce information
        uint32_t index = 1;
        while (1)
        {
            std::string reduce = "reduce" + std::to_string(index);
            std::string host_data = m_zkclient.GetData(reduce);

            if (host_data == "")
            {
                LOG_MAIN_DEBUG << reduce << " is not exist";
                break;
            }
            else
            {
                reduces.push_back(reduce);
            }
            index++;
        }

        // create zookeeper
        std::string m_ip = Rpcinit::GetInstance()->get_config(std::string("master_ip"));
        std::string m_port = Rpcinit::GetInstance()->get_config(std::string("master_port"));
        m_zkclient.Start();
        std::string path_data = m_ip + ":" + m_port;
        m_zkclient.Create("master", path_data.c_str(), strlen(path_data.c_str()), ZOO_EPHEMERAL);

        // reduce check
        reduce_check = new std::thread(&reduce_zk, this);

        muduo::net::InetAddress address(m_ip, atoi(m_port.c_str()));
        m_server = new muduo::net::TcpServer(m_loop, address, "master");
        m_server->setConnectionCallback(std::bind(&master::serverConnection, this, std::placeholders::_1));
        m_server->setMessageCallback(std::bind(&master::serverMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_server->setThreadNum(4);

        data_work = new std::thread(&work, this);

        m_server->start();
    }

    void master::work()
    {

        for (size_t i = 0; i < files.size(); i++)
        {
            while (1)
            {
                if (cut_data.size() < 6)
                {
                    std::string res = m_cut_method->cut(files[i]);
                    cut_data.push_back(std::make_pair(0, res));
                    if (res.size() == 0)
                    {
                        break;
                    }
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                }
            }
        }
        over = true;
    }

    void master::reduce_zk()
    {
        while (!over)
        {
            // check map
            for (auto iter = connection.begin(); iter != connection.end();)
            {
                std::string map = iter->first;
                std::string host_data = m_zkclient.GetData(map);
                uint32_t id = static_cast<uint32_t>(map[3]);

                if (host_data == "")
                {
                    LOG_MAIN_ERROR << map << " is not exist";

                    for (int i = 0; i < cut_data.size(); i++)
                    {
                        if (cut_data[i].first == 0)
                        {
                            break;
                        }
                        else if (cut_data[i].first == id)
                        {
                            auto inp = cut_data[i];
                            cut_data.erase(cut_data.begin() + i);
                            inp.first = 0;
                            cut_data.push_back(inp);
                        }
                    }

                    iter->second->disconnect();
                    iter = connection.erase(iter);
                }
                else
                {
                    iter++;
                }
            }

            // check reduce

            for (auto iter = reduces.begin(); iter != reduces.end();)
            {
                std::string reduce = *iter;
                std::string host_data = m_zkclient.GetData(reduce);
                uint32_t id = static_cast<uint32_t>(reduce[6]);

                if (host_data == "")
                {
                    LOG_MAIN_ERROR << reduce << " is not exist";

                    error_reduces.push_back(std::make_pair(reduce, 0));
                    reduces.erase(iter);
                }
                else
                {
                    iter++;
                }
            }

            // sleep
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }

}
