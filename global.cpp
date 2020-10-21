// Definition of the logging function

#include "global.h"

extern void LOG_IT(std::string info)
{
    openlog("kaskad", LOG_PID|LOG_CONS,LOG_USER);
    syslog(LOG_DEBUG, info.c_str());
    closelog();
}

extern const char* MAKE_PATH(std::string file_path)
{
    std::string full_path = BIN_PATH + file_path;
    return full_path.c_str();
}
