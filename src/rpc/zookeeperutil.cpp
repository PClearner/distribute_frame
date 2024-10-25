#include "rpc/include/zookeeperutil.h"

namespace star
{

    // ȫ�ֵ�watcher�۲���(��һ���첽�߳�), zkserver��zkclient����֪ͨ
    void global_watcher(zhandle_t *zh, int type, int state, const char *path,
                        void *watcherCtx)
    {
        if (type == ZOO_SESSION_EVENT)
        {
            // �ص�����Ϣ�����ǺͻỰ��ص���Ϣ����
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

    // zk�ͻ�����������zk�����
    void ZkClient::Start()
    {

        // ����zk��IP�Ͷ˿ںţ�Ĭ��Ϊ2181
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
        sem_init(&sem, 0, 0);             // ��ʼ����ԴΪ0
        zoo_set_context(m_zhandle, &sem); // ���������ģ���Ӷ�����Ϣ

        sem_wait(&sem);
        LOG_MAIN_INFO << "zookeeper_init success!";
    }

    // ��zkserver�ϸ���ָ����path����znode���
    void ZkClient::Create(const char *path, const char *data,
                          int datalen, int state)
    {
        char path_buffer[128];
        int bufferlen = sizeof(path_buffer);

        int flag = zoo_exists(m_zhandle, path, 0, nullptr);
        if (flag == ZNONODE)
        {
            // ��㲻����, �򴴽����
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

    // �������ָ������·��, ��ȡznode����ֵ
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
            // ��ȡ�ɹ�
            return buffer;
        }
    }

}
