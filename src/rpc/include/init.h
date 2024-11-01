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

        static void init(Rpcinit *m, std::string yamlfile = "");
        static void rpcinit(std::string yamlfile = "");
        ~Rpcinit();

        inline std::string get_config(std::string key)
        {
            return config_map[key];
        }

    private:
        Rpcinit() {};
        Rpcinit(const Rpcinit &) = delete;
        Rpcinit(Rpcinit &&) = delete;
        std::unordered_map<std::string, std::string> config_map;
        bool if_init = false;
    };

}