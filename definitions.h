#ifndef _definitions_include_file_
#define _definitions_include_file_

#define PORT 30000
#define BORDER '|'
#define USER_BORDER '&'
#define SOME_TEXT "X"
#define BIN_PATH "/src/server/2.5/"

//Request Format: <Operation code><SIZE>|<FROM_login>|<TO_login>|<MESSAGE>

//------------Requests----------------
//Send message to server
#define SEND_MESSAGE 'S'
//S29|87564&Password&&&&|54327|Message

//Send file to server
#define SEND_FILE 'Z'
//Z29|87564&Password&&&&|54327|file


#define REGISTER_USER 'R'
//R36|||&Password&Nick&Name&Family&Father

#define DELETE_USER 'D'
//D18|67453&Password&&&&||

//Take message from server
#define TAKE_MESSAGE 'T'
//T17|86754&Password&&&&||

//Get users activity status from server
#define GET_ACTIVE 'A'
//A21|||54623|13245|87657|

//Login
#define LOGIN 'L'
//L17|46534&Password&&&&|&&&&&|

//Logout
#define LOGOUT 'O'
//O17|46534&Password&&&&|&&&&&|

//Find user in users database
#define FIND 'F'
//F33|||45286&&Nick&Name&Family&Father

//-------------------------------------


//Answer format: <Operation code><SIZE>|<FROM>|<TO>|<MESSAGE>

//------------Answers--------------------------

//New user registered successfully
#define SUCCESS_REGISTRATION 'L'
//L3|||

//New user logged in successfully
#define SUCCESS_LOGIN 'V'
//V3|||

//User logged out successfully
#define SUCCESS_LOGOUT 'G'
//G3|||

//Message successfully received by server
#define SUCCESS_RECEIVED 'R'
//R3|||

//User successfully deleted
#define SUCCESS_DELETE 'D'
//D3|||

//Message successfully sent by server
#define SUCCESS_SENT 'S'
//S22|Igoran|Alexey|Message

//No such user on server
#define ERROR_USER 'U'
//U3|||

//New user registration failed
#define ERROR_REGISTRATION 'F'
//F3|||

//User login failed
#define ERROR_LOGIN 'W'
//W3|||


//No messages for user
#define ERROR_NOMESSAGE 'N'
//N3|||

//Active users list sent
#define LIST_SENT 'I'
//I10|||YYNYNNY

//User search results
#define SEARCH_SENT 'X'
//X4|||Y
//X4|||N

//Error in request format or unable to receive full message
#define ERROR_REQUEST 'Q'
//Q3|||

//Timeout expired while waiting for next portion of message
#define ERROR_TIMEOUT 'T'
//T3|||


//-----------------------------------------------

#endif
