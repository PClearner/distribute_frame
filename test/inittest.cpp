#include "rpc/include/init.h"

int main()
{
    star::Rpcinit::rpcinit("config.yaml");
    LOG_MAIN_INFO << "show level:" << star::getMainLogger()->get_level_threshold();
    LOG_MAIN_INFO << "show ip:" << star::Rpcinit::GetInstance()->get_config("ip");
    LOG_MAIN_INFO << "show port:" << star::Rpcinit::GetInstance()->get_config("port");
    return 0;
}