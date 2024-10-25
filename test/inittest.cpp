#include "rpc/include/init.h"

int main()
{
    star::Rpcinit::rpcinit("config.yaml");
    LOG_MAIN_INFO << "show level:" << star::getMainLogger()->get_level_threshold();
    LOG_MAIN_INFO << "show ip:" << star::Rpcinit::GetInstance()->get_ip();
    LOG_MAIN_INFO << "show port:" << star::Rpcinit::GetInstance()->get_port();
    LOG_MAIN_INFO << "show zkip:" << star::Rpcinit::GetInstance()->get_zkip();
    LOG_MAIN_INFO << "show zkport:" << star::Rpcinit::GetInstance()->get_zkport();
    LOG_MAIN_DEBUG << "show zkport:" << star::Rpcinit::GetInstance()->get_zkport();
    return 0;
}