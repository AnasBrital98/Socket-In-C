#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include<wait.h>
    int main(void)
    {
      int SocketDescripteur = 0,ClientSocket = 0;
      
      struct sockaddr_in ServerAdd;
    
      char sendBuff[1025],receiveBuff[1025];  
      int numrv;  
    
      SocketDescripteur = socket(AF_INET, SOCK_STREAM, 0);      
      memset(&ServerAdd, '0', sizeof(ServerAdd));
      memset(sendBuff, '0', sizeof(sendBuff));
      memset(receiveBuff,'0',sizeof(receiveBuff));    
      ServerAdd.sin_family = AF_INET;    
      ServerAdd.sin_addr.s_addr = htonl(INADDR_ANY); 
      ServerAdd.sin_port = htons(5000);    
    
      bind(SocketDescripteur, (struct sockaddr*)&ServerAdd,sizeof(ServerAdd));
      
      if(listen(SocketDescripteur, 10) == -1){
          perror("Failed to listen ... \n");
          exit(EXIT_FAILURE);
      }     
      
      ClientSocket = accept(SocketDescripteur, (struct sockaddr*)NULL ,NULL);
  
      if(recv(ClientSocket,receiveBuff,sizeof(receiveBuff),0) == -1)
      {
          perror("Failed to recive ...\n");
          exit(EXIT_FAILURE);
      }
      if(fork()==0)
      {
          int count = 0;
      char *ptr = receiveBuff;
      while((ptr = strchr(ptr, ' ')) != NULL) {
          count++;
          ptr++;
      }
      char *argv[count+2];
      const char *delimiter = " ";
      char *token;
      token = strtok(receiveBuff,delimiter);
      int i=0;
      while(token != 0)
      {
        argv[i] = malloc(sizeof(token)+1);	
        strcpy(argv[i],token);
        i++;
        token = strtok(0,receiveBuff);
      }
      argv[i] = NULL;
      dup2(ClientSocket,STDOUT_FILENO);
      execvp(argv[0],argv);
      }
      wait(NULL);
      close(ClientSocket);    

  return 0;
}
