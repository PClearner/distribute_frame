#include <string>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include "rpc/include/rpcheader.pb.h"
#include "rpc/include/rpcchannel.h"
#include "rpc/include/zookeeperutil.h"
#include "rpc/include/zookeeperutil.h"

namespace star
{
    void rpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                                google::protobuf::RpcController *controller,
                                const google::protobuf::Message *request,
                                google::protobuf::Message *response,
                                google::protobuf::Closure *done)
    {
        const google::protobuf::ServiceDescriptor *sd = method->service();
        std::string service_name = sd->name();
        std::string method_name = method->name();

        // 获取参数的序列化字符串长度args_size
        uint32_t args_size = 0;
        std::string args_str;

        if (request->SerializeToString(&args_str))
        {
            args_size = args_str.size();
        }
        else
        {
            LOG_MAIN_ERROR << "args_str SerializeToString error!";
            controller->SetFailed("serialize request error!");
            return;
        }

        // 定义rpc的请求header
        star::RpcHeader rpcHeader;
        rpcHeader.set_service_name(service_name);
        rpcHeader.set_method_name(method_name);
        rpcHeader.set_args_size(args_size);

        uint32_t header_size = 0;
        std::string rpc_header_str;
        if (rpcHeader.SerializeToString(&rpc_header_str))
        {
            header_size = rpc_header_str.size();
        }
        else
        {
            LOG_MAIN_ERROR << "rpc_header_str SerializeToString error!";
            controller->SetFailed("serialize rpc_header_str error!");
            return;
        }

        // 组织待发送的rpc请求的字符串
        std::string send_rpc_str;
        send_rpc_str.insert(0, std::string((char *)&header_size, 4)); // header_size
        send_rpc_str += rpc_header_str;                               // rpcheader
        send_rpc_str += args_str;                                     // args

        // 打印调试信息
        LOG_MAIN_INFO << "======================";
        LOG_MAIN_INFO << "header_size: " << header_size;
        LOG_MAIN_INFO << "rpc_header_str: " << rpc_header_str;
        LOG_MAIN_INFO << "service_name: " << service_name;
        LOG_MAIN_INFO << "method_name: " << method_name;
        LOG_MAIN_INFO << "args_str: " << args_str;
        LOG_MAIN_INFO << "======================";

        /* 读取配置文件rpcserver的信息
        */

        // rpc调用服务, 需要查询zookeeper上该服务的ip:port信息
        ZkClient zkCli;
        zkCli.Start();

        std::string method_path = "/" + service_name + "/" + method_name;
        std::string host_data = zkCli.GetData(method_path.c_str());
        if (host_data == "")
        {
            controller->SetFailed(method_path + "is not exists!");
            return;
        }

        int idx = host_data.find(":");
        if (idx == -1)
        {
            controller->SetFailed(method_path + "address is invalid!");
            return;
        }

        std::string ip = host_data.substr(0, idx);
        uint16_t port = atoi(host_data.substr(idx + 1, host_data.length() - idx).c_str());

        // 使用tcp编程完成rpc方法的远程调用
        int clientfd = socket(AF_INET, SOCK_STREAM, 0);
        if (clientfd < 0)
        {
            controller->SetFailed("create socket error!");
            LOG_MAIN_ERROR << "create socket failed!";
        }
        struct sockaddr_in serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(port);
        serv.sin_addr.s_addr = inet_addr(ip.c_str());

        if (connect(clientfd, (struct sockaddr *)&serv, sizeof(serv)) != 0)
        {
            controller->SetFailed("connect error!");
            close(clientfd);
            exit(EXIT_FAILURE);
        }

        // 发送rpc请求
        if (send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0) <= 0)
        {
            controller->SetFailed("send error!");
            close(clientfd);
            return;
        }

        // 接收rpc请求的响应
        char buf[1024] = {0};
        int recv_size = 0;
        if ((recv_size = recv(clientfd, buf, 1024, 0)) <= 0)
        {
            controller->SetFailed("send error!");
            close(clientfd);
            return;
        }

        // 反序列化rpc调用的响应数据
        if (!response->ParseFromArray(buf, recv_size))
        {
            controller->SetFailed("parse error!");
        }

        close(clientfd);
        return;
    }

}