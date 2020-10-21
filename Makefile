# Makefile for the Kaska server
#
CC=/usr/bin/g++
FLAGS=-pthread -static
simple_server_objects = global.o Socket.o ServerSocket.o Message.o User.o Semaphore.o simple_server_main.o


kaskad: $(simple_server_objects)
	$(CC) $(simple_server_objects) -o kaskad $(FLAGS)
global.o: global.cpp global.h
	$(CC) -c $(FLAGS) global.cpp
Socket.o: Socket.cpp Socket.h
	$(CC) -c $(FLAGS) Socket.cpp
Message.o: Message.cpp Message.h definitions.h debug.cpp
	$(CC) -c $(FLAGS) Message.cpp
User.o: User.cpp User.h definitions.h debug.cpp
	$(CC) -c $(FLAGS) User.cpp
ServerSocket.o: ServerSocket.cpp ServerSocket.h
	$(CC) -c $(FLAGS) ServerSocket.cpp
Semaphore.o: Semaphore.cpp Semaphore.h
	$(CC) -c $(FLAGS) Semaphore.cpp
simple_server_main.o: simple_server_main.cpp definitions.h processing.cpp run_client.cpp file_save.cpp file_load.cpp debug.cpp idinarray.cpp
	$(CC) -c $(FLAGS) simple_server_main.cpp
clean:
	rm -f *.o simple_server