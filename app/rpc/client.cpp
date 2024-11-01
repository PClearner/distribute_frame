#include "rpc/include/rpcprovider.h"
#include "rpc/include/rpcchannel.h"
#include "rpc/userservice.pb.h"
#include <iostream>

int main()
{
    star::Rpcinit::rpcinit("config.yaml");

    Userservice_Stub stub(new star::rpcChannel());

    args request;
    request.set_arg1(32);
    request.set_arg2(10);

    results response;
    stub.Add(nullptr, &request, &response, nullptr);

    LOG_MAIN_INFO << "response = " << response.res1();

    return 0;
}