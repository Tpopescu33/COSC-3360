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

// struct sockaddr_in serv_addr;
// struct hostent *server;

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

void * decode(void *x_void_ptr)
{
    
	struct info *x_ptr = (struct info *)x_void_ptr;
   

    // std::cout<<"argc: "<<x_ptr->argc<<", argv0: "<<x_ptr->argv[0]<<", argv2: "<<x_ptr->argv[2]<<std::endl;
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
   

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
    
// std::cout<<"argc: "<<x_ptr->argc<<", argv0: "<<argv0<<", argv2: "<<argv2<<std::endl;
    
    if (x_ptr -> argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv0);
       exit(0);
    }
    portno = atoi(argv2);
     
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    // std::cout<<"sockfd: "<<sockfd<<"\n";
    server = x_ptr->server;
    
    //std::cout<<"address: "<<server<<"\n";
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such h1ost\n");
        exit(0);
    }
    //std::cout<<"address: "<<server<<"\n";
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    // printf("connected\n");
    bzero(buffer,256);

    for (int i = 0; i < sizeof(x_ptr->binaryValues); i++){
        buffer[i] = x_ptr->binaryValues[i];
    }
    // // printf("Here is the message: %s\n",buffer);

    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
        error("ERROR reading to socket");
    bzero(buffer,256);

     n = read(sockfd, buffer, 255);
    //   printf("Here is the message: %s\n",buffer);
    x_ptr->answers[0] = buffer[0];
    if (n < 0) 
        error("ERROR writing to socket");
    bzero(buffer,256);
    close(sockfd);
   
    // n = read(sockfd1,buffer,255);
    // if (n < 0) 
   
    //      std::cout<<"ERROR reading from socket";
   
	// x_ptr->answers = buffer[0];
	return NULL;
}





int main(int argc, char *argv[])
{   
    int numChars;
    int numBits;
    std::string compMsg;
    getline(std::cin, compMsg);

  //  std::cout <<"coded message " << compMsg<< std::endl;


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
    //std::cout<<"address: "<<server<<"\n";
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
//     char tempBuffer[sizeof(buffer)];
    
//     for (int i = 0 ; i< sizeof(buffer); i++){
//     tempBuffer[i] = buffer[i];
    
//     }
//    numBits = (int) tempBuffer;
    
   // printf("number of numBits: %d\n",numBits);
    numChars = compMsg.length()/numBits;
   // printf("number of numChars: %d\n",numChars);
   // printf("comp MSG: %d\n",compMsg.length());
    std::vector<info> first(numChars);


    for (int i = 0; i < numChars; i++){
        for (int j = 0; j < numBits; j++){
            first[i].binaryValues[j] = compMsg.at((i*numBits)+j);
           // std::cout<<first[i].binaryValues[j];
        }
        first[i].argc = argc;
        first[i].server = server;
        first[i].serv_addr = serv_addr;
        for (int k = 0; k < 3; k++){
            first[i].argv.push_back(argv[k]);

        }
      //  std::cout<<"\n";
}

    

   // std::cout<<"test\n";
    // bzero(buffer,256);
    // for (int i = 0; i < 3; i++){
    //     buffer[i] = first[0].binaryValues[i];
    // }
    
    //   n = write(sockfd, buffer, 255);
    //   n=read(sockfd, buffer, 255);
    //   printf("Here is the message: %s\n",buffer);

    //  if (n < 0) 
    //      error("ERROR reading from socket");
    // bzero(buffer,256);
    // for (int i = 0; i < 3; i++){
    //     buffer[i] = first[1].binaryValues[i];
    // }
    
    //   n = write(sockfd, buffer, 255);
    //    n=read(sockfd, buffer, 255);
    //   printf("Here is the message: %s\n",buffer);
    //  if (n < 0) 
    //      error("ERROR reading from socket");
    //       bzero(buffer,256);
    // for (int i = 0; i < 3; i++){
    //     buffer[i] = first[2].binaryValues[i];
    // }
    
    //   n = write(sockfd, buffer, 255);
    //    n=read(sockfd, buffer, 255);
    //   printf("Here is the message: %s\n",buffer);
    //  if (n < 0) 
    //      error("ERROR reading from socket");
    //       bzero(buffer,256);
    // for (int i = 0; i < 3; i++){
    //     buffer[i] = first[3].binaryValues[i];
    // }
    
    //   n = write(sockfd, buffer, 255);
    //    n=read(sockfd, buffer, 255);
    //   printf("Here is the message: %s\n",buffer);
    //  if (n < 0) 
    //      error("ERROR reading from socket");
    // close(sockfd);
    // std::cout<<argv[0]<<std::endl;

    // std::cout<<"test: "<<first[1].binaryValues[0]<<first[1].binaryValues[1]<<first[1].binaryValues[2]<<std::endl;
    // std::cout<<"argc: "<<first[0].argc<<", argv0: "<<first[0].argv[0]<<", argv2: "<<first[0].argv[2]<<std::endl;
    // std::cout<<"argc: "<<first[1].argc<<", argv0: "<<first[1].argv[0]<<", argv2: "<<first[1].argv[2]<<std::endl;
    // std::cout<<"argc: "<<first[2].argc<<", argv0: "<<first[2].argv[0]<<", argv2: "<<first[2].argv[2]<<std::endl;
    // std::cout<<"argc: "<<first[3].argc<<", argv0: "<<first[3].argv[0]<<", argv2: "<<first[3].argv[2]<<std::endl;
    // std::cout<<"argc: "<<first[4].argc<<", argv0: "<<first[4].argv[0]<<", argv2: "<<first[4].argv[2]<<std::endl;

    std::vector<pthread_t> tid(numChars);

    for(int i = 0; i<numChars;i++){
        if(pthread_create(&tid[i], NULL, decode, &first[i]))
        {
			fprintf(stderr, "Error creating thread\n");
			return 1;

		}	
    }
    for (int i = 0; i < numChars; i++)
        	pthread_join(tid[i], NULL);


    std::cout<<"Decompressed message: ";

    for (int i = 0; i < numChars; i++) {
        std::cout<<first[i].answers[0];
    }
    std::cout<<std::endl;
    

    return 0;
}
