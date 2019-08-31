//
//  main.cpp
//  server_lab2
//
//  Created by 干宇 on 6/8/19.
//  Copyright © 2019 Carrie. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <iostream>
#include <sstream>

#define PORT 3780
using namespace std;

string generateId(){


    //return std::to_string(rand()%256);
    stringstream ss;
    ss << rand()%256;
    return ss.str();
}


int main(int argc, char const *argv[]) {
    
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) <0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        continue;
    }
    
    srand(time(NULL));
    sleep(1);
    if (recv(new_socket, buffer, 1023, 0) > 0) {
        cout << buffer << ": Received discovery"<< endl;

    }

    string id0 = string(buffer);
    string addr = generateId() + "." + generateId() + "." + generateId() + "." + id0;
    string id1 = generateId();
    string msg = id1 + ":" + addr;
    strcpy(buffer, msg.c_str());
    if (send(new_socket, buffer, 1023, 0) > 0) {
        cout << id1 << ": Offer IP address: " << addr << endl;
    }
    sleep(1);
    if (recv(new_socket, buffer, 1023, 0) > 0) {
        cout << buffer<< ": Received request"<< endl;
    }
    
    string id2 = generateId();
    strcpy(buffer, id2.c_str());
    if (send(new_socket, buffer,1023, 0) > 0) {
        cout << id2 << ": Ack "<< endl;
	close(new_socket);
	break;
    }
    close(server_fd);
    }
    return 0;
}
