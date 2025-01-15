#include "mapreduce/include/worker.h"

// word count

std::unordered_map<std::string, uint64_t> func(std::string ss)
{
    std::unordered_map<std::string, uint64_t> statistics;

    std::string record = "";

    for (size_t i = 0; i < ss.size(); i++)
    {
        if (ss[i] != '\n')
        {
            record.append(1, ss[i]);
        }
        else
        {
            int idx = record.find(":");

            if (idx == -1)
            {
                assert(1 && "idx can't find");
                return statistics;
            }

            std::string word = record.substr(0, idx);
            uint64_t count = atoi(record.substr(idx + 1, record.length() - idx).c_str());
            statistics[word] = count;
            record.clear();
        }
    }

    return statistics;
}

int main()
{

    star::Rpcinit::rpcinit("reduce1.yaml");

    star::reduce m_reduce(star::Rpcinit::GetInstance()->get_config("id"), func);

    return 0;
}