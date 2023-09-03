#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>


int main( int argc, char *argv[] )  {

   openlog(NULL,0,LOG_USER);
   syslog(LOG_DEBUG,"Writer App]\n");
   if( argc != 3 ) {
      printf("2 parameters required\n");        //, argv[1]);
	  syslog(LOG_ERR,"2  parameters required\n");
      return 1;
   }
   extern int errno;
   int fd,ErrTemp;

   fd =open(argv[1],O_WRONLY | O_CREAT | O_TRUNC,
S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
   if ( fd ==-1 ){
	   ErrTemp=errno;
	   syslog(LOG_ERR,"%s",strerror(ErrTemp));
	   return 1;
   }   
   
   syslog(LOG_DEBUG,"Writing %s to %s\n",argv[2],argv[1]);
   write(fd,argv[2],strlen(argv[2]));
   return 0;
}
