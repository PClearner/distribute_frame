#include "rpc/include/init.h"

namespace star
{

    bool checkyaml(std::string file)
    {
        for (size_t i = 0; i < file.size(); i++)
        {
            if (file[i] == '.')
            {
                if (file.substr(i + 1) == "yaml")
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        return false;
    }

    void Rpcinit::init(Rpcinit *m, std::string yamlfile)
    {
        if (yamlfile == "" || !checkyaml(yamlfile))
        {
            LOG_MAIN_ERROR << "Rpc init config file is not exist!!";
            assert("Rpc init error");
        }

        YAML::Node config = YAML::LoadFile(yamlfile);

        if (config["MainLogger"])
        {
            getMainLogger()->set_file(config["MainLogger"]["file"].as<std::string>());

            if (config["MainLogger"]["model"].as<std::string>() == "all")
            {
                getMainLogger()->set_model(Output::all);
            }
            else if (config["MainLogger"]["model"].as<std::string>() == "print")
            {
                getMainLogger()->set_model(Output::print);
            }
            else if (config["MainLogger"]["model"].as<std::string>() == "write")
            {
                getMainLogger()->set_model(Output::write);
            }
            else
            {
                LOG_MAIN_ERROR << "MainLogger model init error";
                assert("Rpc init error");
            }

            if (config["MainLogger"]["level_threshold"].as<std::string>() == "INFO")
            {
                getMainLogger()->set_level_threshold(Logger::Level::Info);
            }
            else if (config["MainLogger"]["level_threshold"].as<std::string>() == "DEBUG")
            {
                getMainLogger()->set_level_threshold(Logger::Level::Debug);
            }
            else if (config["MainLogger"]["level_threshold"].as<std::string>() == "ERROR")
            {
                getMainLogger()->set_level_threshold(Logger::Level::Error);
            }
            else if (config["MainLogger"]["level_threshold"].as<std::string>() == "FATAL")
            {
                getMainLogger()->set_level_threshold(Logger::Level::Fatal);
            }
            else
            {
                LOG_MAIN_ERROR << "MainLogger level_threshold init error";
                assert("Rpc init error");
            }
        }

        if (config["net"])
        {
            m->config_map["ip"] = config["net"]["ip"].as<std::string>();
            m->config_map["port"] = config["net"]["port"].as<std::string>();
        }

        if (config["zknet"])
        {
            m->config_map["zkip"] = config["zknet"]["ip"].as<std::string>();
            m->config_map["zkport"] = config["zknet"]["port"].as<std::string>();
        }

        LOG_MAIN_INFO << "Rpcinit over";
        m->if_init = true;
    }

    Rpcinit::~Rpcinit() {}

    Rpcinit *Rpcinit::GetInstance()
    {
        static Rpcinit* m_rpc = new Rpcinit();
        return m_rpc;
    }

    void Rpcinit::rpcinit(std::string yamlfile)
    {
        Rpcinit::init(Rpcinit::GetInstance(), yamlfile);
    }

}