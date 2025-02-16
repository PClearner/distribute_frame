#include "kv_raft/include/kv_client.h"

namespace star
{
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

    KVclient::KVclient()
    {
        m_loop = new muduo::net::EventLoop();
        uint16_t size = static_cast<uint16_t>(std::stoi(Rpcinit::GetInstance()->get_config("nodesizes")));

        for (uint16_t i = 1; i <= size; i++)
        {
            std::string node = "node" + std::to_string(i);
            std::string nodeip = Rpcinit::GetInstance()->get_config(node + "_ip");
            std::string nodeport = Rpcinit::GetInstance()->get_config(node + "_port");

            std::pair<std::string, std::string> nodeinfo(nodeip, nodeport);
            nodes_record[node] = nodeinfo;
        }

        loopthread = new std::thread(&runEventLoop, this);
    }

    bool KVclient::send_command(std::string commmand)
    {
        m_command = commmand;
        while (!over)
        {
            if (leaderid == "")
            {
                leaderid = nodes_record.begin()->first;
                std::string ip = nodes_record[leaderid].first;
                uint16_t port = atoi(nodes_record[leaderid].second.c_str());

                muduo::net::InetAddress addr(ip, port);

                m_connect = new muduo::net::TcpClient(m_loop, addr, leaderid);
                m_connect->setConnectionCallback(std::bind(&KVclient::clientonConnection, this, std::placeholders::_1));
                m_connect->setMessageCallback(std::bind(&KVclient::clientonMessage, this,
                                                        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            }
            else
            {
                if (!m_conn->connected())
                {
                    auto it = nodes_record.find(leaderid);
                    it++;
                    leaderid = it->first;

                    std::string ip = nodes_record[leaderid].first;
                    uint16_t port = atoi(nodes_record[leaderid].second.c_str());

                    muduo::net::InetAddress addr(ip, port);

                    m_connect = new muduo::net::TcpClient(m_loop, addr, leaderid);
                    m_connect->setConnectionCallback(std::bind(&KVclient::clientonConnection, this, std::placeholders::_1));
                    m_connect->setMessageCallback(std::bind(&KVclient::clientonMessage, this,
                                                            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                }
            }
        }
    }

    void KVclient::clientonConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        m_conn = conn;
        if (conn->connected())
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:online";
            raft::ClientRequest req;
            req.set_command(m_command);

            req.set_start(start);

            std::string output;
            req.SerializeToString(&output);

            conn->send(header_make("5" + output));
        }
        else
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:offline";
            conn->shutdown();
        }
    }
    void KVclient::clientonMessage(const muduo::net::TcpConnectionPtr &conn,
                                   muduo::net::Buffer *buffer, muduo::Timestamp time)
    {

        int64_t size = buffer->readInt32();
        std::string m_buffer = buffer->retrieveAsString(size);
        char type = m_buffer[0];
        m_buffer = m_buffer.substr(1);

        raft::ClientResponse response;
        response.ParseFromString(m_buffer);

        if (response.success())
        {
            over = true;
            success = true;
        }
        else
        {
            if (response.reason() == "notleader")
            {
                conn->shutdown();
            }
            else if (response.reason() == "wait")
            {
                leaderid = response.leaderid();
                start = true;
                index = response.index();
            }
            else if (response.reason() == "indexerror")
            {
                over = true;
                success = false;
            }
            else
            {
                LOG_MAIN_ERROR << "reason wrong";
            }
        }
    }

}