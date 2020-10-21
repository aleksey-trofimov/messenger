
//Save current arrays to files

int clients_descriptor;
clients_descriptor = open(MAKE_PATH("clients"), O_WRONLY|O_CREAT|O_TRUNC);

int messages_descriptor;
messages_descriptor = open(MAKE_PATH("messages"), O_WRONLY|O_CREAT|O_TRUNC);

for(int i = 0; i<users_array.size(); i++)
{
    vector<char> data = users_array[i].PrepareRaw();
    
    write(clients_descriptor,  &data[0], data.size());
    write(clients_descriptor, "&", 1);    
}


for(int i = 0; i<messages_array.size(); i++)
{
//    vector<char> data = messages_array[i].PrepareRaw();
        
    write(messages_descriptor, &messages_array[i][0], messages_array[i].size());	
    write(messages_descriptor, "|", 1);    
}
close(clients_descriptor);
close(messages_descriptor);
