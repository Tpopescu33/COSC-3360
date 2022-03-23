// code template provided by Dr. Rincon from Black board.

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
#include <sys/wait.h>


//fireman function to wait
void fireman(int)
{
   while (waitpid(-1, NULL, WNOHANG) > 0)
       wait(0);
}

//function to convert to binary
std::string toBinary(int n, int numBits)
{
    std::string r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    
    for (int i = r.size(); i < numBits; i++){
        r.insert(0,"0");
    }
    return r;
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){

    /// get Input

    std::string temp;
    std::string temp1;
    std::string temp3;
    std::string temp4;
    std::stringstream tempString;

    int temp2 = 0;
    int symbols;
   
    //get number of symbols
    getline(std::cin, temp4);
    
    tempString << temp4;
    
    tempString >> symbols;


    
    

   

    std::vector<std::string> character;
    std::vector<int> value;
    std::vector<std::string> binaryValue;

   

    // get rest of input
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

    int opt = 1;
    int maxValue = 0;
    int numBits = 0;
    
    //get max value
    for(int i = 0; i< symbols; i++){
        if (maxValue<value[i]){
            maxValue=value[i];
        }
    }
    // calculate num bits
    numBits = ceil(log2(maxValue+1));
       
    
    std::stringstream numBitTemp;
   std::string numBitChar;
   numBitTemp << numBits;
   numBitChar = numBitTemp.str();
        
    //change values to binary so we can compare later.
    
    for(int i = 0; i< symbols; i++){
        
     binaryValue.push_back(toBinary(value.at(i), numBits));
    }

    //set up socket, this is a modified version of what Dr. Rincon provided.
    int sockfd, newsockfd, portno, clilen, pid;
     char buffer[256];

    

     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     //create listening socket
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR, (char *)&opt, sizeof(opt))<0){
        error("error setsockopt");
    };
    
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd, 5);
     clilen = sizeof(cli_addr);
    
    //call fireman before infinite loop
    signal(SIGCHLD, fireman);
    
   //infinite loop
    while (1) {
        //create new socket to handle new connection so other socket can continue listening
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
            if (newsockfd<0){
                error("ERROR on Accpet");
                 }

                 //Fork
            if (fork() == 0){
                //close listening socket if child
                close(sockfd);
      
                bzero(buffer,256);
                n = read(newsockfd,buffer,255);
                if (n < 0) error("ERROR reading from socket");
                    //if we read a 5 we know its a request for the number of bits, if its not 5 its wanting the binary to be decoded.
                if (buffer[0] == 5){
                        //send num of bits back
                    buffer[0] = numBitChar.at(0);
                    n = write(newsockfd, buffer,strlen(buffer));
                    } else {
                        //request for character
                        std::string tempBinary;

                        //match buffer to binary values and send char back.
                        for (int i=0; i<symbols; i++){
                            if (buffer == binaryValue[i]){
                                bzero(buffer,255);
                                buffer[0] = character[i].at(0);
                                n= write(newsockfd,buffer,255);
                                if (n < 0) error("ERROR writing to socket");
                                }
                            }    
                bzero(buffer,256);
     
     
                }
                //This is the parent, we close the socket and exit.
            close(newsockfd);
            _exit(0);
     
    
    }
    
    close(newsockfd);
   

    }
     return 0; 
     
}