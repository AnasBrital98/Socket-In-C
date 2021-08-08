#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>

int main(int argc, char** argv) {
  if(argc<2)
  {
      fprintf(stderr,"Not Enough Parameters ...\n");
      exit(EXIT_FAILURE);
  }
  struct sockaddr_un SocketAddress;
  int socketDescripteur,rc;

  if ( (socketDescripteur = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
      fprintf(stderr,"Failed to create The Socket ...\n");
      exit(EXIT_FAILURE);
  }

  memset(&SocketAddress, 0, sizeof(SocketAddress));
  bcopy(argv[1],SocketAddress.sun_path,sizeof(argv[1]));
  SocketAddress.sun_family = AF_UNIX;

  if (connect(socketDescripteur, (struct sockaddr*)&SocketAddress, sizeof(SocketAddress)) == -1) {
      fprintf(stderr,"Failed to Connect To The Server ...\n");
      exit(EXIT_FAILURE);
  }

  if(send(socketDescripteur,argv[2],sizeof(argv[2]),0) == -1)
  {
      fprintf(stderr,"Failed to Send The Message ...\n");
      exit(EXIT_FAILURE);
  }  

  if(recv(socketDescripteur,argv[2],sizeof(argv[2]),0) == -1)
  {
      fprintf(stderr,"Failed to Receive The Message ...\n");
      exit(EXIT_FAILURE);
  }  
   
   printf("Message From The Server :  %s \n",argv[2]);
   close(socketDescripteur);

  return 0;
}