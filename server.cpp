#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <pthread.h>
#include <sstream>


void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){

    std::cout<<"server is running"<<std::endl;

    std::string temp;
    std::string temp1;
    std::string temp3;
    std::string temp4;
    std::stringstream tempString;

    int temp2 = 0;
    int symbols;
   

    getline(std::cin, temp4);
    
    tempString << temp4;
    
    tempString >> symbols;


    
    

   

    std::vector<std::string> character;
    std::vector<int> value;

   

  
    for(int i = 0; i< symbols; i++){
        getline(std::cin, temp);
        
        temp3 = temp[2];
        temp3 +=temp[3];
        temp1 = temp[0];
        std::stringstream tempString1;
        tempString1 << temp3;
        tempString1>>temp2;
        character.push_back(temp1);
        value.push_back(temp2);
        
       
        
        
    }

  
     int maxValue = 0;
     int numBits = 0;
    for(int i = 0; i< symbols; i++){
        if (maxValue<value[i]){
            maxValue=value[i];
        }
    }
     for (int i = 0; i < symbols; i++){
    numBits = ceil(log2(maxValue+1));
    }
    std::stringstream numBitTemp;
   std::string numBitChar;
   numBitTemp << numBits;
   numBitChar = numBitTemp.str();
        

    for(int i = 0; i< symbols; i++){
        std::cout<< character.at(i) << " " << value.at(i) << "\n";


    }
    

    int sockfd, newsockfd, portno, clilen, pid;
     char buffer[256];

    

     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
     bzero(buffer,256);
     buffer[0] = numBitChar.at(0);
     std::cout<<"first"<<std::endl;
     n = write(newsockfd, buffer,strlen(buffer));
     if (n < 0) error("ERROR writing to socket");

     while (1) {
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

    pid = fork();
    if(pid < 0){
        error("ERROR on fork");
    }
    if (pid == 0){
     close(sockfd);
     bzero(buffer,256);
    //  n = read(newsockfd,buffer,255);
    //  if (n < 0) error("ERROR reading from socket");
    //  printf("Here is the message: %s\n",buffer);
     bzero(buffer,256);
     buffer[0] = numBitChar.at(0);
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd, buffer,strlen(buffer));
     if (n < 0) error("ERROR writing to socket");
     _exit(0);
    }
    else close(newsockfd);
     }
    

     return 0; 
     
}