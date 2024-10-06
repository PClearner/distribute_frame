#include "tools/include/logger.h"

int main()
{
    std::cout << "start" << std::endl;
    LOG_MAIN_INFO << "hello world";
    LOG_MAIN_FATAL << "hello world";
    LOG_MAIN_DEBUG << "hello world";
    return 0;
}
