#include "mapreduce/include/worker.h"
// word count

std::vector<std::string> function(std::string ss)
{
    std::unordered_map<std::string, uint32_t> statistics;

    std::string record = "";

    for (size_t i = 0; i < ss.size(); i++)
    {
        if (ss[i] != ' ')
        {
            record.append(1, ss[i]);
        }
        else if (ss[i] == ',' || ss[i] == '.' || ss[i] == '?' || ss[i] == '!' || ss[i] == '-' || ss[i] == '\n' || ss[i] == '\'')
        {
            continue;
        }
        else
        {
            if (record != "")
            {
                auto iter = statistics.find(record);
                if (iter == statistics.end())
                {
                    statistics[record] = 1;
                }
                else
                {
                    statistics[record] += 1;
                }
                record.clear();
            }
        }
    }

    std::vector<std::string> res(statistics.size(), "");
    uint32_t index = 0;
    for (auto iter = statistics.begin(); iter != statistics.end(); iter++)
    {
        std::string tmp = iter->first + std::string(":") + std::to_string(iter->second);
        res[index] = tmp + '\n';
        std::cout << "res[" << index << "]:" << res[index] << std::endl;
        index++;
    }

    return res;
}

int main()
{

    star::Rpcinit::rpcinit("map2.yaml");

    star::map m_map(star::Rpcinit::GetInstance()->get_config("id"), function);

    return 0;
}