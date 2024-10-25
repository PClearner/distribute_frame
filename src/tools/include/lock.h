#pragma once

#include <memory>
#include <thread>
#include <mutex>

namespace star
{

    class Locker
    {

    public:
        typedef std::shared_ptr<Locker> ptr;

        Locker(std::mutex *locker);
        ~Locker();

    private:
        std::mutex *m_mtx;
    };

}