#include <string>
#include <stdio.h>
#include "vector.h"
#include "sys/types.h"
#include <fcntl.h>
#include "sys/time.h"
#include "sys/stat.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/resource.h>
#include <signal.h>
#include <syslog.h>
#include <pwd.h>

#include "ServerSocket.h"
#include "SocketException.h"
#include "Message.h"
#include "User.h"
#include "Semaphore.h"
#include "definitions.h"
#include "debug.cpp"
#include "global.h"

//Global definitions

vector<std::string> messages_array;
vector<User> users_array;
vector<std::string> active_users_array;
vector<char> empty_vector;

CSemaphore processing_sema;

long ID_IN_ARRAY(long id);
int PASS_IN_ARRAY(std::string pass, long i);
#include "idinarray.cpp"

void* run_client(void* new_sock);
#include "run_client.cpp"


//-----------------------------------------------------------------

int main ( int argc, int argv[] )
{

std::cout << "\nkaskad, server for Vaska v2.51b\n";
std::cout << "Recovering previous state..\n";
std::cout << "Entering daemon mode.\n";

//////////CHANGING USER/////////////////////////////
struct passwd* pwd = getpwnam("nobody"); //Convert user name to UID
uid_t UID = pwd->pw_uid;
gid_t GID = pwd->pw_gid;
struct stat statv;
if(UID == (uid_t) - 1)
    std::cout<<"Invalid user name\n";
else
{ 
    setgid(GID);
    setuid(UID);
}
////////////////////////////////////////////////////
#include "file_load.cpp"
std::cout << "running....\n";


//////////DAEMON CREATION////////////////////////////

int fd;
struct rlimit flim;
if(getpid()!=1)
{

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    
    if(fork() != 0)
	exit(0);
    setsid();
}
getrlimit(RLIMIT_NOFILE, &flim);
for(fd=0;fd<flim.rlim_max;fd++)
    close(fd);
chdir("/");

LOG_IT("////////////KASKAD STARTED//////////////");

/////////DAEMON STARTED//////////////////////////////
try
{
// Create socket
    ServerSocket server ( PORT );
    while ( true )
    {
    
	ServerSocket* new_sock = new ServerSocket;
	
	server.accept ( *new_sock );

        pthread_t client_thread;
        pthread_create( &client_thread, 0, run_client, (void*)new_sock );
	pthread_detach(client_thread);
//	if(pthread_detach(client_thread) != 0)
//	    LOG_IT("Detach failed. Contact me if you see this message");
	    
    }
}
catch ( SocketException& e )
{
    LOG_IT("Exception in the end. Kaskad terminated. \n");
}

    return 0;
  
}


