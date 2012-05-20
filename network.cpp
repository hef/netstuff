#include "network.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <arpa/inet.h>

network::network() :
    sockfd(-1)
{
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // not ipv6 friendly
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, "3490", &hints, &res);

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, 1024);
    freeaddrinfo(res);
}

network::~network()
{
    for( std::vector<struct sockaddr*>::iterator i = others.begin(); i != others.end(); ++i)
    {
        free(*i);
    }
    close(sockfd);
}

void network::send(const char* message)
{
    int count=0;
    for( std::vector<struct sockaddr*>::iterator i = others.begin(); i != others.end(); ++i)
    {
        std::cout << ++count << std::endl;
        sendto(sockfd, message, strlen(message), 0, *i, sizeof(struct sockaddr_storage));
    }

}

void network::recv()
{
    char buffer[1024];
    struct sockaddr* from = (struct sockaddr*)malloc(sizeof(struct sockaddr));
    socklen_t fromlen = sizeof(*from);
    ssize_t size = recvfrom(sockfd, buffer, 1024, 0, from, &fromlen);
    buffer[size]= '\0'; //null terminate string
    
    bool known = false;
    for( std::vector<struct sockaddr*>::iterator i = others.begin(); i != others.end(); ++i)
    {
        if( 0 == strncmp(from->sa_data,(*i)->sa_data,14) )
        {
            std::cout << "match" << std::endl;
            known = true;
            break;
        }

    }
    if(!known)
    {
        others.push_back(from);
    }
    else
    {
        free(from);
    }
    std::cout << buffer << std::endl;

}

void network::addTarget( const char* target)
{
    struct sockaddr_in* sa = (struct sockaddr_in*)malloc(sizeof(sa));
    inet_pton(AF_INET, target, &sa->sin_addr);
    sa->sin_port = htons(3490);
}



