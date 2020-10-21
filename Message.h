// Definition of the Message class

#ifndef Message_class
#define Message_class

#include <string>
#include "stdio.h"
#include "vector.h"
#include "definitions.h"
#include "User.h"
#include "global.h"




class Message
{
public:

    Message ( vector<char> string );
    
    Message ( 	char	     op_code,
		User 	     from,
		User         to,
		vector<char> msg
	    );
		
    virtual	~Message();
    int		ProcessRaw();
    
//Creates string to transfer    
    vector<char> PrepareRaw();
    
    char	Get_CODE();
    long	Get_SIZE();
    User	Get_FROM();
    User	Get_TO();
    vector<char> Get_MESSAGE();
    void	Set_FIELDS(char		code,
			   User		from,
			   User		to,
			   vector<char>	message) 
		{
		    Operation_code = code;
		    FROM_login = from;
		    TO_login = to;
		    MESSAGE = message;
		    Message_size = 
		    3
		    +	FROM_login.size()
		    +	TO_login.size()
		    +	MESSAGE.size();
		}
		
    void	Set_CODE(char code);
    void	Set_FROM(User);
    void	Set_TO(User);
    void	Set_MESSAGE(vector<char>);
    void	Add_TO_MESSAGE(vector<char>);
    void	Update_SIZE();
  
private:

    vector<char>	raw;
    char	Operation_code;
    long	Message_size;
    User	FROM_login;
    User	TO_login;
    vector<char> MESSAGE;
  
};


#endif
