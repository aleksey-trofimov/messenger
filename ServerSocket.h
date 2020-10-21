// Definition of the ServerSocket class

#ifndef ServerSocket_class
#define ServerSocket_class

#include "Socket.h"
#include "unistd.h"
#include "vector.h"
#include "Message.h"
#include "sys/types.h"
#include "fcntl.h"
#include "global.h"

class ServerSocket : private Socket
{
public:

    ServerSocket ( int port );
    ServerSocket (){};
    virtual ~ServerSocket();

    void	send( const vector<char>) const;
    int		recv ( vector<char>&); 
    void	accept ( ServerSocket& );  
    void	set_non_blocking ( const bool );
    
    void	Set_OWNER(const std::string s)
		 {owner = s;
		  owner_base_path = "msgbases/" + owner + '/' + "msgbase";}
    
    std::string	Get_OWNER() const {return owner;}
    std::string	Get_OWNER_BASE_PATH() const {return owner_base_path;}
    
    //Add active user to the end of users_names array.
    void	AddActiveUser(const std::string& s)    
		{ users_names.push_back(s); }
		
    //Add active user status to the end of users_status array.
    void	AddActiveStatus(const std::string& s)    
		{ users_status.push_back(s); }
		
    //Get status at specified position
    vector<char> GetActiveStatusList();
		
    //Clear users_names array
    void	ClearActiveUsersNames()
		{
		    users_names.erase(users_names.begin(), users_names.end());
		}
		
    //Clear users_status array	
    void	ClearActiveUsersStatus()
		{
		    users_status.erase(users_status.begin(), users_status.end());
		}
    
    void	logout(vector<std::string>*);
    int		Take_MESSAGE(vector<std::string>*, vector<char>*);
    int		Compare_LISTS(const vector<std::string>*);
    int		Get_SOCK() const {return Socket::get_sock();}
    bool 	is_valid() const { return Socket::is_valid(); }
    

private:
    
    std::string owner;
    std::string owner_base_path;
    vector<std::string> users_names;
    vector<std::string> users_status;
};


#endif
