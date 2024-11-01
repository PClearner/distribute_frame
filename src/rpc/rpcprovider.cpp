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
                    // 找到method的路径: /service_name/method_name, 存储当前这个rpc结点的主机ip和port
                    std::string method_path = service_path + '/' + mp.first;
                    char method_path_data[128] = {0};
                    sprintf(method_path_data, "%s:%d", Rpcinit::GetInstance()->get_config("ip").c_str(), atoi(Rpcinit::GetInstance()->get_config("port").c_str()));

                    // ZOO_EPHEERAL表示临时性结点
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
            // 连接已经断开
            conn->shutdown();
        }
    }

    // 先取出四个字节，这四个字节转换成int，这个int存储的是rpc报文的字节大小，里面是服务名称，方法名称以及参数大小这三个信息
    // 再根据参数大小接着获取远端传输过来的所有的参数

    void Rpcprovider::OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp t)
    {
        std::string recv_buf = buffer->retrieveAllAsString();
        uint32_t header_size = 0;
        recv_buf.copy((char *)&header_size, 4, 0);

        // 读取数据头的原始字符流
        std::string rpc_header_str = recv_buf.substr(4, header_size);

        // 反序列化数据
        star::RpcHeader rpcHeader;
        std::string service_name; // 请求的服务对象名
        std::string method_name;  // 请求的方法名
        std::string args_str;     // 参数内容
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

        google::protobuf::Service *service = it->second.m_service;      // service对象
        const google::protobuf::MethodDescriptor *method = mit->second; // method方法

        // 生成rpc方法调用的请求request和响应response参数
        google::protobuf::Message *request = service->GetRequestPrototype(method).New();
        if (!request->ParseFromString(args_str))
        {
            LOG_MAIN_ERROR << "request parse failed!";
            return;
        }
        google::protobuf::Message *response = service->GetResponsePrototype(method).New();

        //  绑定一个Closure的回调函数, 供下面的CallMethod调用
        google::protobuf::Closure *done = google::protobuf::NewCallback<Rpcprovider, const muduo::net::TcpConnectionPtr &,
                                                                        google::protobuf::Message *>(this, &Rpcprovider::SendRpcResponse,
                                                                                                     conn, response);

        // 在框架上根据远端rpc请求, 调用当前rpc结点上发布的方法
        service->CallMethod(method, nullptr, request, response, done); // 相当于UserService.Login(...)
    }

    void Rpcprovider::SendRpcResponse(const muduo::net::TcpConnectionPtr &conn, google::protobuf::Message *response)
    {
        std::string response_str;
        if (response->SerializeToString(&response_str))
        { // 进行序列化
            // 接下来通过网络发送出去
            conn->send(response_str);
            conn->shutdown(); // 短连接, RpcProvide主动断开连接
        }
        else
        {
            LOG_MAIN_ERROR << "RpcProvider response serialized failed!";
        }
    }

}