// Implementation of the ServerSocket class

#include "ServerSocket.h"
#include "SocketException.h"
#include "User.h"
#include "debug.cpp"


ServerSocket::ServerSocket ( int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create server socket." );
    }

  if ( ! Socket::bind ( port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

  if ( ! Socket::listen() )
    {
      throw SocketException ( "Could not listen to socket." );
    }
    
}

ServerSocket::~ServerSocket()
{
}



void ServerSocket::send( const vector<char> v ) const
{
    int i = Socket::send ( v );
  if ( !i  )
    {
      throw SocketException ( "Could not write to socket." );
    }
}

int ServerSocket::recv ( vector<char>& v ) 
{
    int i = Socket::recv ( v );
    
  if (i == -1)
    {
	return -1; //No message arrived
    }
  
  if ( i == 0 )
    {
      throw SocketException ( "Could not read from socket." );
      return 0; //Error     
    }
    return i; //Success
}


void ServerSocket::accept ( ServerSocket& sock )
{
  if ( ! Socket::accept ( sock ) )
    {
      throw SocketException ( "Could not accept socket." );
    }
}

void ServerSocket::set_non_blocking ( const bool b )
{
    Socket::set_non_blocking(b);
}

void ServerSocket::logout(vector<std::string>* active_array)
{
#ifdef DEBUG_MODE
		LOG_IT("LOGOUT sequence initiated..\n");
#endif
	    	for(int i = 0; i< active_array->size(); i++)
		    if(owner == active_array->operator[](i))
    			active_array->erase(active_array->begin()+i);
#ifdef DEBUG_MODE
		LOG_IT("USER logged out.\n");
#endif
}

int ServerSocket::Take_MESSAGE(vector<std::string>* message_array, vector<char>* answer)
{
    int flag = 0;

////////////////////////////////
    int j = message_array->size();
    char c = '\0';
    
    for(int k = 0; k<j; k++)
	if(message_array->operator[](k) == owner)
//Fill answer with contents of owner`s msgbase
	{
#ifdef DEBUG_MODE
	    LOG_IT("MESSAGE for USER found!\n");
#endif
	    flag = 1;
	    int messages_descriptor;
	    messages_descriptor = open(MAKE_PATH(owner_base_path), O_RDONLY);
	    
	    while(read(messages_descriptor, &c, 1) > 0)	
	    	answer->push_back(c);
	    close(messages_descriptor);
//erase msgbase
	    
	    messages_descriptor = open(MAKE_PATH(owner_base_path), O_RDONLY|O_CREAT|O_TRUNC);
	    close(messages_descriptor);
//erase record in messages_array
	    message_array->erase(message_array->begin()+k);		
	    
	}
	
	if(flag == 1) //If MESSAGE for USER not found
        {
#ifdef DEBUG_MODE
	    LOG_IT("MESSAGE for USER removed from array.\n");
#endif
	    return 0;
        }

	
	if(flag == 0) //If MESSAGE for USER not found
	{
#ifdef DEBUG_MODE
//		LOG_IT("MESSAGE for USER not found!\n");
#endif
		return -1;
	}

/////////////////////////////////////////////
	    
}

////Check if there are any changes in users status
////If yes, update users_status array

int ServerSocket::Compare_LISTS(const vector<std::string>* active_users)
{
    vector<std::string> new_status_list;
    int flag;
    
//Create updated version of status array in new_status_list
    for(int i = 0; i< users_names.size(); i++)
    {
	flag = 0;
        for(int j = 0; j< active_users->size(); j++)
	{
	    if(users_names[i] == active_users->operator[](j))
		{
		    
		    flag = 1;		    
		}		
	}
	if(flag == 1)
	    new_status_list.push_back("Y");
	else
	    new_status_list.push_back("N");	    
    }
    flag = 0;    
    
//Compare new_status_list and previous version of status list
    for( int i = 0; i<users_status.size(); i++ )    
    {
	if(users_status[i] != new_status_list[i])
	    {
		flag = 1;
		//Update users_status array
		users_status[i] = new_status_list[i];
	    }
    }
    
    if(flag == 1) return -1; //Changes detected
    else return 0;
}
vector<char> ServerSocket::GetActiveStatusList()    
{
    vector<char> v;
    for(int j = 0; j< users_status.size(); j++)
    v.push_back(  (users_status[j])[0] );
	
    return v;
}

