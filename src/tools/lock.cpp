#include "tools/include/lock.h"

namespace star
{

    Locker::Locker()
    {
        this->mtx.lock();
    }

    Locker::~Locker()
    {
        this->mtx.unlock();
    }

}
