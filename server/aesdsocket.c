#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdbool.h>
#include<fcntl.h>

int sockfd;
#define PORT "9000" // the port user will be connecting to

#define BACKLOG 10 //how many pending connections queue will hold
#define BUF_SIZE 1024 // size of the buffer for receiving data
static FILE *data_file = NULL;
void sig_terminate(int s){
    syslog(LOG_INFO, "Caught signal %d, exiting\n",s);
    close(sockfd);
    fclose(data_file);
    remove("/var/tmp/aesdsocketdata");
    exit(EXIT_SUCCESS);

}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc,char*argv[])
{
    int new_fd;
    int daemon = 0;
    if (argc == 2 && strcmp(argv[1], "-d") == 0) {
        daemon = 1;
    }
    openlog("aesdsocket",LOG_PID,LOG_USER);
     data_file = fopen("/var/tmp/aesdsocketdata", "w+");
    if (!data_file) {
        syslog(LOG_ERR, "Cannot open or create data file");
        exit(EXIT_FAILURE);
    }
    struct addrinfo hints, *res;
    struct sockaddr_storage their_addr; // connector's address information
    struct sigaction sa; //signals
    
    // setup the signal
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler=sig_terminate;
   sigaction (SIGINT,&sa,NULL);
    sigaction(SIGTERM,&sa,NULL);
    
     
    

    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_INET; // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;  // fill in my IP for me
    
    getaddrinfo(NULL, PORT, &hints, &res);

     // socket
    sockfd=socket(res->ai_family,res->ai_socktype,0);
       int optval = 1;
    setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
// bind
    bind(sockfd,res->ai_addr,res->ai_addrlen);
       
//listen
    listen(sockfd,BACKLOG);
      
if(daemon){
    if(fork()!=0)
    {
        exit(EXIT_SUCCESS);
    }
setsid();
chdir("/");
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);

}
 while(1) {
// accepting
    
   struct sockaddr_in client_address;
   
        socklen_t addr_size = sizeof(client_address);
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    
    char s[INET_ADDRSTRLEN];
    // reading the ip address
    //struct sockaddr *sa=(struct sockaddr*)&their_addr;
    inet_ntop(AF_INET, get_in_addr((struct sockaddr *)&their_addr),s, INET_ADDRSTRLEN);   

    syslog(LOG_DEBUG,"Accepted connection from %s\n",s);
// recieve the data in the buffer and storing in the file 
      char buffer[BUF_SIZE]={0};
      
     
         char single_char;
         ssize_t  numbytes; //listen on sock_fd, new connection on new_fd
while ((numbytes = recv(new_fd, &single_char, 1, 0)) > 0) {
        fwrite(&single_char, 1, numbytes, data_file);
        if(single_char == '\n') break;
    }
    fflush(data_file);
    rewind(data_file);
   // send data from the file to the IP

    
    while ((numbytes = fread(buffer, 1,BUF_SIZE, data_file)) > 0) {
        send(new_fd, buffer,numbytes, 0);
    }
        
        syslog(LOG_INFO, "Closed connection from %s\n", s);
        close(new_fd);
    }
return 0;
}