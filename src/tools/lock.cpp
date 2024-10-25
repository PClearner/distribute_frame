#include "tools/include/lock.h"

namespace star
{

    Locker::Locker(std::mutex *locker)
    {
        m_mtx = locker;
        this->m_mtx->lock();
    }

    Locker::~Locker()
    {
        this->m_mtx->unlock();
    }

}
