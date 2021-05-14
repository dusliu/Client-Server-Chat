# Client-Server-Chat
To use this application, open a Linux/UNIX terminal like Ubuntu or MobaXterm.

Compile this files by doing 

for server
g++ -std=c++11 -pthread server.cpp -o server

and for the client
g++ -std=c++11 -pthread client.cpp -o client

Open up to 5 terminals (max)

Run the server file by:
./server

Run clients by:
./client 8080 
where 8080 is the port number that this application runs on. If not 8080, the program will send an error message.

When clients are connected to the server, input options are displayed to each terminal showing the types of messages.
	ONLINE - check all of the servers that the current client can send messages to
	SEND - send a message. First, enter which client to send the message to followed by the actual message.
	Ex: SEND <enter>
	    1 <enter>
            Hello from client... <enter>
	BYE - the client will exit the server
