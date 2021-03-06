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

// struct to hold info to pass to threads

struct info {
    std::string binaryValues;
    std::string answers;
    int argc; 
    std::vector<std::string> argv;
    struct hostent *server;
    struct sockaddr_in serv_addr;
};


void error(char *msg)
{
    perror(msg);
    exit(0);
}

//thread function

void * decode(void *x_void_ptr)
{
    // pointer stuff to get info passed
	struct info *x_ptr = (struct info *)x_void_ptr;
   

   
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
   
    //create char arrays to hold values
    char buffer[256];

    char argv0[x_ptr->argv[0].length()];
    for (int i = 0; i< x_ptr->argv[0].length(); i++){
        argv0[i] = x_ptr->argv[0].at(i);
    }
    char argv1[x_ptr->argv[1].length()];
    for (int i = 0; i< x_ptr->argv[1].length(); i++){
        argv1[i] = x_ptr->argv[1].at(i);
    }
    char argv2[x_ptr->argv[2].length()];
    for (int i = 0; i< x_ptr->argv[2].length(); i++){
        argv2[i] = x_ptr->argv[2].at(i);
    }
    

    
    if (x_ptr -> argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv0);
       exit(0);
    }
    //socket stuff, copied template from Dr. Rincon and made changes
    portno = atoi(argv2);
     
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    //here I had to send the original server from the main function in order for it to work on moodle.
    //if i call gethostbyname again i get a no such host error on moodle. (works fine in VSC)
    server = x_ptr->server;
    
    
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
    
    bzero(buffer,256);

    //load binary values into buffer
    for (int i = 0; i < sizeof(x_ptr->binaryValues); i++){
        buffer[i] = x_ptr->binaryValues[i];
    }
   
    //send buffer to server
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
        error("ERROR reading to socket");
    bzero(buffer,256);

    //get buffer back from server
    n = read(sockfd, buffer, 255);
    //copy buffer to answers
    x_ptr->answers[0] = buffer[0];
    if (n < 0) 
        error("ERROR writing to socket");
    bzero(buffer,256);
    close(sockfd);
   
    
	return NULL;
}





int main(int argc, char *argv[])
{   
    int numChars;
    int numBits;

    //get input
    std::string compMsg;
    getline(std::cin, compMsg);

  
    //set up socket, again most of the code is from Dr. Rincon.

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
    
    bzero(buffer,256);
    
    //first we send the number 5 to the server, if the server gets a 5 it'll send back the number of bits. if it doesn't get a 5 itll decode the message and send back the character.
    buffer[0] = 5;
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
        error("ERROR writing to socket");
    bzero(buffer,256);
   
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    
 close(sockfd);

    numBits = std::strtol(buffer, nullptr, 10);
    numChars = compMsg.length()/numBits;   

    //create and populate struct to send to threads
    std::vector<info> first(numChars);
    
    for (int i = 0; i < numChars; i++){
        for (int j = 0; j < numBits; j++){
            first[i].binaryValues[j] = compMsg.at((i*numBits)+j);
          
        }
        first[i].argc = argc;
        first[i].server = server;
        first[i].serv_addr = serv_addr;

        for (int k = 0; k < 3; k++){
            first[i].argv.push_back(argv[k]);

        }
     
}

    //create threads

    std::vector<pthread_t> tid(numChars);

    for(int i = 0; i<numChars;i++){
        if(pthread_create(&tid[i], NULL, decode, &first[i]))
        {
			fprintf(stderr, "Error creating thread\n");
			return 1;

		}	
    }

    //join to close threads

    for (int i = 0; i < numChars; i++)
        	pthread_join(tid[i], NULL);


    std::cout<<"Decompressed message: ";

    for (int i = 0; i < numChars; i++) {
        std::cout<<first[i].answers[0];
    }
    std::cout<<std::endl;
    

    return 0;
}
