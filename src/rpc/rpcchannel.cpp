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

        // ��ȡ���������л��ַ�������args_size
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

        // ����rpc������header
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

        // ��֯�����͵�rpc������ַ���
        std::string send_rpc_str;
        send_rpc_str.insert(0, std::string((char *)&header_size, 4)); // header_size
        send_rpc_str += rpc_header_str;                               // rpcheader
        send_rpc_str += args_str;                                     // args

        // ��ӡ������Ϣ
        LOG_MAIN_INFO << "======================";
        LOG_MAIN_INFO << "header_size: " << header_size;
        LOG_MAIN_INFO << "rpc_header_str: " << rpc_header_str;
        LOG_MAIN_INFO << "service_name: " << service_name;
        LOG_MAIN_INFO << "method_name: " << method_name;
        LOG_MAIN_INFO << "args_str: " << args_str;
        LOG_MAIN_INFO << "======================";

        /* ��ȡ�����ļ�rpcserver����Ϣ
        */

        // rpc���÷���, ��Ҫ��ѯzookeeper�ϸ÷����ip:port��Ϣ
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

        // ʹ��tcp������rpc������Զ�̵���
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

        // ����rpc����
        if (send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0) <= 0)
        {
            controller->SetFailed("send error!");
            close(clientfd);
            return;
        }

        // ����rpc�������Ӧ
        char buf[1024] = {0};
        int recv_size = 0;
        if ((recv_size = recv(clientfd, buf, 1024, 0)) <= 0)
        {
            controller->SetFailed("send error!");
            close(clientfd);
            return;
        }

        // �����л�rpc���õ���Ӧ����
        if (!response->ParseFromArray(buf, recv_size))
        {
            controller->SetFailed("parse error!");
        }

        close(clientfd);
        return;
    }

}