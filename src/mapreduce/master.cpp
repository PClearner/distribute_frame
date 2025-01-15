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

    static std::string header_make(const std::string &input)
    {
        int32_t size = static_cast<int32_t>(input.size());
        std::string packet;

        // 将长度信息添加到字符串 (网络字节序)
        size = htonl(size); // 转换为网络字节序（大端序）
        packet.append(reinterpret_cast<const char *>(&size), sizeof(size));

        // 添加实际数据
        packet.append(input);

        return packet;
    }

    // static std::string header_make(std::string input)
    // {
    //     int32_t size = static_cast<int32_t>(input.size());
    //     std::string packet(sizeof(size) + input.size(), '\0');
    //     return packet;
    // }

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

        char ch = buf[3];
        uint32_t id = ch - '0';

        // if reduce error;`

        for (auto iter = error_reduces.begin(); iter != error_reduces.end();)
        {
            if (!checkBit(iter->second, id))
            {

                conn->send(header_make(std::string(iter->first.substr(1) + "error")));
                iter->second = setBit(iter->second, id);
                return;
            }
        }

        // deal with map
        if (m_buffer == std::string(buf + "yes"))
        {
            for (auto iter = cut_data.begin(); iter != cut_data.end();)
            {
                if (iter->first == buf[3] - '0')
                {
                    iter = cut_data.erase(iter);
                    send_index--;
                }
                else
                {
                    iter++;
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
            if (!over)
            {
                send_index++;
            }
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
            conn->send(header_make("master_ok"));
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

        reduceport = buffer->retrieveAsString(size);

        std::string buf = reduceport.substr(0, 7);

        if (reduceport == std::string(buf + "over"))
        {
            conn->disconnected();
            reduceresult[buf] = true;
            bool ifover = true;
            for (auto it = reduceresult.begin(); it != reduceresult.end(); it++)
            {
                if (!it->second)
                {
                    ifover = false;
                }
            }

            if (ifover)
            {
                LOG_MAIN_INFO << "all reduce data receive successfully";
                receive_over = true;
                std::ofstream file(m_outfile);
                file << reducebuffer;
                reducebuffer.clear();
                file.close();
            }

            LOG_MAIN_DEBUG << "master disconnect" << buf;
        }
        else
        {
            reducebuffer.append(reduceport);
            conn->send(header_make("master_ok"));
        }
    }

    void master::connect()
    {
        uint32_t index = 1;

        while (1)
        {
            std::string map = "/map" + std::to_string(index);
            std::string host_data = m_zkclient.GetData(map.c_str());

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

            std::string mapname = "map" + std::to_string(index);
            connection[mapname] = mapconnect;
            mapconnect->connect();
            index++;
        }

        index = 1;
        while (1)
        {
            std::string reduce = "/reduce" + std::to_string(index);
            std::string host_data = m_zkclient.GetData(reduce.c_str());

            if (host_data == "")
            {
                LOG_MAIN_DEBUG << reduce << " is not exist";
                break;
            }
            else
            {
                std::string reducename = "reduce" + std::to_string(index);
                reduces.push_back(reducename);
                reduceresult[reducename] = false;
                LOG_MAIN_DEBUG << "add " << reduce;
            }
            index++;
        }
    }

    master::master(std::vector<std::string> inputfiles, default_cut *cut_method)
    {

        m_zkclient.Start();
        files = inputfiles;
        if (cut_method == nullptr)
        {
            m_cut_method = new default_cut;
        }
        else
        {
            m_cut_method = cut_method;
        }

        m_outfile = Rpcinit::GetInstance()->get_config("outfile");
        m_loop = new muduo::net::EventLoop();

        // get map and reduce zookeeper information
        connect();

        // create zookeeper
        std::string m_ip = Rpcinit::GetInstance()->get_config(std::string("master_ip"));
        std::string m_port = Rpcinit::GetInstance()->get_config(std::string("master_port"));

        std::string serverpath = "/master";
        std::string path_data = m_ip + ":" + m_port;

        m_zkclient.Create(serverpath.c_str(), path_data.c_str(), strlen(path_data.c_str()), ZOO_EPHEMERAL);

        // reduce checks
        reduce_check = new std::thread(std::bind(&star::master::reduce_zk, this));

        muduo::net::InetAddress address(m_ip, atoi(m_port.c_str()));
        m_server = new muduo::net::TcpServer(m_loop, address, "master");
        m_server->setConnectionCallback(std::bind(&master::serverConnection, this, std::placeholders::_1));
        m_server->setMessageCallback(std::bind(&master::serverMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_server->setThreadNum(4);

        data_work = new std::thread(std::bind(&star::master::work, this));

        m_server->start();
        m_loop->loop();
    }

    void master::work()
    {

        for (size_t i = 0; i < files.size(); i++)
        {
            while (1)
            {
                if (cut_data.size() <= 6)
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
                std::string map = "/" + iter->first;
                std::string host_data = m_zkclient.GetData(map.c_str());
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
                std::string reduce = "/" + *iter;
                std::string host_data = m_zkclient.GetData(reduce.c_str());

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

    master::~master()
    {
        while (!receive_over)
        {
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        data_work->join();
        reduce_check->join();
    }

}
