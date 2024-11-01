#include "rpc/include/rpcprovider.h"
#include <unistd.h>

namespace star
{

    Rpcprovider::Rpcprovider()
    {
        begin = false;

        m_serviceMap = std::make_shared<std::unordered_map<std::string, struct ServiceInfo>>();
        zkCli = std::make_shared<ZkClient>();

        std::thread thread(Rpcprovider::run, this);
        m_thread = std::move(thread);
    }

    Rpcprovider::~Rpcprovider()
    {

        while (1)
        {
            if (m_thread.joinable())
            {
                m_thread.join();
                break;
            }
        }
    }

    void Rpcprovider::AddService(google::protobuf::Service *service)
    {
        while (1)
        {
            if (begin)
            {
                ServiceInfo info;
                const google::protobuf::ServiceDescriptor *servicedesc = service->GetDescriptor();

                std::string service_name = servicedesc->name();

                int methodCnt = servicedesc->method_count();

                for (int i = 0; i < methodCnt; ++i)
                {
                    const google::protobuf::MethodDescriptor *Methodesc = servicedesc->method(i);
                    std::string method_name = Methodesc->name();
                    info.m_methodMap[method_name] = Methodesc;
                }

                info.m_service = service;
                m_serviceMap->insert({service_name, info});

                std::string service_path = '/' + service_name;
                zkCli->Create(service_path.c_str(), nullptr, 0);

                for (auto &mp : info.m_methodMap)
                {
                    // �ҵ�method��·��: /service_name/method_name, �洢��ǰ���rpc��������ip��port
                    std::string method_path = service_path + '/' + mp.first;
                    char method_path_data[128] = {0};
                    sprintf(method_path_data, "%s:%d", Rpcinit::GetInstance()->get_config("ip").c_str(), atoi(Rpcinit::GetInstance()->get_config("port").c_str()));

                    // ZOO_EPHEERAL��ʾ��ʱ�Խ��
                    zkCli->Create(method_path.c_str(), method_path_data, strlen(method_path_data), ZOO_EPHEMERAL);
                }
                break;
            }
            else
            {
                sleep(1);
            }
        }
    }

    void Rpcprovider::run(Rpcprovider *pro)
    {
        pro->m_eventLoop = std::make_shared<muduo::net::EventLoop>();
        std::string ip = Rpcinit::GetInstance()->get_config("ip");
        uint16_t port = atoi(Rpcinit::GetInstance()->get_config("port").c_str());

        muduo::net::InetAddress address(ip, port);
        muduo::net::TcpServer server(pro->m_eventLoop.get(), address, "RpcProvider");

        server.setConnectionCallback([pro](const muduo::net::TcpConnectionPtr &conn)
                                     { pro->OnConnection(conn); });
        server.setMessageCallback([pro](const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp t)
                                  { pro->OnMessage(conn, buffer, t); });

        server.setThreadNum(4);
        pro->zkCli->Start();
        pro->begin = true;
        server.start();
        pro->m_eventLoop->loop();
    }

    void Rpcprovider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
    {
        if (!conn->connected())
        {
            // �����Ѿ��Ͽ�
            conn->shutdown();
        }
    }

    // ��ȡ���ĸ��ֽڣ����ĸ��ֽ�ת����int�����int�洢����rpc���ĵ��ֽڴ�С�������Ƿ������ƣ����������Լ�������С��������Ϣ
    // �ٸ��ݲ�����С���Ż�ȡԶ�˴�����������еĲ���

    void Rpcprovider::OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp t)
    {
        std::string recv_buf = buffer->retrieveAllAsString();
        uint32_t header_size = 0;
        recv_buf.copy((char *)&header_size, 4, 0);

        // ��ȡ����ͷ��ԭʼ�ַ���
        std::string rpc_header_str = recv_buf.substr(4, header_size);

        // �����л�����
        star::RpcHeader rpcHeader;
        std::string service_name; // ����ķ��������
        std::string method_name;  // ����ķ�����
        std::string args_str;     // ��������
        uint32_t args_size;

        if (rpcHeader.ParseFromString(rpc_header_str))
        {
            service_name = rpcHeader.service_name();
            method_name = rpcHeader.method_name();
            args_size = rpcHeader.args_size();
            args_str = recv_buf.substr(4 + header_size, args_size);
        }
        else
        {
            LOG_MAIN_ERROR << "rpcHeader parse failed!";
            return;
        }

        auto it = m_serviceMap->find(service_name);
        if (it == m_serviceMap->end())
        {
            LOG_MAIN_DEBUG << "service: %s Not Found!", service_name.c_str();
        }

        auto mit = it->second.m_methodMap.find(method_name);
        if (mit == it->second.m_methodMap.end())
        {
            LOG_MAIN_DEBUG << "service: %s's method: %s Not Found!", service_name.c_str(), method_name.c_str();
        }

        google::protobuf::Service *service = it->second.m_service;      // service����
        const google::protobuf::MethodDescriptor *method = mit->second; // method����

        // ����rpc�������õ�����request����Ӧresponse����
        google::protobuf::Message *request = service->GetRequestPrototype(method).New();
        if (!request->ParseFromString(args_str))
        {
            LOG_MAIN_ERROR << "request parse failed!";
            return;
        }
        google::protobuf::Message *response = service->GetResponsePrototype(method).New();

        //  ��һ��Closure�Ļص�����, �������CallMethod����
        google::protobuf::Closure *done = google::protobuf::NewCallback<Rpcprovider, const muduo::net::TcpConnectionPtr &,
                                                                        google::protobuf::Message *>(this, &Rpcprovider::SendRpcResponse,
                                                                                                     conn, response);

        // �ڿ���ϸ���Զ��rpc����, ���õ�ǰrpc����Ϸ����ķ���
        service->CallMethod(method, nullptr, request, response, done); // �൱��UserService.Login(...)
    }

    void Rpcprovider::SendRpcResponse(const muduo::net::TcpConnectionPtr &conn, google::protobuf::Message *response)
    {
        std::string response_str;
        if (response->SerializeToString(&response_str))
        { // �������л�
            // ������ͨ�����緢�ͳ�ȥ
            conn->send(response_str);
            conn->shutdown(); // ������, RpcProvide�����Ͽ�����
        }
        else
        {
            LOG_MAIN_ERROR << "RpcProvider response serialized failed!";
        }
    }

}