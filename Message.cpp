// Implementation of the Message class

#include "Message.h"
#include "definitions.h"
#include "debug.cpp"
#include "stdlib.h"
#include "unistd.h"
#include "linux/types.h"


Message::Message(vector<char> string) : raw(string)
{
    ProcessRaw();
}

Message::Message(char	      op_code,
		 User	      from,
		 User	      to,
		 vector<char> msg )
		 
	       : Operation_code(op_code),
		 FROM_login(from),
		 TO_login(to),
		 MESSAGE(msg),
		 Message_size(from.size() + to.size() + msg.size() + 3)
{
}


Message::~Message()
{
}

int Message::ProcessRaw()
{
Message_size = 0;

int size = raw.size();
if(size>0)
{
    int cursor=0;
    int size = raw.size();
    vector<char> temp;
    
    
    Operation_code = raw[cursor];
    cursor++;
        
//raw is now free of Operation code	

//Split message into SIZE, FROM, TO and MESSAGE parts
	std::string size_string;
	
	while((raw[cursor] != BORDER) && (cursor<size))
	{
	    size_string += raw[cursor];
	    cursor++;
	}
	size_string = size_string + '\0';
	
	if(cursor==size)
	{
#ifdef DEBUG_MODE
	    LOG_IT("\nFormat Error! Unable to find Message_size\n");
#endif
	    Operation_code = ERROR_REQUEST;
    	    cursor--;
	}
	Message_size = atol(size_string.c_str());
	if(Message_size < 3)
	    Operation_code = ERROR_REQUEST;
	
	size_string = "\0";
	
//Finding FROM_login	
	cursor++;
	while((raw[cursor] != BORDER) && (cursor<size))
	{
	    temp.push_back(raw[cursor]);
	    cursor++;
	}
	if(cursor==size)	    
	{
#ifdef DEBUG_MODE
	    LOG_IT("\nFormat Error! Unable to find FROM_login\n");
#endif		
	    Operation_code = ERROR_REQUEST;
	    cursor--;
	}
	FROM_login.UpdateRaw(temp);
	temp.erase(temp.begin(), temp.end());	    
	
//Finding TO_login	
	cursor++;
	while((raw[cursor] != BORDER) && (cursor<size))
	{
	    temp.push_back(raw[cursor]);
	    cursor++;
	}
	if(cursor==size)
	{
#ifdef DEBUG_MODE
	    LOG_IT("\nFormat Error! Unable to find TO_login\n");
#endif
	    Operation_code = ERROR_REQUEST;
	    cursor--;
	}	    
	TO_login.UpdateRaw(temp);
	
//Finding MESSAGE	
	cursor++;
	
	while(cursor<size)
	{
	    MESSAGE.push_back(raw[cursor]);
	    cursor++;
	}	
	 
//Low size check:///////////
	if(FROM_login.size() + TO_login.size() + MESSAGE.size() + 3 > Message_size)
	    Operation_code = ERROR_REQUEST;
////////////////////////////
	
}    
return 0;
}

/////////////////////////////////////////////////////////////////
vector<char> Message::PrepareRaw()
{
#ifdef DEBUG_MODE        	    
	LOG_IT("\nPrepareRaw begins...\n");
#endif

    vector<char> out;
    vector<char> temp;    
    int cursor = 0;
    char buf[30];
    snprintf(buf, sizeof(buf), "%d", Message_size);
    buf[sizeof(buf)-1] = '\0';
    std::string size_string = buf;
    
//////////////////Construction/////////
    out.push_back( Operation_code);
    
    for(int i = 0; i< size_string.size(); i++ )
	out.push_back( size_string[i] );
	
    out.push_back( BORDER);
    
    temp = FROM_login.PrepareRaw();
    for(int i = 0; i< temp.size(); i++ )
	out.push_back( temp[i] );
	
    out.push_back( BORDER );

    temp = TO_login.PrepareRaw();    
    for(int i = 0; i< temp.size(); i++ )
	out.push_back( temp[i] );
	
    out.push_back( BORDER );

    for(int i = 0; i< MESSAGE.size(); i++ )
	out.push_back( MESSAGE[i] );
	
    return out;
}

char Message::Get_CODE()
{
    return Operation_code;
}

User Message::Get_FROM()
{
    return FROM_login;
}

User Message::Get_TO()
{
    return TO_login;
}

vector<char> Message::Get_MESSAGE()
{
    return MESSAGE;
}
long Message::Get_SIZE()
{
    return Message_size;
}

void Message::Set_CODE(char code)
{
    Operation_code = code;
}

void Message::Set_FROM(User from)
{
    FROM_login = from;
}

void Message::Set_TO(User to)
{
    TO_login = to;
}

void Message::Update_SIZE()
{
    Message_size = FROM_login.size() + TO_login.size() + MESSAGE.size() +3;

}

void Message::Set_MESSAGE(vector<char> message)
{
    MESSAGE = message;
}

void Message::Add_TO_MESSAGE(vector<char> message)
{
    for(int i = 0; i< message.size(); i++)
	MESSAGE.push_back( message[i] );
}
