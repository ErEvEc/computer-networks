//
//  main.cpp
//  client_lab2
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
#include <string>

#define PORT 3780
using namespe std;

string generateId(){


    //return std::to_string(rand()%256);
    stringstream ss;
    ss << rand()%256;
    return ss.str();
}

int main(int argc, char const *argv[])
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    srand(time(NULL));
    string id0;
    id0 = generateId();
    strcpy(buffer, id0.c_str());
    if (send(sock, buffer, 1023, 0) > 0) {
        cout << id0 << ": Discovery" << endl;
    }
    sleep(1);

    string newAddr;
    if (recv(sock, buffer, 1023, 0) > 0) {
        string msg = string(buffer);
        int seperator = msg.find(':');
        string id1 = msg.substr(0, seperator);
        newAddr = msg.substr(seperator+1);
        cout << id1 << ": Received address:" << newAddr << endl;
    }
    
    
    string id2 = generateId();
    strcpy(buffer, id2.c_str());
    if (send(sock, buffer, 1023, 0) > 0) {
        cout << id2 << ": Request: " << newAddr << endl;
    }
    sleep(1);
    
    if (recv(sock, buffer, 1023, 0) > 0) {
        cout << buffer << ": Completed" << endl;
    }
    close(sock);
    
    return 0;
}
