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

        Locker();
        ~Locker();

    private:
        std::mutex mtx;
    };

}