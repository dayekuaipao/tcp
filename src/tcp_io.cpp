#include "tcp_io.h"
bool TcpRead(const int sockfd,char *buffer,int *ibuflen,const int itimeout)
{
  if (sockfd == -1) return false;

  if (itimeout > 0)
  {
    fd_set tmpfd;

    FD_ZERO(&tmpfd);
    FD_SET(sockfd,&tmpfd);

    struct timeval timeout;
    timeout.tv_sec = itimeout; timeout.tv_usec = 0;

    int i;
    if ( (i = select(sockfd+1,&tmpfd,0,0,&timeout)) <= 0 ) return false;
  }

  (*ibuflen) = 0;

  if (Readn(sockfd,(char*)ibuflen,4) == false) return false;

  if (Readn(sockfd,buffer,(*ibuflen)) == false) return false;

  return true;
}

bool TcpWrite(const int sockfd,const char *buffer,const int ibuflen)
{
  if (sockfd == -1) return false;

  fd_set tmpfd;

  FD_ZERO(&tmpfd);
  FD_SET(sockfd,&tmpfd);

  struct timeval timeout;
  timeout.tv_sec = 5; timeout.tv_usec = 0;

  if ( select(sockfd+1,0,&tmpfd,0,&timeout) <= 0 ) return false;
  
  int ilen=0;

  // 如果长度为0，就采用字符串的长度
  if (ibuflen==0) ilen=strlen(buffer);
  else ilen=ibuflen;

  char strTBuffer[ilen+4];
  memset(strTBuffer,0,sizeof(strTBuffer));
  memcpy(strTBuffer,&ilen,4);
  memcpy(strTBuffer+4,buffer,ilen);
  
  if (Writen(sockfd,strTBuffer,ilen+4) == false) return false;

  return true;
}

bool Readn(const int sockfd,char *buffer,const size_t n)
{
  int nLeft,nread,idx;

  nLeft = n;
  idx = 0;

  while(nLeft > 0)
  {
    if ( (nread = recv(sockfd,buffer + idx,nLeft,0)) <= 0) return false;

    idx += nread;
    nLeft -= nread;
  }

  return true;
}

bool Writen(const int sockfd,const char *buffer,const size_t n)
{
  int nLeft,idx,nwritten;
  nLeft = n;  
  idx = 0;
  while(nLeft > 0 )
  {    
    if ( (nwritten = send(sockfd, buffer + idx,nLeft,0)) <= 0) return false;      
    
    nLeft -= nwritten;
    idx += nwritten;
  }

  return true;
}
