// Header files for client
#include<iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include<thread>


using namespace std;

// Function for the client. Receives messages from another client via the Server
void connRecv (int sID)
{
    int cliSock = sID;
    while(true)
    {
        char data[1024];
        int quan = recv(cliSock, data, 1024, 0);
        data[quan - 1] = '\0';
        if(strcmp(data, "STOP") == 0)
            exit(0);
        cout << data << endl;
    }
}


int main(int argc, char *argv[])
{
    int pn = atoi(argv[1]);
    if(pn != 8080)
    {
        cerr << "Invalid port number for this application" << endl;
        exit(0);
    }
    // set up connection to the server
    int sock;
    struct sockaddr_in serv_addr;
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    
    
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_port = htons(pn);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
   
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	
    // need thread because multiple clients will exist
    thread t(connRecv, sock);
    char message[1024];
    
	while(true)
	{
        // Inside here, input data to server;
        cin.getline(message, 1023, '\n');
        message[strlen(message)] = '\0';
        send(sock, message, sizeof(message), 0);
               
        
	}
    
    close(sock);
    return 0;
}

