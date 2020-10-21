//Function look for given ID in users_array
long ID_IN_ARRAY(long id)
{
    int size = users_array.size();
    for(long i = 0; i< size; i++)
        if(id == users_array[i].Get_ID())
	    return i;
	    
    return -1;
}

int PASS_IN_ARRAY(std::string pass, long i)
{
        if(pass == users_array[i].Get_PASSWORD())
	    return 1;
	    
    return 0;
}

/*const char* MAKE_PATH(std::string file_path)
{
    std::string full_path = BIN_PATH + file_path;
    return full_path.c_str();
}*/


