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
            // 保存服务对象
            google::protobuf::Service *m_service;

            // 记录服务方法名和服务方法descriptor的映射
            std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> m_methodMap;
        };

        // 存储注册为rpc方法成功的服务对象和其服务方法的所有信息
        std::shared_ptr<std::unordered_map<std::string, struct ServiceInfo>> m_serviceMap;

        // 组合EventLoop
        std::shared_ptr<muduo::net::EventLoop> m_eventLoop;

        std::shared_ptr<ZkClient> zkCli;

        std::thread m_thread;

        // 新的socket连接回调
        void OnConnection(const muduo::net::TcpConnectionPtr &conn);

        // 已建立连接用户的读写事件回调
        void OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp t);

        // Closure的回调, 用于序列化rpc响应和网络传送
        void SendRpcResponse(const muduo::net::TcpConnectionPtr &conn, google::protobuf::Message *response);
    };

}
