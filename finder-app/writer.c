// #include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
int fd;
openlog(argv[0],0,LOG_USER);

if(argc!=3)
{
    syslog(LOG_ERR,"ERROR please provide 2 arguments arg1 = file arg2= string");
     return 1;

}
   fd=open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644); 
   if(fd == -1)
   {
     syslog(LOG_ERR,"ERROR while opening the file");
     return 1;
   }

   
   ssize_t nr;
   ssize_t count;
   count =strlen(argv[2]);
 nr = write(fd,argv[2],count);
 if(nr==-1)
 {
    syslog(LOG_ERR,"ERROR while writing the string to the file");
    return 1;
 }
 else if(nr!= count){
         syslog(LOG_ERR,"partial string is write in the file");
         return 1;
 }
 else{
syslog(LOG_DEBUG,"Writing %s to solution.txt",argv[2]);
}
if(close(fd)== -1)
{
  //perror("close");
   syslog(LOG_ERR,"ERROR in closing the file");
   return 1;
}

return 0;
 }

