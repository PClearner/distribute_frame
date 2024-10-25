#include "rpc/include/zookeeperutil.h"

namespace star
{

    // 全局的watcher观察器(是一个异步线程), zkserver给zkclient发送通知
    void global_watcher(zhandle_t *zh, int type, int state, const char *path,
                        void *watcherCtx)
    {
        if (type == ZOO_SESSION_EVENT)
        {
            // 回调的消息类型是和会话相关的消息类型
            if (state == ZOO_CONNECTED_STATE)
            {
                sem_t *sem = (sem_t *)zoo_get_context(zh);
                sem_post(sem);
            }
        }
    }

    ZkClient::ZkClient() : m_zhandle(nullptr)
    {
    }

    ZkClient::~ZkClient()
    {
        if (m_zhandle != nullptr)
        {
            zookeeper_close(m_zhandle);
        }
    }

    // zk客户端启动连接zk服务端
    void ZkClient::Start()
    {

        // 加载zk的IP和端口号，默认为2181
        std::string host = Rpcinit::GetInstance()->get_zkip();
        std::string port = Rpcinit::GetInstance()->get_zkport();
        std::string connstr = host + ":" + port;

        m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
        if (m_zhandle == nullptr)
        {
            LOG_MAIN_ERROR << "zookeeper init error!";
            exit(EXIT_FAILURE);
        }

        sem_t sem;
        sem_init(&sem, 0, 0);             // 初始化资源为0
        zoo_set_context(m_zhandle, &sem); // 设置上下文，添加额外信息

        sem_wait(&sem);
        LOG_MAIN_INFO << "zookeeper_init success!";
    }

    // 在zkserver上根据指定的path创建znode结点
    void ZkClient::Create(const char *path, const char *data,
                          int datalen, int state)
    {
        char path_buffer[128];
        int bufferlen = sizeof(path_buffer);

        int flag = zoo_exists(m_zhandle, path, 0, nullptr);
        if (flag == ZNONODE)
        {
            // 结点不存在, 则创建结点
            flag = zoo_create(m_zhandle, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE,
                              state, path_buffer, bufferlen);
            if (flag == ZOK)
            {
                LOG_MAIN_INFO << "znode create successfully!";
            }
            else
            {
                LOG_MAIN_ERROR << "znode create error!\n"
                               << "flag: " << flag << "\nznode create error... path: " << path;
                exit(EXIT_FAILURE);
            }
        }
    }

    // 传入参数指定结点的路径, 获取znode结点的值
    std::string ZkClient::GetData(const char *path)
    {
        char buffer[64];
        int bufferlen = sizeof(buffer);
        int flag = zoo_get(m_zhandle, path, 0, buffer, &bufferlen, nullptr);

        if (flag != ZOK)
        {
            LOG_MAIN_ERROR << "get znode data error!\n"
                           << "get znode error... path: " << path;
            return "";
        }
        else
        {
            // 获取成功
            return buffer;
        }
    }

}
