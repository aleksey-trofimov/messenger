
//Client thread

void* run_client(void* new_sock)
{
    ServerSocket* socket = reinterpret_cast<ServerSocket*>(new_sock);    
    socket->set_non_blocking ( 1 );
    
    int report;
    int size;
    int check = 0;
    
//To sleep < 1 second
    struct timeval st_delay;

    LOG_IT("Thread started!\n");    
    
//Trying to receive client message
    while(1)
    {
	vector<char> data;
	report = 0;
	check++;
	
	//Try to see if there is anything in socket
	    try
	    {
		report = socket->recv( data );
	    //Return values:	-1 No message arrived
	    //			0  Error 
	    //			>0 Success
	    }
    	    catch(SocketException& re)
	    {
		LOG_IT("Exception was caught! Exiting from thread.\n");
		LOG_IT("Logout forced!\n");
		processing_sema.lock();
		socket->logout(&active_users_array);
		processing_sema.unlock();
		delete socket;
		return(0);
	    }
	    
	    if(report == 0)
	    {
		LOG_IT("recv() report == 0! Logout forced!\n");
		processing_sema.lock();
		socket->logout(&active_users_array);
		processing_sema.unlock();
		delete socket;
		return(0);	    
	    }
		    
//Client message processing/////////////////////////////////

	if(report > 0) //Some data was received
	{
	    check = 0;
    	    Message msg_to_process(data);
//////////////////////////Was it a long message?

	    size =	msg_to_process.Get_SIZE() -
			msg_to_process.Get_FROM().size() -
			msg_to_process.Get_TO().size() -3;
	
	    if(((msg_to_process.Get_MESSAGE()).size() < size) && (msg_to_process.Get_CODE() != ERROR_REQUEST))
		while((msg_to_process.Get_MESSAGE()).size() != size)
		{
	    	    data.erase(data.begin(), data.end());
	    	    fd_set read_set;
	    	    FD_ZERO( &read_set );
		    FD_SET ( socket->Get_SOCK(), &read_set );
	    	    st_delay.tv_sec = 100;
		    
	    	    if(select(socket->Get_SOCK()+1, &read_set, NULL, NULL, &st_delay) == 0)
	    	    {
	        	LOG_IT("Timeout expired! recv() terminated, connencion closed.\nExiting from thread.\n");
	        	Message error(ERROR_TIMEOUT,empty_vector,empty_vector,empty_vector);
			try
			{
			    socket->send(error.PrepareRaw());
			}
			catch(SocketException& re)
			{
		    	    LOG_IT("Exception was caught! Exiting from thread.\n");
		    	    LOG_IT("Logout forced!\n");
		    	    processing_sema.lock();
		    	    socket->logout(&active_users_array);
		    	    processing_sema.unlock();
		    	    delete socket;
		    	    return(0);
			}		    
			LOG_IT("Logout forced!\n");
		    	processing_sema.lock();
		    	socket->logout(&active_users_array);
		    	processing_sema.unlock();
		    	delete socket;
		    	return(0);
		    }
		
		    try
		    {
			socket->recv( data );
		    }
		    catch(SocketException& re)
		    {
			LOG_IT("Exception was caught. Exiting from thread.\n");
			LOG_IT("Logout forced!\n");
			processing_sema.lock();
			socket->logout(&active_users_array);
			processing_sema.unlock();
			delete socket;
			return(0);
		    }
		    msg_to_process.Add_TO_MESSAGE(data);		    
		}	
	
///////////////CHECK IF WE HAVE FILE TRANSFER MESSAGE///////////////////
//Change code to message transfer code to save resources
	    int check = 0;
	    if(msg_to_process.Get_CODE() == SEND_FILE)
	    {
	        check = 1;
	        msg_to_process.Set_CODE(SEND_MESSAGE);
	    }
///////////////////////////////////////////////////////////////////////

	    processing_sema.lock();
	    #include "processing.cpp"
	    processing_sema.unlock();
			
//Trying to send answer/////////////////////////////////////	
		
#ifdef DEBUG_MODE
	    LOG_IT("Trying to send message..\n");
#endif	
	    try
	    {	
		socket->send( msg_to_process.PrepareRaw() );
	    }
	    catch(SocketException& re)
	    {
		LOG_IT("Exception was caught! Exiting from thread.\n");
		LOG_IT("Logout forced!\n");
		processing_sema.lock();
		socket->logout(&active_users_array);
		processing_sema.unlock();
		delete socket;
		return(0);
	    }
		    
///TODO: Save if message was of specific type ONLY!!!!!!
	    processing_sema.lock();
	    #include "file_save.cpp"
	    processing_sema.unlock();
	    data.erase(data.begin(), data.end());
	
	}
	
//////////////RECEIVING & PROCESSING COMPLETED!///////////////
/////TRYING TO FIND ANY MESSAGE FOR OWNER OF THE THREAD///////
	if(socket->Get_OWNER() != "\0")
	{
	    vector<char> msg;
	    processing_sema.lock();
	    int m = socket->Take_MESSAGE(&messages_array, &msg);
	    processing_sema.unlock();
	    if(m != -1)
	    {
#ifdef DEBUG_MODE
		LOG_IT("Trying to send message from base..\n");
#endif		
		try
		{		
		    socket->send( msg );
		}		
		catch(SocketException& re)
		{
		    LOG_IT("Exception was caught! Exiting from thread.\n");
		    LOG_IT("Logout forced!\n");
		    processing_sema.lock();
		    socket->logout(&active_users_array);
		    processing_sema.unlock();
		    delete socket;
		    return(0);
		}		    
		
//		st_delay.tv_usec = 2000000;		    
//		select(32, NULL, NULL, NULL, &st_delay);
	    
///TODO: Save if message was of specific type ONLY!!!!!!
		processing_sema.lock();
		#include "file_save.cpp"
		processing_sema.unlock();
	    }	    	
	}
	
/////////////////////////////////////////////////////////////////	
///////////////TRYING TO FIND CHANGES IN ACTIVE USERS LIST///////
	int flag;
	
	processing_sema.lock();	
	flag = socket->Compare_LISTS(&active_users_array);
	processing_sema.unlock();
	
	if(flag == -1)
	{
	    Message new_list(LIST_SENT,empty_vector,empty_vector,socket->GetActiveStatusList());
#ifdef DEBUG_MODE
	    LOG_IT("Trying to send updated active users list..\n");
#endif	
	    try
	    {	
	        socket->send(new_list.PrepareRaw());
	    }
	    
	    catch(SocketException& re)
	    {
	        LOG_IT("Exception was caught! Exiting from thread.\n");
	        LOG_IT("Logout forced!\n");
	        processing_sema.lock();
	        socket->logout(&active_users_array);
	        processing_sema.unlock();
	        delete socket;
	        return(0);
	    }
        }
////////////Socket check///////////

	if(check > 3000)
	{
	        LOG_IT("No checking packet from client! Exiting from thread.\n");
	        LOG_IT("Logout forced!\n");
	        processing_sema.lock();
	        socket->logout(&active_users_array);
	        processing_sema.unlock();
	        delete socket;
	        return(0);
	}
	
//Sleep to save resources
	st_delay.tv_usec = 100000; //0.1 sec.
	st_delay.tv_sec = 0; //0.1 sec.
	select(32, NULL, NULL, NULL, &st_delay);
	    
/////////////////////////////////////////////////////////////////

    }	
    LOG_IT("Logout forced!\n");
    processing_sema.lock();
    socket->logout(&active_users_array);
    processing_sema.unlock();
    delete socket;
    return(0);
}
