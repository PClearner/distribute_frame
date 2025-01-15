#include "mapreduce/include/master.h"

int main()
{

    star::Rpcinit::rpcinit("master.yaml");

    std::vector<std::string> infiles;
    infiles.push_back(star::Rpcinit::GetInstance()->get_config("infile1"));

    star::master m_master(infiles);

    return 0;
}
