// Implementation of the User class

#include "User.h"
#include "definitions.h"
#include "debug.cpp"
#include "stdlib.h"
#include "unistd.h"
#include "linux/types.h"


User::~User()
{
}

int User::ProcessRaw()
{

int size = raw.size();
if(size>0)
{
	
//Split string into id, nick, password, name, family and father parts
	int i=0;
	
	id_string.erase(id_string.begin(), id_string.end());
	
	while((raw[i] != USER_BORDER) && (i<size))
	{
	    id_string += raw[i];	    
	    i++;
	}
	
	if(i==size)
	{
#ifdef DEBUG_MODE
	    LOG_IT("\nFormat Error! Unable to find id\n");
#endif
    	    i--;
	}
	i++;

//Finding password...
	password.erase(password.begin(), password.end());
	while((raw[i] != USER_BORDER) && (i<size))
	{
	    password += raw[i];	    
	    i++;
	}
	if(i==size)	    
	{
#ifdef DEBUG_MODE
	    LOG_IT("\nFormat Error! Unable to find password\n");
#endif		
	    i--;
	}
	i++;
	
//Finding nick...
	nick = "\0";
	
	while((raw[i] != USER_BORDER) && (i<size))
	{
	    nick += raw[i];
	    i++;
	}
	if(i==size)
	{
#ifdef DEBUG_MODE
	    LOG_IT("\nFormat Error! Unable to find nick\n");
#endif
	    i--;
	}	    
	i++;

//Finding name...
	name = "\0";
	
	while((raw[i] != USER_BORDER) && (i<size))
	{
	    name += raw[i];
	    i++;
	}
	if(i==size)
	{
#ifdef DEBUG_MODE
	    LOG_IT("\nFormat Error! Unable to find name\n");
#endif
	    i--;
	}	    
	i++;
	
//Finding family...
	family = "\0";
	
	while((raw[i] != USER_BORDER) && (i<size))
	{
	    family += raw[i];
	    i++;
	}
	if(i==size)
	{
#ifdef DEBUG_MODE
	    LOG_IT("\nFormat Error! Unable to find family\n");
#endif
	    i--;
	}	    
	i++;

//Finding father...
	father = "\0";
	
	while((raw[i] != USER_BORDER) && (i<size))
	{
	    father += raw[i];
	    i++;
	}
	
	id = atol(id_string.c_str());
	
}    
return 0;
}

/////////////////////////////////////////////////////////////////
vector<char> User::PrepareRaw()
{
        
    std::string accumulator = "\0";
    vector<char> out;
    
    accumulator = (id_string + USER_BORDER + password + USER_BORDER + nick + USER_BORDER + name
		   + USER_BORDER + family + USER_BORDER + father);
	  
    int size = accumulator.size();
    for(int i=0; i<size; i++)
	out.push_back(accumulator[i]);
    
    return out;
}

long User::Get_ID()
{
    return id;
}

std::string User::Get_NICK()
{
    return nick;
}

std::string User::Get_PASSWORD()
{
    return password;
}

std::string User::Get_NAME()
{
    return name;
}

std::string User::Get_FAMILY()
{
    return family;
}

std::string User::Get_FATHER()
{
    return father;
}

std::string User::Get_ID_STRING()
{
    return id_string;
}

void User::Set_ID(long number)
{
    char buf[30];
    snprintf(buf, sizeof(buf), "%d", number);
    buf[sizeof(buf)-1] = '\0';
    
    id_string = buf;
    id = number;
}

void User::Set_ID(std::string string)
{
    id_string = string;
    id = 0;
    id = atol(string.c_str());
}
int User::size()
{
    return  5
    	    + id_string.size()
	    + password.size()
	    + nick.size()
	    + name.size()
	    + family.size()
	    + father.size();
}
void User::UpdateRaw(vector<char> v)
{
    raw = v;
    ProcessRaw();    
}

