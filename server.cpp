// Header files for Server
#include<iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>
#include <thread>
#include<mutex>

// This application will use port 8080
#define PORT 8080

using namespace std;

// client class 
class client
{
  public:
    int index;
    int ID;
    struct sockaddr_in clientAddress;
    socklen_t len;  
};

// clients and threads
client clients[5];
thread threads[5];
int counter = 0;

// connection function, allows connection to server
void connection(client);
void connection(client client_)
{
    // index of the client currently working with
    int ind = client_.index;

    // socket ID of current client
    int sockID = client_.ID;

    // Message to indicate the client is connected to the server
    cout << "Client " << ind + 1 << " connected" << endl;
    char greeting[] = "From server: You are connected to the server. \nOptions:\n1. To check which clients are available, type 'ONLINE'\n2. To send a message, type 'SEND'\n3. To leave the chat, type 'BYE'";
    send(clients[ind].ID, greeting, sizeof(greeting), 0); 
   
   
    // lock to provide synchronization
    mutex m;
    lock_guard<mutex> guard(m);

    // while loop, allow client to send messages to each other, display online servers, or quit
    while(true)
    {
    
        char command[1024];
        char dest_cli[1024];
        
        // receive the initial message
        int r = recv(sockID, command, 1024, 0);
        command[r] = '\0';
       
        // if client wishes to see which clients are online
        if(strcmp(command, "ONLINE") == 0)
        {
                string s = "From server: can connect to\n";
                for(int i = 0; i < counter; i++)
                {
                   
                    if(i != ind && clients[i].ID != 0)
                    {
                        string add = "Client ";
                        int sum = i + 1;
                        add += to_string(sum);                       
                        s+=add;
                        s+= "\n";
                    }
                }
                send(sockID, s.c_str(), s.length(), 0);
                continue;

        }

        // If client wishes to send a message to another client
        if(strcmp(command, "SEND") == 0)
        {
            // client will send another input, which client this message will be sent to
          	  char which[] = "From server: To which client?";
              send(sockID, which, sizeof(which), 0);

              r = recv(sockID, dest_cli, 1024, 0);
            
            dest_cli[r] = '\0';
            const char *a = dest_cli; 
            string s(a);

            int destination = atoi(dest_cli) - 1;
            
            // now client will enter message to send
            char enter_message[] = "From server: Please enter the message ";
            send(sockID, enter_message, sizeof(enter_message), 0);
            r = recv(sockID, command, 1024, 0);
            command[r] = '\0';

            int srcCli = ind + 1;
            string src = "From client " + to_string(srcCli) + ": "; 
            const char *b = command;
            string c(b);
            src+=c;

            send(clients[destination].ID, src.c_str(), src.length() + 1, 0);
            char sent[] = "From server: Sent";
            send(clients[ind].ID, sent, sizeof(sent), 0);

           continue;

        }

        // If the client wishes to quit
        if(strcmp(command, "BYE") == 0)
        {
            cout << "Client " << ind + 1 << " disconnected" << endl;
            // Disconnect, replace the previous client with a new one
            client tmp;
            tmp.ID = 0;
            clients[ind] = tmp;
            send(sockID, "STOP", sizeof("STOP"), 0);
        }
    }
}

int main(int argc, char const *argv[])
{
    // can hold up to 5 clients
    int max = 5;
    int client_socket[max];

    int server_fd;
    struct sockaddr_in address;
   
    // Creating socket file descriptor
    server_fd = socket(PF_INET, SOCK_STREAM, 0);
       
    address.sin_family = PF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // have socket bind with the address   
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    
    // allows max of 5 clients
    listen(server_fd, 5);
    

    struct sockaddr_in cliAddr;
   
    
    // Initialize the threads and allow connection of each client
    while(true)
    {
       
        clients[counter].ID = accept(server_fd, (struct sockaddr *) &clients[counter].clientAddress, &clients[counter].len);
        clients[counter].index = counter;
        threads[counter] = thread(connection, clients[counter]);
        counter++;
    }

    // join the threads
    for(int i = 0; i < counter; i++)
        threads[i].join();
    
    close(server_fd);
    return 0;
}

