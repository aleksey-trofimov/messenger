
//Load current arrays from files

int clients_descriptor;
int messages_descriptor;
int log_descriptor;
clients_descriptor = open(MAKE_PATH("clients"), O_RDONLY);
if( clients_descriptor == -1 )
{
    if(open(MAKE_PATH("clients"), O_RDONLY | O_CREAT))
	{
	    chmod(MAKE_PATH("clients"), S_IRWXU);// | S_IWOTH | S_IXOTH));
	    LOG_IT("No clients file present. New created.");    
	}
    else 
	LOG_IT("Unable to create clients file!\n");

}

messages_descriptor = open(MAKE_PATH("messages"), O_RDONLY);
if( messages_descriptor == -1 )
{
    if(open(MAKE_PATH("messages"), O_RDONLY | O_CREAT))
	{
	    chmod(MAKE_PATH("messages"), S_IRWXU);
	    LOG_IT("No messages file present. New created.\n");
	}
    else 
	LOG_IT("Unable to create messages file!\n");
}

char c = '\0'; 
vector<char> accumulator;
int count = 0;


//////////////////////////////////////////////////
while(read(clients_descriptor, &c, 1) > 0)
{
    count = 0;
    
    while( count != 6 )
    {
	accumulator.push_back( c );
	read(clients_descriptor, &c, 1);
	if(c == '&') count++;
    }
    User loaded_message(accumulator);
    users_array.push_back(loaded_message);
    accumulator.erase(accumulator.begin(), accumulator.end());

}
/////////////////////////////////////////////////

std::string id;
while(read(messages_descriptor, &c, 1) > 0)
{
    count = 0;
    
    while( count != 1 )
    {
	id += c;
	read(messages_descriptor, &c, 1);
	if(c == '|') count++;
    }
    messages_array.push_back(id);
    id.erase(id.begin(), id.end());
}


close(clients_descriptor);
close(messages_descriptor);

//Create directory for mesage bases
    mkdir(MAKE_PATH("msgbases"), S_IRWXU|S_IRWXG|S_IRWXO);
