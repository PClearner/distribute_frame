#include "kv_raft/include/kv_server.h"
#include <algorithm>

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

    bool KVserver::log_input(uint64_t term, uint64_t index, std::string command)
    {

        // std::string id = std::to_string(index) + ":"+std::to_string(term);

        // std::string log = id + " " + command;
        std::tuple<uint64_t, uint64_t, std::string> log(term, index, command);

        m_log.push_back(log);

        last_log_term = term;
    }

    bool KVserver::write_commit()
    {

        std::fstream file(m_logfile, std::ios::in | std::ios::out | std::ios::app);

        for (; writeIndex <= commitIndex - 1; writeIndex++)
        {

            uint64_t tmpterm = std::get<0>(m_log[writeIndex]);
            uint64_t tmpindex = std::get<1>(m_log[writeIndex]);
            std::string tmpcommand = std::get<2>(m_log[writeIndex]);

            std::string id = std::to_string(tmpterm) + ":" + std::to_string(tmpindex);

            std::string log = id + " " + tmpcommand + "\n";

            file << log;
            writeIndex++;
        }
        file.close();
    }
    // 222333

    bool KVserver::deletefile()
    {
        std::fstream file(m_logfile, std::ios::in | std::ios::out);
        file.close();
    }

    void KVserver::loadlog()
    {
        std::fstream file(m_logfile, std::ios::in | std::ios::out);
        std::string line;
        while (std::getline(file, line))
        {
            int idx1 = line.find(" ");
            std::string id = line.substr(0, idx1);
            std::string command = line.substr(idx1 + 1);
            int idx2 = id.find(":");
            uint64_t term = std::stoull(id.substr(0, idx2));
            uint64_t index = std::stoull(id.substr(idx2 + 1));
            std::tuple<bool, std::string> res = do_command(command);
            if (!std::get<0>(res))
            {
                assert(false && "loadlog error");
            }
        }
        file.close();
    }

    // PUT:K=10
    std::tuple<bool, std::string> KVserver::do_command(std::string command)
    {
        int idx = command.find(":");
        std::string method = command.substr(0, idx);
        std::string kv = command.substr(idx + 1);

        // 注意，get和delete逻辑上一样，因为get和delete失败都是因为没有这个key

        if (method == "GET")
        {
            if (kv_map.find("kv") != kv_map.end())
            {
                std::tuple<bool, std::string> res(true, kv_map["kv"]);
                return res;
            }
            else
            {
                std::tuple<bool, std::string> res(false, "");
                return res;
            }
        }
        else if (method == "APPEND")
        {
            int idx2 = kv.find(":");
            std::string k = kv.substr(0, idx2);
            std::string v = kv.substr(idx2 + 1);
            kv_map[k] = v;
            std::tuple<bool, std::string> res(true, "");
        }
        else if (method == "DELETE")
        {
            auto i = kv_map.find("kv");
            if (i != kv_map.end())
            {
                kv_map.erase(i);
                std::tuple<bool, std::string> res(true, "");
                return res;
            }
            else
            {
                std::tuple<bool, std::string> res(false, "");
                return res;
            }
        }
    }

    KVserver::KVserver()
    {

        m_logfile = Rpcinit::GetInstance()->get_config("logfile");
        loadlog();
        m_state = state::Follower;
        acceptheartbeat = false;
        timeout = 300;
        m_term = 0;

        m_loop = new muduo::net::EventLoop();
        m_id = Rpcinit::GetInstance()->get_config("m_id");

        uint16_t size = static_cast<uint16_t>(std::stoi(Rpcinit::GetInstance()->get_config("nodesizes")));

        // node1_ip:xxx
        // node1_port

        for (u_int16_t i = 1; i <= size; i++)
        {
            std::string node = "node" + std::to_string(i);
            std::string nodeip = Rpcinit::GetInstance()->get_config(node + "_ip");
            std::string nodeport = Rpcinit::GetInstance()->get_config(node + "_port");
            if (m_id == node)
            {
                m_ip = nodeip;
                m_port = nodeport;
                votesituation[node] = 3;
                continue;
            }
            nodes_information.push_back(std::make_tuple(node, nodeip, nodeport, nullptr, false, nullptr));
            votesituation[node] = 3;
        }

        muduo::net::InetAddress address(m_ip, atoi(m_port.c_str()));
        m_server = new muduo::net::TcpServer(m_loop, address, std::string(m_id));

        m_server->setConnectionCallback(std::bind(&KVserver::serveronConnection, this, std::placeholders::_1));
        m_server->setMessageCallback(std::bind(&KVserver::serveronMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_server->setThreadNum(4);

        m_server->start();
        loopthread = new std::thread(&runEventLoop, this);

        work();
    }

    void KVserver::serveronConnection(const muduo::net::TcpConnectionPtr &conn)
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
    // 若出现最开始阶段，所有node的日志都是空的，node1一开始作为leader，刚有两个日志还没同步就挂了
    // 然后node2当leader，此时node1立即恢复，并准备日志同步。
    // 对齐index，由于leader为空，所以发送的是empty，由于previndex比node1小，于是node1要同步，需要删除log

    void KVserver::serveronMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time)
    {
        int64_t size = buffer->readInt32();
        std::string m_buffer = buffer->retrieveAsString(size);
        char type = m_buffer[0];
        m_buffer = m_buffer.substr(1);
        if (type == '1')
        {
            raft::RequestVote rqvote;
            rqvote.ParseFromString(m_buffer);
            raft::RequestVoteResponse res;
            res.set_term(m_term);
            if (rqvote.term() > m_term)
            {
                if (rqvote.last_log_term() >= m_term)
                {
                    if (rqvote.last_log_index() >= m_log.size())
                    {
                        if (votefor)
                        {
                            res.set_vote_granted(false);
                        }
                        else
                        {
                            res.set_vote_granted(true);
                            m_state = state::Follower;
                        }
                    }
                }
                else
                {
                    res.set_vote_granted(false);
                }
            }
            else
            {
                res.set_vote_granted(false);
            }

            std::string output;
            res.SerializeToString(&output);
            conn->send(header_make("3" + output));
        }
        else if (type == '2')
        {
            acceptheartbeat.store(true, std::memory_order_relaxed);
            raft::AppendEntries appentries;
            raft::AppendEntriesResponse res;
            res.set_term(m_term);
            res.set_conflict_index(m_log.size());

            if (appentries.leader_commit() <= m_log.size())
            {
                commitIndex = appentries.leader_commit();
            }
            else
            {
                commitIndex = m_log.size();
            }

            appentries.ParseFromString(m_buffer);

            m_term = appentries.term();

            leaderid = appentries.leader_id();

            if (appentries.prev_log_term() > last_log_term)
            {
                if (appentries.prev_log_index() <= std::get<1>(m_log[m_log.size() - 1]))
                {
                    m_log.erase(m_log.begin() + appentries.prev_log_index(), m_log.end());
                }
                res.set_success(false);
                res.set_term(m_term);
                res.set_conflict_index(appentries.prev_log_index());
                res.set_conflict_term(appentries.prev_log_term());

                std::string output;
                res.SerializeToString(&output);
                conn->send(header_make("4" + output));
            }
            else
            {
                if (appentries.prev_log_index() >= std::get<1>(m_log[m_log.size() - 1]))
                {
                    res.set_success(false);
                    res.set_term(m_term);
                    res.set_conflict_index(appentries.prev_log_index());
                    res.set_conflict_term(appentries.prev_log_term());

                    std::string output;
                    res.SerializeToString(&output);
                    conn->send(header_make("4" + output));
                }
                else
                {

                    res.set_success(true);
                    res.set_term(m_term);

                    if (appentries.entries_size() == 0)
                    {
                        res.set_conflict_index(appentries.prev_log_index());
                        res.set_conflict_term(appentries.prev_log_term());
                    }
                    else
                    {

                        raft::LogEntry tmp = appentries.entries(0);
                        log_input(tmp.term(), tmp.index(), tmp.command());
                        command_queue.push(tmp.command());
                        res.set_conflict_index(tmp.term());
                        res.set_conflict_term(tmp.index());
                    }

                    std::string output;
                    res.SerializeToString(&output);
                    conn->send(header_make("4" + output));
                }
            }
        }
        else if (type == '5')
        {

            raft::ClientRequest req;
            req.ParseFromString(m_buffer);
            raft::ClientResponse resp;
            if (m_state != state::Leader)
            {
                resp.set_success(false);
                resp.set_reason("notleader");

                resp.set_leaderid(leaderid);
            }
            else
            {
                uint64_t index;
                if (!req.start())
                {

                    {
                        Locker::ptr mtx = std::make_shared<Locker>(&m_mtx);
                        command_queue.push(req.command());
                        index = m_log.size() + 1;
                        log_input(m_term, index, req.command());
                    }

                    resp.set_success(false);
                    resp.set_reason("wait");

                    resp.set_leaderid(leaderid);
                    resp.set_index(index);

                    std::string output;
                    resp.SerializeToString(&output);
                    conn->send(header_make("6" + output));

                    while (1)
                    {
                        if (commitIndex >= index)
                        {
                            resp.set_success(true);

                            std::string output;
                            resp.SerializeToString(&output);
                            conn->send(header_make("6" + output));
                            break;
                        }
                        else
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
                        }
                    }
                }
                else
                {
                    if (m_log.size() >= req.index())
                    {
                        if (std::get<2>(m_log[req.index() - 1]) != req.command())
                        {
                            resp.set_success(false);
                            resp.set_reason("indexerror");

                            resp.set_leaderid(leaderid);
                            resp.set_index(req.index());

                            std::string output;
                            resp.SerializeToString(&output);
                            conn->send(header_make("6" + output));
                        }
                        else
                        {
                            while (1)
                            {
                                if (commitIndex >= index)
                                {
                                    resp.set_success(true);
                                    resp.set_leaderid(leaderid);
                                    resp.set_index(req.index());

                                    std::string output;
                                    resp.SerializeToString(&output);
                                    conn->send(header_make("6" + output));
                                    break;
                                }
                                else
                                {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
                                }
                            }
                        }
                    }
                    else
                    {
                        resp.set_success(false);
                        resp.set_reason("indexerror");

                        resp.set_leaderid(leaderid);
                        resp.set_index(req.index());

                        std::string output;
                        resp.SerializeToString(&output);
                        conn->send(header_make("6" + output));
                    }
                }
            }
        }
    }

    void KVserver::clientonConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:online";
            for (size_t i = 0; i < nodes_information.size(); i++)
            {
                if (conn->name() == std::get<0>(nodes_information[i]))
                {
                    std::get<3>(nodes_information[i]) = conn;
                    std::get<4>(nodes_information[i]) = true;
                }
            }
            if (m_state == state::Candidate)
            {
                raft::RequestVote voterequest;
                voterequest.set_term(m_term);
                voterequest.set_candidate_id(m_id);
                voterequest.set_last_log_index(commitIndex);
                voterequest.set_last_log_term(last_log_term);

                std::string output;
                voterequest.SerializeToString(&output);

                conn->send(header_make("1" + output));
            }
            else if (m_state == state::Leader)
            {

                raft::AppendEntries heartbeat;
                heartbeat.set_term(m_term);
                heartbeat.set_leader_id(m_id);
                heartbeat.set_prev_log_index(nextIndex[conn->name()] - 1);

                if (nextIndex[conn->name()] - 1 == 0)
                {
                    heartbeat.set_prev_log_term(0);
                }
                else
                {
                    heartbeat.set_prev_log_term(std::get<0>(m_log[nextIndex[conn->name()] - 2]));
                }

                if (nextIndex[conn->name()] <= m_log.size())
                {
                    raft::LogEntry logsend;
                    logsend.set_command(std::get<2>(m_log[nextIndex[conn->name()] - 1]));
                    logsend.set_index(nextIndex[conn->name()]);
                    logsend.set_term(std::get<0>(m_log[nextIndex[conn->name()] - 1]));
                    *heartbeat.add_entries() = logsend;
                }

                std::string output;
                heartbeat.SerializeToString(&output);

                conn->send(header_make("2" + output));
            }
            else
            {
                LOG_MAIN_ERROR << "clientonConnection error";
            }
        }
        else
        {
            LOG_MAIN_DEBUG << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << "state:offline";
            muduo::net::TcpClient *tmp;
            for (size_t i = 0; i < nodes_information.size(); i++)
            {
                if (conn->name() == std::get<0>(nodes_information[i]))
                {
                    std::get<4>(nodes_information[i]) = false;
                    tmp = std::get<5>(nodes_information[i]);
                }
            }
            conn->shutdown(); // 连接断开将socket资源释放
            delete tmp;
            if (m_state == state::Candidate)
            {
                votesituation[conn->name()] = 2;
            }
            // 或者调用_loop->quit()退出epoll;
        }
    }

    // leader有两个日志，follower为0个，leader发送append请求，previndex为2，follower接收到previndex为2，自己的logsize为0
    // 于是发送false，leader接收到后修改nextIndex后接着发送，follower接收到的previndex为1时执行log，返回true。
    // leader接收到true后发送previndex为2的日志，follower接收后发送true，leader接收到true后由于自己就两个日志
    // 于是重新发送previndex为2，但是不发送log了，follower确认previndex为2，但是自己这边已经index为2了，返回success。

    void KVserver::clientonMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time)
    {
        int64_t size = buffer->readInt32();
        std::string m_buffer = buffer->retrieveAsString(size);
        char type = m_buffer[0];
        m_buffer = m_buffer.substr(1);
        if (type == '3')
        {
            raft::RequestVoteResponse voteresponse;
            if (voteresponse.ParseFromString(m_buffer))
            {
                if (voteresponse.term() >= m_term)
                {
                    m_state = state::Follower;
                }
                else
                {
                    if (voteresponse.vote_granted())
                    {
                        votesituation[conn->name()] = 1;
                    }
                    else
                    {
                        votesituation[conn->name()] = 2;
                    }

                    uint32_t successnumber = 0;
                    bool ifunknown = false;
                    for (auto i = votesituation.begin(); i != votesituation.end(); i++)
                    {
                        if (i->second == 1)
                        {
                            successnumber++;
                        }
                        else if (i->second == 3)
                        {
                            ifunknown = true;
                            break;
                        }
                    }
                    if (!ifunknown)
                    {
                        if (successnumber >= (nodes_information.size() + 1) / 2)
                        {
                            m_state = state::Leader;
                        }
                    }
                }
            }
            else
            {
                LOG_MAIN_ERROR << "voteresponse can't analyse ";
            }
        }
        else if (type == '4')
        {
            raft::AppendEntriesResponse entriesresponse;
            if (entriesresponse.ParseFromString(m_buffer))
            {
                if (entriesresponse.success())
                {
                    // conflict_index 是follower手里的日志数量
                    // 若比leader大，则删除直到与leader对齐
                    // 会不会存在log的index不一致情况？
                    // 默认不会先，没想到反例，测试阶段再细看。
                    nextIndex[conn->name()] = entriesresponse.conflict_index() + 1;

                    matchIndex[conn->name()] = entriesresponse.conflict_index();

                    // follower last_log_term = 0;leader prev_log_term = 1;
                    // follower return 0
                    // leader

                    //
                    if (commitIndex < entriesresponse.conflict_index())
                    {
                        std::vector<uint64_t> tmp;
                        for (auto i = matchIndex.begin(); i != matchIndex.end(); i++)
                        {
                            tmp.push_back(i->second);
                        }
                        std::sort(tmp.begin(), tmp.end());

                        size_t index = tmp.size() / 2;

                        if (tmp[index] > commitIndex)
                        {
                            commitIndex = tmp[index];
                        }
                    }

                    raft::AppendEntries heartbeat;
                    heartbeat.set_term(m_term);
                    heartbeat.set_leader_id(m_id);

                    heartbeat.set_prev_log_index(nextIndex[conn->name()] - 1);
                    heartbeat.set_prev_log_term(std::get<0>(m_log[nextIndex[conn->name()] - 1]));

                    if (nextIndex[conn->name()] <= m_log.size())
                    {
                        raft::LogEntry logsend;
                        logsend.set_command(std::get<2>(m_log[nextIndex[conn->name()] - 1]));
                        logsend.set_index(std::get<1>(m_log[nextIndex[conn->name()] - 1]));
                        logsend.set_term(std::get<0>(m_log[nextIndex[conn->name()] - 1]));
                        *heartbeat.add_entries() = logsend;
                    }

                    std::string output;
                    heartbeat.SerializeToString(&output);

                    conn->send(header_make("2" + output));
                }
                else
                {

                    nextIndex[conn->name()] = entriesresponse.conflict_index();

                    raft::AppendEntries heartbeat;
                    heartbeat.set_term(m_term);
                    heartbeat.set_leader_id(m_id);

                    if (nextIndex[conn->name()] == 0)
                    {
                        heartbeat.set_prev_log_term(0);
                    }
                    else
                    {
                        heartbeat.set_prev_log_term(std::get<0>(m_log[nextIndex[conn->name()] - 2]));
                    }

                    heartbeat.set_prev_log_index(nextIndex[conn->name()] - 1);

                    if (nextIndex[conn->name()] <= m_log.size())
                    {
                        raft::LogEntry logsend;
                        logsend.set_command(std::get<2>(m_log[nextIndex[conn->name()] - 1]));
                        logsend.set_index(std::get<1>(m_log[nextIndex[conn->name()] - 1]));
                        logsend.set_term(std::get<0>(m_log[nextIndex[conn->name()] - 1]));
                        *heartbeat.add_entries() = logsend;
                    }

                    std::string output;
                    heartbeat.SerializeToString(&output);

                    conn->send(header_make("2" + output));
                }
            }
        }
    }

    void KVserver::work()
    {

        while (1)
        {
            write_commit();
            if (m_state == state::Follower)
            {
                Leaderstart = false;
                std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
                if (acceptheartbeat.load(std::memory_order_relaxed))
                {
                    Locker::ptr mtx = std::make_shared<Locker>(&m_mtx);
                    acceptheartbeat.store(false, std::memory_order_relaxed);
                }
                else
                {
                    m_term++;
                    m_state = state::Candidate;
                }
            }
            else if (m_state == state::Leader)
            {
                while (1)
                {
                    if (!command_queue.empty())
                    {
                        std::string command = command_queue.front();
                        command_queue.pop();
                        do_command(command);
                    }
                }

                if (!Leaderstart)
                {
                    for (size_t i = 0; i < nodes_information.size(); i++)
                    {

                        nextIndex[std::get<0>(nodes_information[i])] = m_log.size() + 1;
                    }
                    Leaderstart = true;
                }

                for (size_t i = 0; i < nodes_information.size(); i++)
                {

                    if (!std::get<4>(nodes_information[i]))
                    {
                        std::string ip = std::get<1>(nodes_information[i]);
                        uint16_t port = atoi(std::get<2>(nodes_information[i]).c_str());
                        muduo::net::InetAddress addr(ip, port);
                        muduo::net::TcpClient *connect = new muduo::net::TcpClient(m_loop, addr, std::get<0>(nodes_information[i]));
                        connect->setConnectionCallback(std::bind(&KVserver::clientonConnection, this, std::placeholders::_1));
                        connect->setMessageCallback(std::bind(&KVserver::clientonMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
                        std::get<5>(nodes_information[i]) = connect;
                    }
                }
            }
            else if (m_state == state::Candidate)
            {
                Leaderstart = false;
                for (auto i = votesituation.begin(); i != votesituation.end(); i++)
                {
                    i->second = 3;
                }

                votesituation[m_id] = 1;
                for (size_t i = 0; i < nodes_information.size(); i++)
                {
                    if (!std::get<4>(nodes_information[i]))
                    {
                        std::string ip = std::get<1>(nodes_information[i]);
                        uint16_t port = atoi(std::get<2>(nodes_information[i]).c_str());
                        muduo::net::InetAddress addr(ip, port);
                        muduo::net::TcpClient *connect = new muduo::net::TcpClient(m_loop, addr, std::get<0>(nodes_information[i]));
                        connect->setConnectionCallback(std::bind(&KVserver::clientonConnection, this, std::placeholders::_1));
                        connect->setMessageCallback(std::bind(&KVserver::clientonMessage, this,
                                                              std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
                        std::get<5>(nodes_information[i]) = connect;
                    }
                }
            }
            else
            {
                LOG_MAIN_ERROR << "m_state is error";
                break;
            }
        }
    }

}
