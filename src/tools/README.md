日志器默认生成的话，是打印终端的输出形式，日志级别是Debug。若要修改的话可以查看头文件中接口，查看属性也可以通过头文件中的接口查看。

——————————————————————————————————————————————————————————————————————————————————————————————
写日志通过logwork函数，下面是示例:
logger.logwork(str,Debug)

————————————————————————————————————————————————————————————————————————————————————————————
本模块还设置了主日志器，通过getMainLogger()即可获取其指针，不用自己创建日志器。
下面是使用流式操作：
若自己创建了日志器，则:

LOG_INFO(logger, level) << "xxxxx";
LOG_DEBUG(logger, level) << "xxxxx";
LOG_ERROR(logger, level) << "xxxxx";
LOG_FATAL(logger, level) << "xxxxx";
即可

使用主日志器，则：
LOG_MAIN_INFO
LOG_MAIN_DEBUG
LOG_MAIN_ERROR
LOG_MAIN_FATAL

——————————————————————————————————————————————————————————————————————————————————————————————————
若要重启日志器，运行restart函数即可，删除日志器则killlogger函数。

___________________________________________________________________________________________________
代码案例在app/tools目录下