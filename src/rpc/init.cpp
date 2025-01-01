#include "rpc/include/init.h"

namespace star
{


    Rpcinit *Rpcinit::GetInstance()
    {
        static Rpcinit *m_rpc = new Rpcinit();
        return m_rpc;
    }

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

        for (auto iter = config.begin(); iter != config.end(); iter++)
        {

            if (iter->first.as<std::string>() == "MainLogger")
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
            else
            {
                Rpcinit::GetInstance()->config_map[iter->first.as<std::string>()] = iter->second.as<std::string>();
            }
        }

        LOG_MAIN_DEBUG << "Rpcinit over";
        m->if_init = true;
    }

    Rpcinit::~Rpcinit() {}

    void Rpcinit::rpcinit(std::string yamlfile)
    {
        Rpcinit::init(Rpcinit::GetInstance(), yamlfile);
    }

}