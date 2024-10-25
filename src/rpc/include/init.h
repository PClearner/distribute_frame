#pragma once

#include "tools/include/logger.h"
#include "yaml-cpp/yaml.h"
#include <unordered_map>

namespace star
{

    class Rpcinit
    {
    public:
        static Rpcinit *GetInstance();

        static void init(Rpcinit *m,std::string yamlfile = "");
        static void rpcinit(std::string yamlfile = "");
        ~Rpcinit();

        inline std::string get_ip()
        {
            return config_map["ip"];
        }

        inline std::string get_port()
        {
            return config_map["port"];
        }

        inline std::string get_zkip()
        {
            return config_map["zkip"];
        }

        inline std::string get_zkport()
        {
            return config_map["zkport"];
        }

    private:
        Rpcinit() {};
        Rpcinit(const Rpcinit &) = delete;
        Rpcinit(Rpcinit &&) = delete;
        std::unordered_map<std::string, std::string> config_map;
        bool if_init = false;
    };

}