// Definition of the User class

#ifndef User_class
#define User_class

#include <string>
#include "stdio.h"
#include "vector.h"
#include "definitions.h"
#include "global.h"


class User
{
public:

    User ( vector<char> array):
				id_string("\0"),
				password("\0"),
				nick("\0"),
				name("\0"),
				family("\0"),
				father("\0"),
				id(0),
				raw(array)
	    {
	        ProcessRaw();
	    }
	    
    User ():
	    id_string("\0"),
	    password("\0"),
	    nick("\0"),
	    name("\0"),
	    family("\0"),
	    father("\0"),
	    id(0)
	    {
	    }
	    
    User (const User& u):
    	    id_string(u.id_string),
	    password(u.password),
	    nick(u.nick),
	    name(u.name),
	    family(u.family),
	    father(u.father),
	    id(u.id),
	    raw(u.raw)
	    {
	    }



User ( std::string string):
				id_string("\0"),
				password("\0"),
				nick("\0"),
				name("\0"),
				family("\0"),
				father("\0"),
				id(0)
	    {
		vector<char> array;
		for(int i = 0; i< string.size(); i++)
		    array.push_back(string[i]);
		raw = array;
	        ProcessRaw();
	    }
	    
    virtual	~User();
    int		ProcessRaw();
    
//Creates string to transfer    
    vector<char> PrepareRaw();
    
    long	Get_ID();
    std::string	Get_ID_STRING();
    std::string	Get_NICK();
    std::string Get_PASSWORD();
    std::string Get_NAME();
    std::string Get_FAMILY();
    std::string Get_FATHER();
    int		size();
    void	Set_ID(long);
    void	Set_ID(std::string);
    void	Set_PASSWORD(std::string s) { password = s; }
    void	Set_FATHER(std::string s){ father = s; }
    
    void	Set_FIELDS(
    			    std::string	i,
			    std::string	p,
			    std::string	n,
			    std::string	nm,
			    std::string	f,
			    std::string	fr
			  )
		{
		    id_string = i;
		    password = p;
		    nick = n;
		    name = nm;
		    family = f;
		    father = fr;
		    id = atol(id_string.c_str());
		}
    void	UpdateRaw(vector<char>);
    
		  
private:
	vector<char>	raw;
	long		id;
	std::string	id_string;
	std::string	password;
	std::string	nick;
	std::string	name;
	std::string	family;
	std::string	father;  
};


#endif
