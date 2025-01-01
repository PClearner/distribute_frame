#pragma once

#include "tools/include/logger.h"
#include "yaml-cpp/yaml.h"
#include <unordered_map>

namespace star
{
    static std::string error_code()
    {
        return "error";
    }

    class Rpcinit
    {
    public:
        static Rpcinit *GetInstance();

        static void init(Rpcinit *m, std::string yamlfile = "");
        static void rpcinit(std::string yamlfile = "");
        ~Rpcinit();

        inline std::string get_config(std::string key)
        {
            auto s = config_map.find(key);
            if (s == config_map.end())
            {
                return error_code();
            }
            else
            {
                return s->second;
            }
        }

    private:
        Rpcinit() {};
        Rpcinit(const Rpcinit &) = delete;
        Rpcinit(Rpcinit &&) = delete;
        std::unordered_map<std::string, std::string> config_map;
        bool if_init = false;
    };

}