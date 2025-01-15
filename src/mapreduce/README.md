前置条件：需要自行安装protobuf,zookeeper

sudo apt-get install libzookeeper-mt-dev zookeeper zookeeperd
sudo apt-get install protobuf-compiler libprotobuf-dev

zookeeper还需要官网下载，本项目用的是zookeeper3.8.4

___________________________________________________________________________________________________________
还使用了moduo库和yaml-cpp库，不过都集成在项目里面了，不用自行下载

______________________________________________________________________________________________________________
使用流程：
根据你的需要自行写不限数目的map程序和reduce程序，map函数和reduce函数需要自己实现。

map的配置文件格式：
MainLogger:
  type: logger
  file: xxx
  model: all
  level_threshold: DEBUG

Logger看个人需求自行设置

id: mapi
mapi_ip: ip
mapi_port: 8081


reduce_size: n

reduce1_ip: ip
reduce1_port: port

......

reducen_ip: ip
reducen_port: port


reduce1log: log1path
......
reducenlog: lognpath



zkip: ip
zkport: 2181








reduce的配置文件格式：
MainLogger:
  type: logger
  file: xxx
  model: all
  level_threshold: DEBUG
Logger看个人需求自行设置

id: reducen
reducen_ip: ip
reducen_port: ip


zkip: ip
zkport: 2181



master程序假如没有自行编写切片策略的话则会使用默认切片策略，默认切片策略则是按行来切片，下面是切片函数
    class default_cut
    {
    public:
        default_cut(uint64_t volum = 500)
        {
            m_volum = volum;
        }

        virtual std::string cut(std::string file_path)
        {
            std::string line;
            std::ifstream file(file_path);
            std::string res;
            file.seekg(index, std::ios::beg);
            while (std::getline(file, line))
            {
                res.append(line);
                if (res.size() >= m_volum)
                {
                    break;
                }
            }

            if (res.size() == 0)
            {
                index = 0;
                return res;
            }
            index += res.size();

            return res;
        }

        uint64_t m_volum;
        uint64_t index = 0;
    };

默认切片自带索引和切片大小限制，若要自行编写切片策略的话继承default_cut然后重新编写cut函数即可。


master配置文件格式如下：
MainLogger:
  type: logger
  file: xxx
  model: all
  level_threshold: DEBUG
Logger看个人需求自行设置

infile: inputfiles
注意：infile可以不用填，你需要在master程序里面自行输入要mapreduce的文件即可，使用infile只是方便自己动态修改输入文件

outfile: outputfile

master_ip: ip
master_port: port

zkip: ip
zkport: 2181


注意：文件名假如是带有目录的话需要自行创建路径比如logpath = reducelog/1logmap1,outputfile = res/result.txt。

tips：程序运行时出现get znode:map(n+1) error日志这个属于正常现象（注意是n+1）。
假如出现一大堆fd connection等，将程序关闭了重新启动即可。
假如出现flag=-101报文，这是zookeeper的问题，只需要将程序关闭后等待一会再启动即可（最多等待35s）。


启动顺序：先启动map程序，全部启动且满足tips之后，再启动reduce程序，全部启动无误后启动master程序即可。最后会输出到master的outfile配置中的文件名字上。当master报文中出现"all reduce data receive successfully"则说明计算完成，此时可以将master程序关闭。

______________________________________________________________________________________________________________
关于接口使用，可以查看app/mapreduce目录下的例程,该例程使用的是2个map2个reduce。
生成路径在bin/mapreduce下,该例程使用《简爱》作为输入，实现词频统计。结果在result.txt文件里面，result.txt里面对于词频给的统计效果不是很好，主要因为我map和reduce写的有问题，导致统计词频筛选不是很好。
但是本例程只是作为一个示例，对于map和reduce都由使用者编写，所以自己写得不是很好其实无伤大雅。