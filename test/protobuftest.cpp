#include "rpc/include/rpcheader.pb.h"
#include <iostream>
int main()
{
    star::RpcHeader aa;
    aa.set_service_name("hello world");
    std::cout << aa.service_name() << std::endl;
}