#include "rpc/include/rpcprovider.h"
#include "rpc/userservice.pb.h"
#include <unistd.h>

class User : public Userservice
{
public:
    int32_t Add(int32_t arg1, int32_t arg2)
    {
        int32_t res = arg1 + arg2;
        LOG_MAIN_INFO << "calculate arg1 + arg2=" << res;
        return res;
    }

    int32_t multi(int32_t arg1, int32_t arg2)
    {
        int32_t res = arg1 * arg2;
        LOG_MAIN_INFO << "calculate arg1 * arg2=" << res;
        return res;
    }

    void Add(::google::protobuf::RpcController *controller,
             const ::args *request,
             ::results *response,
             ::google::protobuf::Closure *done)
    {
        int32_t arg1 = request->arg1();
        int32_t arg2 = request->arg2();

        int32_t res = Add(arg1, arg2);

        response->set_res1(res);
        response->set_success("sucess");

        done->Run();
    }

    void multi(::google::protobuf::RpcController *controller,
               const ::args *request,
               ::results *response,
               ::google::protobuf::Closure *done)
    {
        int32_t arg1 = request->arg1();
        int32_t arg2 = request->arg2();

        int32_t res = multi(arg1, arg2);

        response->set_res1(res);
        response->set_success("sucess");
        done->Run();
    }
};

int main()
{
    std::cout << "run" << std::endl;
    star::Rpcinit::rpcinit("config1.yaml");
    star::Rpcprovider mm;
    LOG_MAIN_DEBUG << "addservice";
    std::cout << "run in 59" << std::endl;

    mm.AddService(new User());
    return 0;
}