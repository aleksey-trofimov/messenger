// Definition of the logging function

#ifndef global_h
#define global_h


#include <string>
#include "sys/types.h"
#include "sys/stat.h"
#include <sys/param.h>
#include <syslog.h>

#include "definitions.h"

extern void LOG_IT(std::string info);
extern const char* MAKE_PATH(std::string file_path);

#endif
