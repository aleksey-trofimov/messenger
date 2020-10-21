//Processing of the received message

switch(msg_to_process.Get_CODE())
{
//Store received message on server.
case SEND_MESSAGE:
    {
	User user_TO( msg_to_process.Get_TO() );

	int flag = 0;
#ifdef DEBUG_MODE
	LOG_IT("SEND_MESSAGE message type received\n");
#endif
	    if(ID_IN_ARRAY(user_TO.Get_ID()) != -1)
		{
#ifdef DEBUG_MODE
		 LOG_IT("login_TO found!\n");
#endif
		 flag = 1;
		}
	
	if(flag == 1)	  //If login_TO found, looking for login_FROM
	{
		
	    User user_FROM( msg_to_process.Get_FROM() );
	    
//Check user ID and PASSWORD
	    int test = ID_IN_ARRAY(user_FROM.Get_ID());
	    if( (test != -1) 
		 && (PASS_IN_ARRAY(user_FROM.Get_PASSWORD(), test)) )
	    {
#ifdef DEBUG_MODE
	        LOG_IT("login_FROM found!\n");
#endif
	        flag = 2; 
		    
	        //Removing FROM password
	        user_FROM.Set_PASSWORD(SOME_TEXT);
	        msg_to_process.Set_FROM(user_FROM);
	        msg_to_process.Update_SIZE();
	        //Removing complete
	    }	
	}  

		
	if(flag == 2)  //FROM & TO found
	{
/////RESTORE FILE TRANSFER CODE IF IT WAS FILE TRANSFER MESSAGE/////
	    if(check)
		msg_to_process.Set_CODE(SEND_FILE);

///////////////////////////////////////////////
//Store message in file
	    
//Check if user directory exists
	    std::string path = "msgbases/" + user_TO.Get_ID_STRING();
	    mkdir(MAKE_PATH(path), S_IRWXU|S_IRWXG|S_IRWXO);

	    int messages_descriptor;
	    std::string path_file = "msgbases/" + user_TO.Get_ID_STRING()
				  + "/msgbase";
	    messages_descriptor = open(MAKE_PATH(path_file), O_WRONLY|O_APPEND|O_CREAT);
	    chmod(MAKE_PATH(path_file), S_IRWXU);

	    vector<char> data = msg_to_process.PrepareRaw();
	    write(messages_descriptor, &data[0], data.size());	
	    
	    close(messages_descriptor);
////////////////////////////////////////////////////
//Check if we have message for this user already and push his ID in
//messages_array
	    int veryfy = 0;
	    std::string id_to_veryfy = msg_to_process.Get_TO().Get_ID_STRING();
	    for(int i = 0; i<messages_array.size(); i++)
		if(messages_array[i] == id_to_veryfy)
		    veryfy = 1;
	
	    if(!veryfy)
		messages_array.push_back(id_to_veryfy);
//Message now stored in messages_array.
	    
	    msg_to_process.Set_FIELDS
	    (
		SUCCESS_RECEIVED,
		empty_vector,
		empty_vector,
		empty_vector
	    );
	}
	else
	{
#ifdef DEBUG_MODE
	    LOG_IT("Message was not accepted: invalid USER name\n");
#endif
	    msg_to_process.Set_FIELDS
	    (
		ERROR_USER,
		empty_vector,
		empty_vector,
		empty_vector
	    );
	}	
    }
    break;
    
/*case TAKE_MESSAGE:
    {
	    //////////////!!!!!!!!!!!!!!    
//User's request to receive his messages (obsolete)
//#ifdef DEBUG_MODE
//	std::cout << "TAKE_MESSAGE message type received\n";
//#endif
	int flag = 0;
	int m_id = -1;
	
	User user_FROM( msg_to_process.Get_FROM() );
	    
//Check user ID and PASSWORD
	    int test = ID_IN_ARRAY(user_FROM.Get_ID());
	    if( ( test )
	         && ( PASS_IN_ARRAY(user_FROM.Get_PASSWORD(), test) ) )
	    {
//#ifdef DEBUG_MODE
//	        std::cout << "USER found!\n";
//#endif
	        flag = 1;
	    }
	    
	if(flag == 1) // If USER was found
	    {
		for(int i = 0; i< messages_array.size(); i++)
		{
		    User array_element(messages_array[i].Get_TO());
		    if(user_FROM.Get_ID() == array_element.Get_ID())
		    {
//#ifdef DEBUG_MODE
//			std::cout << "MESSAGE for USER found!\n";
//#endif
			flag = 2; m_id = i;							
		    }
		}
		    
		if(flag == 2) //If  MESSAGE for USER found
		    {
		    
			msg_to_process.Set_FIELDS
			(
			    SUCCESS_SENT,
			    messages_array[m_id].Get_FROM(),
			    messages_array[m_id].Get_TO(),
			    messages_array[m_id].Get_MESSAGE()
			);
			messages_array.erase(messages_array.begin()+m_id);
//#ifdef DEBUG_MODE
//			std::cout << "MESSAGE for USER sent and removed from array.\n";
//#endif
			
		    } 		
	    }	
	    if(flag == 1) //If MESSAGE for USER not found
	    {
//#ifdef DEBUG_MODE
//		std::cout << "MESSAGE for USER not found!\n";		
//#endif
		msg_to_process.Set_FIELDS
		(
		    ERROR_NOMESSAGE,
		    empty_vector,
		    empty_vector,
		    empty_vector
		);

	    }
	    if(flag == 0) //If USER not found
	    {
//#ifdef DEBUG_MODE
//		std::cout << "USER not found!\n";
//#endif
		msg_to_process.Set_FIELDS
		(
		    ERROR_USER,
		    empty_vector,
		    empty_vector,
		    empty_vector
		);
	    }

    }
    break;*/
    
////////////////////////////////////////////////////////////////////
	        
case REGISTER_USER:
    {
#ifdef DEBUG_MODE
	LOG_IT("REGISTER_USER message type received\n");
#endif
	User new_user(msg_to_process.Get_FROM());
	
	long new_user_id = 0;
	new_user_id = new_user.Get_ID();
	int flag = 0;

//If ID was received, check password and update user/////////////	
	
	int test = ID_IN_ARRAY(new_user_id);
	if(test != -1)
	    {
#ifdef DEBUG_MODE
	        LOG_IT("USER already registered!\n");
#endif

//IF FIND_MYSELF REQUESTED
		if( (PASS_IN_ARRAY(new_user.Get_PASSWORD(), test) )
		   && (new_user.Get_NICK() != "\0"))
		{
		
#ifdef DEBUG_MODE
	        LOG_IT("Password accepted!\n");
		LOG_IT("Nick accepted! User must be updated\n");
#endif
		users_array[test] = new_user;
		    msg_to_process.Set_FIELDS
		    (
			SUCCESS_REGISTRATION,
			empty_vector,
			empty_vector,
			users_array[test].PrepareRaw()
		    );		    
		    flag = 1;
		}
/////////////////
		if( (PASS_IN_ARRAY(new_user.Get_PASSWORD(), test) )
		    && (flag != 1) )
		{
		
#ifdef DEBUG_MODE
	        LOG_IT("Password accepted! User info must be sent\n");
#endif

		    msg_to_process.Set_FIELDS
		    (
			SUCCESS_REGISTRATION,
			empty_vector,
			empty_vector,
			users_array[test].PrepareRaw()
		    );		    
		    flag = 1;
		}



/////////////////
		if(flag != 1)
		{
		
#ifdef DEBUG_MODE
	        LOG_IT("Password incorrect!\n");
#endif
		    msg_to_process.Set_FIELDS
		    (
			ERROR_REGISTRATION,
			empty_vector,
			empty_vector,
			empty_vector
		    );
		    flag = 2;
		    
		}
	    }
	    
//flag == 0 ID was not found in array
//flag == 1 ID was found and user was successfully updated
//flag == 2 ID was found but password was incorrect

	    if((flag == 0) && (new_user_id == 0))	    
	    {
#ifdef DEBUG_MODE
		LOG_IT("ID was not found. New user registration!\n");
#endif
		if(users_array.size() != 0)
		    new_user.Set_ID(users_array[users_array.size()-1].Get_ID() + 1);
		else
		    new_user.Set_ID(1);
		users_array.push_back(new_user);	
///////////CREATE FOLDER FOR USER`S MESSAGES///////////////
		std::string path = "msgbases/" + new_user.Get_ID_STRING();
		mkdir(MAKE_PATH(path), S_IRWXU|S_IRWXG|S_IRWXO);
		
#ifdef DEBUG_MODE
		LOG_IT("USER registered successfully!\n");
#endif		
		std::string new_id = new_user.Get_ID_STRING();
		vector<char> v1;
		for(int i=0; i< new_id.size(); i++) v1.push_back(new_id[i]);
		msg_to_process.Set_FIELDS
		(
		    SUCCESS_REGISTRATION,
		    empty_vector,
		    empty_vector,
		    v1
		);	
	    
	    }
	    if((flag == 0) && (new_user_id != 0))	    
	    {
#ifdef DEBUG_MODE
		LOG_IT("Incorrect ID was found!\n");
#endif
		msg_to_process.Set_FIELDS
		(
		    ERROR_REGISTRATION,
		    empty_vector,
		    empty_vector,
		    empty_vector
		);		    
	    }	    
    }
    break;
    
///////////////////////////////////////////////////////////////////////////    
    
case DELETE_USER:
    {
#ifdef DEBUG_MODE
	LOG_IT("DELETE_USER message type received\n");
#endif
	User new_user(msg_to_process.Get_FROM());
	int flag = 0;
	
//ID must exist and passwords must be equal for deleting
	int test = ID_IN_ARRAY(new_user.Get_ID());
	if(test != -1)
	    {
#ifdef DEBUG_MODE
	        LOG_IT("USER found!\n");
#endif
		if(PASS_IN_ARRAY(new_user.Get_PASSWORD(), test) )
		{
		
#ifdef DEBUG_MODE
	    	    LOG_IT("Password accepted!\n");
#endif

		    users_array.erase(users_array.begin()+test);
/////////////////////////
//Delete user message file and directory
    
		    unlink(MAKE_PATH("msgbases/" + new_user.Get_ID_STRING() + "/" + "msgbase"));
		    rmdir(MAKE_PATH("msgbases/" + new_user.Get_ID_STRING()));
/////////////////////////		    
		    
#ifdef DEBUG_MODE
		    LOG_IT("USER deleted and removed from array.\n");
#endif
		    msg_to_process.Set_FIELDS
		    (
			SUCCESS_DELETE,
			empty_vector,
			empty_vector,
			empty_vector
		    );
		    flag = 1;
		}
		else
		{
#ifdef DEBUG_MODE
	    	    LOG_IT("Password incorrect!\n");
#endif		
		    msg_to_process.Set_FIELDS
		    (
			ERROR_USER,
			empty_vector,
			empty_vector,
			empty_vector
		    );		
		    flag = 2;
		}
	    }
		
//flag == 0 ID was not found in array
//flag == 1 ID was found and user was successfully deleted
//flag == 2 ID was found but password was incorrect

			    
	if(flag == 0)
	{	
#ifdef DEBUG_MODE
	    LOG_IT("USER not found!\n");
#endif
	    msg_to_process.Set_FIELDS
	    (
		ERROR_USER,
		empty_vector,
		empty_vector,
		empty_vector
	    );
	}	
    }
    break;
    
///////////////////////////////////////////////////////////////////////////

case LOGIN:
    {
#ifdef DEBUG_MODE
	LOG_IT("LOGIN message type received\n");
#endif
	User new_user(msg_to_process.Get_FROM());
	int flag_user_exists = 0;

//Check for user existance	

    int test = ID_IN_ARRAY(new_user.Get_ID());
    if( (test != -1) && (PASS_IN_ARRAY(new_user.Get_PASSWORD(), test)) )

/*    for(int j = 0; j< users_array.size(); j++)
	if((new_user.Get_ID() == users_array[j].Get_ID()) && (new_user.Get_PASSWORD() == users_array[j].Get_PASSWORD()))*/
	{	
	    int flag_user_active = 0;
	    flag_user_exists = 1;
	
	    for(int i = 0; i< active_users_array.size(); i++)
		if(new_user.Get_ID_STRING() == active_users_array[i])
		{
#ifdef DEBUG_MODE
	    	    LOG_IT("USER already logged in!\n");
#endif
		    msg_to_process.Set_FIELDS
		    (
			ERROR_LOGIN,
			empty_vector,
			empty_vector,
			empty_vector
		    );
	    
		    flag_user_active = 1;
		}
	    
	    if(flag_user_active == 0)	    
	    {
	        active_users_array.push_back(new_user.Get_ID_STRING());	
	        socket->Set_OWNER(new_user.Get_ID_STRING());
		
		int descriptor = open(MAKE_PATH(socket->Get_OWNER_BASE_PATH()), O_RDONLY|O_CREAT);
		chmod(MAKE_PATH(socket->Get_OWNER_BASE_PATH()), S_IRWXU);
		close(descriptor);

#ifdef DEBUG_MODE
		LOG_IT("USER logged in successfully!\n");
#endif
		msg_to_process.Set_FIELDS
		(
		    SUCCESS_LOGIN,
		    empty_vector,
		    empty_vector,
		    empty_vector
		);	    
	    }
	}
	if(flag_user_exists == 0) 
	{
#ifdef DEBUG_MODE
	    LOG_IT("USER not found!\n");
#endif
	    msg_to_process.Set_FIELDS
	    (
		ERROR_USER,
		empty_vector,
		empty_vector,
		empty_vector
	    );
	}
    }
    break;
    
///////////////////////////////////////////////////////////////////////////

case LOGOUT:
    {
#ifdef DEBUG_MODE
	LOG_IT("LOGOUT message type received\n");
#endif
	User new_user(msg_to_process.Get_FROM());

	int flag_user_active = 0;
	
	for(int i = 0; i< active_users_array.size(); i++)
	    if(new_user.Get_ID_STRING() == active_users_array[i])
		{
		    active_users_array.erase(active_users_array.begin()+i);
#ifdef DEBUG_MODE
		    LOG_IT("USER logged out.\n");
#endif
		    msg_to_process.Set_FIELDS
		    (
			SUCCESS_LOGOUT,
			empty_vector,
			empty_vector,
			empty_vector
		    );
		    flag_user_active = 1;
		}
			    
	if(flag_user_active == 0)
	{	
#ifdef DEBUG_MODE
	    LOG_IT("USER was not logged in!\n");
#endif
	    msg_to_process.Set_FIELDS
	    (
		ERROR_USER,
		empty_vector,
		empty_vector,
		empty_vector
	    );
	}	
    }
    break;
    
////////////////////////////////////////////////////////////////////////////

case  GET_ACTIVE:
    {
#ifdef DEBUG_MODE
	LOG_IT("GET_ACTIVE message type received\n");
#endif

	std::string accumulator;
	vector<char> answer;
	vector<char> received_message = msg_to_process.Get_MESSAGE();
	int index; index = 0;
	int flag; flag = 0;
	socket->ClearActiveUsersNames();
	socket->ClearActiveUsersStatus();
	
	while(index < received_message.size())
	{
	    accumulator = "\0";
	    flag = 0;
	    
	    while(received_message[index] != '|')
	    	accumulator = accumulator + received_message[index++]; 
		
	    index++;
	    
#ifdef DEBUG_MODE	    
//	    LOG_IT("User to process:" << accumulator << "\n");
#endif

//Store received user name in array
/////////////////////////////////////////////////////////////

	    socket->AddActiveUser(accumulator);
	    
/////////////////////////////////////////////////////////////
	    
	    for(int i = 0; i< active_users_array.size(); i++) //Cycle through array
	    {
		if(accumulator == active_users_array[i])
		    flag = 1;
	    }
	    
	    if(flag == 0)
	    {
		flag = -1;
		for(int i = 0; i< users_array.size(); i++) //Cycle through array
		{
		    if(accumulator == users_array[i].Get_ID_STRING())
			flag = 0;
		}
	    }

	    
	    if(flag == 1)
		{
		    answer.push_back('Y');
		    socket->AddActiveStatus("Y");
		}
		
	    if(flag == -1)
		{
		    answer.push_back('U');
		    socket->AddActiveStatus("N");
		}
		
	    if(flag == 0)
		{
		    answer.push_back('N');
		    socket->AddActiveStatus("N");
		}
	}
	
//Answer now contains YNUYYNUNYN-like string
	    
#ifdef DEBUG_MODE
		    LOG_IT("Answer prepared.\n");
#endif
	    msg_to_process.Set_FIELDS
	    (
		LIST_SENT,
		empty_vector,
		empty_vector,
		answer
	    );
    }
    break;
    
///////////////////////////////////////////////////////////////////////////
    
case FIND:
    {
#ifdef DEBUG_MODE
	LOG_IT("FIND message type received\n");
#endif
	vector<char> answer;
	User user_to_find(msg_to_process.Get_MESSAGE());
	vector<User> found_array;
	int flag_user_resolved = 0;
	int enable_search = 1;
	int users_found = 0;
	
//If ID is known
	if(user_to_find.Get_ID_STRING() != "\0")
	{
		int test = ID_IN_ARRAY(user_to_find.Get_ID());
		if( test != -1)

		{
#ifdef DEBUG_MODE
	    	    LOG_IT("USER found by ID.\n");
#endif		
		    found_array.push_back(users_array[test]);
		}
	    enable_search = 0;
	}
//If ID is not known
	if(enable_search != 0)
	{
#ifdef DEBUG_MODE
	    LOG_IT("Finding USER by Nick...\n");
#endif
//If Nick is known
	    if(user_to_find.Get_NICK() != "\0")
	    {
    		for(int i = 0; i< users_array.size(); i++)
		{
		    if(user_to_find.Get_NICK() == users_array[i].Get_NICK())
		    {
#ifdef DEBUG_MODE
	    		LOG_IT("USER found by NICK.\n");
#endif		
			found_array.push_back(users_array[i]);
			users_found++;
		    }
		}
		if(users_found==0) enable_search = 0;
	    }
	}

//If we can continue searching
	if(enable_search != 0)
	{
#ifdef DEBUG_MODE
	    LOG_IT("Finding USER by Name...\n");
#endif
//If Name is known
	    if((user_to_find.Get_NAME() != "\0") && (users_found >0))
	    {
    		for(int i = 0; i< found_array.size(); i++)
		{
		    if(user_to_find.Get_NAME() != found_array[i].Get_NAME())
		    {
#ifdef DEBUG_MODE
	    		LOG_IT("USER's NAME not found!.\n");
#endif		
			found_array.erase(found_array.begin() +i);
			users_found--;
			i--;
		    }
		}
		if(users_found==0) enable_search = 0;
	    }

//If ID and NICK are not known
	    if((user_to_find.Get_NAME() != "\0") && (users_found ==0) && (enable_search == 1))	    
	    {
    		for(int i = 0; i< users_array.size(); i++)
		{
		    if(user_to_find.Get_NAME() == users_array[i].Get_NAME())
		    {
#ifdef DEBUG_MODE
	    		LOG_IT("USER found by NAME.\n");
#endif		
			found_array.push_back(users_array[i]);
			users_found++;
		    }
		}
		if(users_found==0) enable_search = 0;
	    }
	}
	
//If we can continue searching
	if(enable_search != 0)
	{
#ifdef DEBUG_MODE
	    LOG_IT("Finding USER by Family...\n");
#endif
//If Family is known
	    if((user_to_find.Get_FAMILY() != "\0") && (users_found >0))
	    {
    		for(int i = 0; i< found_array.size(); i++)
		{
		    if(user_to_find.Get_FAMILY() != found_array[i].Get_FAMILY())
		    {
#ifdef DEBUG_MODE
	    		LOG_IT("USER's FAMILY not found!.\n");
#endif		
			found_array.erase(found_array.begin() +i);
			users_found--;
		    }
		}
		if(users_found==0) enable_search = 0;
	    }

//If ID, NICK and NAME are not known
	    if((user_to_find.Get_FAMILY() != "\0") && (users_found ==0) && (enable_search == 1))	    
	    {
    		for(int i = 0; i< users_array.size(); i++)
		{
		    if(user_to_find.Get_FAMILY() == users_array[i].Get_FAMILY())
		    {
#ifdef DEBUG_MODE
	    		LOG_IT("USER found by FAMILY.\n");
#endif		
			found_array.push_back(users_array[i]);
			users_found++;
		    }
		}
		if(users_found==0) enable_search = 0;
	    }
	}

//If we can continue searching
/////////////////////////////////////////////////
//Finding USERs Activity and remove user`s password

#ifdef DEBUG_MODE
	    LOG_IT("Finding USERs Activity...\n");
#endif	
	    for(int i = 0; i<found_array.size(); i++)
	    {
		found_array[i].Set_FATHER("N");
		found_array[i].Set_PASSWORD(SOME_TEXT);
		for(int j = 0; j<active_users_array.size(); j++)
		{
		    if(found_array[i].Get_ID_STRING() == active_users_array[j])
		    {
			found_array[i].Set_FATHER("Y");			
		    }
		
		}
	    }
	    
//Found users are in found_array now.
//Prepare answer
	for(int i = 0; i<found_array.size(); i++)
	{
	    vector<char> c = found_array[i].PrepareRaw();
	    answer.insert(answer.end(), c.begin(), c.end());
	    answer.push_back('|');	    
	}

		msg_to_process.Set_FIELDS
		(
		    SEARCH_SENT,
	    	    empty_vector,
		    empty_vector,
		    answer
		);
	    
    }
    break;    
    
default:
    {
	    msg_to_process.Set_FIELDS
	    (
		ERROR_REQUEST,
		empty_vector,
		empty_vector,
		empty_vector
	    );		
    }
    break;    

}

