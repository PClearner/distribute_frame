#include "mapreduce/include/worker.h"

namespace star
{
    // static int64_t mpsize = 0;

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

    void map::onConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:online";
        }
        else
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:offline";
            conn->shutdown(); // 连接断开将socket资源释放
            // 或者调用_loop->quit()退出epoll;
        }
    }

    void reduce::onConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:online";
            conn->send(header_make(m_id + "ready"));
        }
        else
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:offline";
            conn->shutdown(); // 连接断开将socket资源释放
            // 或者调用_loop->quit()退出epoll;
        }
    }

    void work::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time)
    {
        int64_t size = buffer->readInt32();
        LOG_MAIN_DEBUG << "message size:" << size;

        std::string m_buffer = buffer->retrieveAsString(size);

        deal(conn, m_buffer);
    }

    map::map(std::string id, std::function<std::vector<std::string>(std::string)> func)
    {
        set_map(func);

        m_id = id;
        m_ip = Rpcinit::GetInstance()->get_config(std::string(m_id + "_ip"));
        m_port = Rpcinit::GetInstance()->get_config(std::string(m_id + "_port"));

        if (m_ip == error_code() || m_port == error_code())
        {
            LOG_MAIN_DEBUG << "can't find reduce ip or port";
            assert(false && "reduce ip or port error");
        }

        m_loop = new muduo::net::EventLoop();

        muduo::net::InetAddress address(m_ip, atoi(m_port.c_str()));
        m_server = new muduo::net::TcpServer(m_loop, address, std::string(m_id));

        m_server->setConnectionCallback(std::bind(&map::onConnection, this, std::placeholders::_1));
        m_server->setMessageCallback(std::bind(&map::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        m_server->setThreadNum(4);
        m_zkclient.Start();
        std::string path_data = m_ip + ":" + m_port;
        std::string service_path = '/' + m_id;
        m_zkclient.Create(service_path.c_str(), path_data.c_str(), strlen(path_data.c_str()), ZOO_EPHEMERAL);

        u_int32_t reduce_size = static_cast<u_int32_t>(std::atoi(Rpcinit::GetInstance()->get_config("reduce_size").c_str()));
        for (u_int32_t i = 1; i <= reduce_size; i++)
        {
            std::string id = "reduce" + std::to_string(i);
            std::string ip = Rpcinit::GetInstance()->get_config(std::string(id + "_ip"));
            std::string port = Rpcinit::GetInstance()->get_config(std::string(id + "_port"));
            if (ip == error_code() || port == error_code())
            {
                LOG_MAIN_DEBUG << "can't find reduce ip or port";
                assert(false && "reduce ip or port error");
            }

            rc_record[id] = std::make_tuple(ip, port);
            rc_data[id] = std::vector<std::string>();
            rc_log[id] = Rpcinit::GetInstance()->get_config(std::string(id + "log"));
        }

        m_server->start();
        m_loop->loop();
    }

    void map::deal(const muduo::net::TcpConnectionPtr &conn, std::string &m_buffer)
    {
        std::string buf = m_buffer.substr(0, 6);
        if (m_buffer == "master_ack")
        {
            // receive master request
            if (complete)
            {
                conn->send(header_make(std::string(m_id + "yes")));
            }
            else
            {
                conn->send(header_make(std::string(m_id + "no")));
            }
        }
        else if (m_buffer == "master_over")
        {
            over = true;
        }
        else if (m_buffer == "master_start")
        {
            over = false;
            if (complete)
            {
                conn->send(header_make(std::string(m_id + "yes")));
            }
            else
            {
                conn->send(header_make(std::string(m_id + "no")));
            }
        }
        else if (buf == "reduce")
        {
            // "reduce1error"  from master
            // "reduce1ready"  from reduce1
            std::string message = m_buffer.substr(7);
            buf = m_buffer.substr(0, 7);
            LOG_MAIN_DEBUG << m_buffer;
            if (message == "error")
            {
                // need to resend
                LOG_MAIN_DEBUG << m_buffer;
                Locker::ptr mtx = std::make_shared<Locker>(&m_mtx);
                std::vector<std::string> data = rc_data[buf];
                rc_data.erase(buf);
                LOG_MAIN_DEBUG << "delete " << buf << " over";
                std::vector<std::vector<std::string>> cap(rc_data.size());
                for (int i = 0; i < data.size(); i++)
                {
                    size_t hashValue = std::hash<std::string>{}(data[i]);
                    int place = hashValue % rc_data.size();
                    cap[place].push_back(data[i]);
                }

                std::ifstream file(rc_log[buf]);

                if (!file)
                {
                    LOG_MAIN_DEBUG << "rc_log file open error";
                }

                std::string line;
                while (std::getline(file, line))
                {
                    size_t hashValue = std::hash<std::string>{}(line);
                    int place = hashValue % rc_data.size();
                    cap[place].push_back(line);
                }
                file.close();

                int index = 0;
                for (auto it = rc_data.begin(); it != rc_data.end(); it++)
                {
                    it->second.insert(it->second.end(), cap[index].begin(), cap[index].end());
                    index++;
                }

                LOG_MAIN_DEBUG << buf << " error deal completely";
                conn->send(header_make(std::string(m_id + "yes")));
            }
            else if (message == "ready")
            {

                if (over)
                {
                    // master has no data to send,map need to send remaining data
                    Locker::ptr mtx = std::make_shared<Locker>(&m_mtx);

                    std::ofstream file(rc_log[buf]);
                    std::string sstream;
                    for (int i = 0; i < rc_data[buf].size(); i++)
                    {
                        sstream = sstream + rc_data[buf][i];
                        file << rc_data[buf][i] << std::endl;
                    }
                    file.close();

                    if (sstream.size() == 0)
                    {
                        LOG_MAIN_DEBUG << "rc_data is empty,and it is over";

                        conn->send(header_make(std::string(m_id + "_over")));
                    }
                    else
                    {
                        conn->send(header_make(sstream));
                        rc_data[buf].clear();
                    }
                }
                else
                {
                    Locker::ptr mtx = std::make_shared<Locker>(&m_mtx);

                    std::ofstream file(rc_log[buf], std::ios::app);

                    if (!file.is_open())
                    {
                        std::cerr << "Failed to open file: " << rc_log[buf] << std::endl;
                        return;
                    }

                    std::string sstream;
                    for (int i = 0; i < rc_data[buf].size(); i++)
                    {
                        sstream = sstream + rc_data[buf][i];

                        file << rc_data[buf][i] << std::endl;
                    }
                    file.close();
                    if (sstream.size() == 0)
                    {
                        LOG_MAIN_DEBUG << "rc_data is empty,but it is not over";

                        conn->send(header_make(std::string(m_id + "_empty")));
                    }
                    else
                    {
                        conn->send(header_make(sstream));
                        rc_data[buf].clear();
                    }
                    complete = true;
                }
                LOG_MAIN_DEBUG << "send " << buf << " over";
            }
            else
            {
                LOG_MAIN_DEBUG << "deal function unknown accident,message:" << message << ",buf:" << buf;
            }
        }
        else
        {
            complete = false;
            Locker::ptr mtx = std::make_shared<Locker>(&m_mtx);
            std::vector<std::string> res = m_func(m_buffer);

            for (int index = 0; index < res.size(); index++)
            {
                size_t hashValue = std::hash<std::string>{}(res[index]);
                int place = hashValue % rc_record.size();

                auto it = rc_data.begin();
                for (int i = 0; i < place; i++)
                {
                    it++;
                }
                it->second.push_back(res[index]);
            }
            conn->send(header_make(std::string(m_id + "no")));
        }
        m_buffer.clear();
    }

    reduce::reduce(std::string id, std::function<std::unordered_map<std::string, uint64_t>(std::string)> m_func)
    {
        m_zkclient.Start();
        set_reduce(m_func);

        m_loop = new muduo::net::EventLoop();

        int index = 1;
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
            mapconnect->setConnectionCallback(std::bind(&reduce::onConnection, this, std::placeholders::_1));
            mapconnect->setMessageCallback(std::bind(&reduce::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

            std::string mapname = "map" + std::to_string(index);
            connection[mapname] = mapconnect;

            index++;
        }

        m_id = id;
        m_ip = Rpcinit::GetInstance()->get_config(std::string(m_id + "_ip"));
        m_port = Rpcinit::GetInstance()->get_config(std::string(m_id + "_port"));

        std::string path_data = m_ip + ":" + m_port;
        std::string service_path = '/' + m_id;
        m_zkclient.Create(service_path.c_str(), path_data.c_str(), strlen(path_data.c_str()), ZOO_EPHEMERAL);

        for (auto it = connection.begin(); it != connection.end(); it++)
        {
            it->second->connect();
        }
        m_loop->loop();
    }

    void reduce::masteronConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:online";
        }
        else
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:offline";
            conn->shutdown(); // 连接断开将socket资源释放
            // 或者调用_loop->quit()退出epoll;
        }
    }

    void reduce::masteronMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time)
    {
        int64_t size = buffer->readInt32();
        LOG_MAIN_DEBUG << "message size:" << size;

        std::string m_buffer = buffer->retrieveAsString(size);
        if (m_buffer == "master_ok")
        {
            if (m_data.empty())
            {
                conn->send(header_make(std::string(m_id + "over")));
                conn->shutdown();
                connection.erase("master");
                return;
            }
            std::string stream;
            for (auto it = m_data.begin(); it != m_data.end(); it++)
            {
                stream += it->first + ":" + std::to_string(it->second) + "\n";
            }
            conn->send(header_make(stream));
            m_data.clear();
        }
        else
        {
            LOG_MAIN_ERROR << "master_ok is not exist";
        }
    }

    void reduce::deal(const muduo::net::TcpConnectionPtr &conn, std::string &m_buffer)
    {
        std::string buf = m_buffer.substr(0, 4);
        LOG_MAIN_DEBUG << "buf:" << buf << " m_buffer:" << m_buffer;
        if (connection.find(buf) != connection.end())
        {
            if (m_buffer == std::string(buf + "_over"))
            {
                if (over)
                {
                    LOG_MAIN_DEBUG << "remote is over";
                    std::this_thread::sleep_for(std::chrono::seconds(15));
                    conn->send(header_make(m_id + "ready"));
                }
                else
                {
                    std::string mastername = "/master";
                    std::string host_data = m_zkclient.GetData(mastername.c_str());

                    if (host_data == "")
                    {
                        LOG_MAIN_DEBUG << "master is not exist";
                        assert(false && "zookeeper can't find master");
                    }
                    int idx = host_data.find(":");
                    if (idx == -1)
                    {
                        LOG_MAIN_DEBUG << "master address is not exist";
                        return;
                    }

                    std::string ip = host_data.substr(0, idx);
                    uint16_t port = atoi(host_data.substr(idx + 1, host_data.length() - idx).c_str());

                    muduo::net::InetAddress addr(ip, port);
                    muduo::net::TcpClient *masterconnect = new muduo::net::TcpClient(m_loop, addr, "master");
                    masterconnect->setConnectionCallback(std::bind(&reduce::masteronConnection, this, std::placeholders::_1));
                    masterconnect->setMessageCallback(std::bind(&reduce::masteronMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

                    LOG_MAIN_DEBUG << "add connection: master";
                    masterconnect->connect();
                    connection["master"] = masterconnect;
                    over = true;
                    conn->send(header_make(m_id + "ready"));
                }
            }
            else if (m_buffer == std::string(buf + "_empty"))
            {
                over = false;
                LOG_MAIN_DEBUG << "remote is empty";
                std::this_thread::sleep_for(std::chrono::seconds(4));
                conn->send(header_make(m_id + "ready"));
            }
        }
        else
        {
            LOG_MAIN_DEBUG << "deal map data";
            over = false;
            std::unordered_map<std::string, uint64_t> res = m_func(m_buffer);
            for (auto it = res.begin(); it != res.end(); it++)
            {
                if (m_data.find(it->first) != m_data.end())
                {
                    m_data[it->first] = m_data[it->first] + it->second;
                }
                else
                {
                    m_data[it->first] = it->second;
                }
            }
            conn->send(header_make(m_id + "ready"));
            LOG_MAIN_DEBUG << m_id << " send ready";
        }
        m_buffer.clear();
    }

}
