#pragma once

#include <functional>
#include <unordered_map>
#include "tools/include/logger.h"
#include "rpc/include/init.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpConnection.h"
#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include "rpc/include/zookeeperutil.h"
#include "rpc/include/rpcheader.pb.h"

namespace star
{
    class Rpcprovider
    {
    public:
        Rpcprovider();
        ~Rpcprovider();

        void AddService(google::protobuf::Service *service);

        static void run(Rpcprovider *pro);

    private:
        struct ServiceInfo
        {
            // ����������
            google::protobuf::Service *m_service;

            // ��¼���񷽷����ͷ��񷽷�descriptor��ӳ��
            std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> m_methodMap;
        };

        // �洢ע��Ϊrpc�����ɹ��ķ�����������񷽷���������Ϣ
        std::shared_ptr<std::unordered_map<std::string, struct ServiceInfo>> m_serviceMap;

        // ���EventLoop
        std::shared_ptr<muduo::net::EventLoop> m_eventLoop;

        std::shared_ptr<ZkClient> zkCli;

        std::thread m_thread;

        // �µ�socket���ӻص�
        void OnConnection(const muduo::net::TcpConnectionPtr &conn);

        // �ѽ��������û��Ķ�д�¼��ص�
        void OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp t);

        // Closure�Ļص�, �������л�rpc��Ӧ�����紫��
        void SendRpcResponse(const muduo::net::TcpConnectionPtr &conn, google::protobuf::Message *response);
    };

}
