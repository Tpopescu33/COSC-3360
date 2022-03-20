//// I used Dr. Rincon's code from blackboard as a starting template

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <pthread.h>
#include <sstream>


struct info {
    std::string binaryValues;
    std::string answers;

};


void error(char *msg)
{
    perror(msg);
    exit(0);
}



int main(int argc, char *argv[])
{   
    int numChars;
    int numBits;
    std::string compMsg;
    std::cin >> compMsg;

    std::cout <<"coded message " << compMsg<< std::endl;


    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    // printf("Please enter the message: ");
    bzero(buffer,256);
    // fgets(buffer,255,stdin);
    // n = write(sockfd,buffer,strlen(buffer));
    // if (n < 0) 
    //      error("ERROR writing to socket");
    // bzero(buffer,256);
   
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");

    std::stringstream tempString1;
    tempString1 << buffer[0];
    tempString1 >> numBits;
    
    printf("number of symbols: %d\n",numBits);
    numChars = compMsg.length()/numBits;
    std::vector<info> first(numChars);

    for(int i = 0;  i < compMsg.length(); i += numBits){
        for (int j = 0; j < numBits; j++){
            first[i].binaryValues[j] = compMsg.at(i+j);
            std::cout<<first[i].binaryValues[j];
        }
        std::cout<<" ";
    }
    
    
    

    return 0;
}
