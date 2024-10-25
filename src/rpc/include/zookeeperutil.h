#pragma once

#include <string>
#include <iostream>
#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include "rpc/include/init.h"

// zookeeper�ͻ�����

namespace star
{

    class ZkClient
    {
    public:
        ZkClient();
        ~ZkClient();

        // zkclient��������zkserver
        void Start();

        // ��zkserver�ϸ��ݲ���path����znode���
        void Create(const char *path, const char *data,
                    int datalen, int state = 0);

        // ���ݲ���path��ȡznode����ֵ
        std::string GetData(const char *path);

    private:
        // zk�Ŀͻ��˾��
        zhandle_t *m_zhandle;
    };

}
