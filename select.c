   #include<socket.h>
   #include<sys/select.h>
   #define KEY 1
   #define ip "192.168.3.239"
   int main()
   {
       struct sockaddr_in serveraddr,clientaddr;
       serveraddr.sin_family=AF_INET;
      serveraddr.sin_port=htons(8080);
      inet_pton(AF_INET,ip,&serveraddr.sin_addr.s_addr);
      int serversock,clientsock;
      serversock=SOCKET(AF_INET,SOCK_STREAM,0);
      bind(serversock,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
      socklen_t recvlen= sizeof(serveraddr);
      listen(serversock,128);
  
      fd_set nset,oset;
      int max_recvmun;
      FD_ZERO(&nset);
      FD_SET(serversock,&nset);
      max_recvmun=serversock;
      ssize_t get_len;
     int client_add[1020];
      char recvbuf[1500];
      int fact=0;
      for( int i=0;i<1020;i++)
     {
          client_add[i]=0;
      }
    int recvnum;
      printf("open select...\n");
      while(KEY)
      {
          oset=nset;
          recvnum=select(max_recvmun+1,&oset,NULL,NULL,NULL);
         while(recvnum)
          {
              if((FD_ISSET(serversock,&oset)))
              {
                 recvlen=sizeof(clientaddr);
                  clientsock= ACCEPT(serversock,(struct sockaddr*)&clientaddr,&recvlen);
                  FD_SET(clientsock,&oset);
                  printf("connect success clientsock %d\n",clientsock);
              for(int i=0;i<1020;i++)
              {
                  if(client_add[i]==0)
                  {
                      client_add[i]=clientsock;
                      break;
                  }
              }
              if(max_recvmun<clientsock)
              {
                  max_recvmun=clientsock;
              }
              }
              else
              {
                  for(int i=0;i<1020;i++)
                  {
                      if(client_add[i]>0)
                      {
                          if(FD_ISSET(client_add[i],&oset))
                          {
                              fact=0;
                          if((get_len=RECV(client_add[i],recvbuf,sizeof(recvbuf),0))>0)
                          {
                              printf("recv message %s\n",recvbuf);
                              while(get_len>fact)
                              {
  
                                  recvbuf[fact]=toupper(recvbuf[fact]);
                                  fact++;
                             }
                              send(client_add[i],recvbuf,get_len,0);
                              bzero(recvbuf,sizeof(recvbuf));
                          }
                         if(get_len==0)
                          {
                             FD_CLR(client_add[i],&oset);
                              close(client_add[i]);
                              client_add[i]=0;
                          }
                          }
                          break;
                      }
                  }
              }
              --recvnum;
         }
  
      }
      return 0;
  }                                                                                                                                                                                                                                                                                                                                                                                                                                
                                                                                                                                                                           
