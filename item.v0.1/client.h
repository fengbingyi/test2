#ifndef __CLIENT_H__
#define __CLIENT_H__

/****************************************
*Name       : client_init
*Description: client init function
*args       :
              none
*return     :socket fd 
****************************************/
int client_init();

/****************************************
*Name       : client_exit
*Description: close the client socket
*args       :
              @fd：socket fd
*return     : none
****************************************/
void client_exit(int fd);

/****************************************
*Name       : client_send
*Description: send message to server
*args       :
              @socketfd：socket fd
			  @buf: the address of buffer
			  @len: the length of the 
			        buffer want to send
*return     : the number of characters send 
****************************************/
int client_send(int sockfd, const void *buf, size_t len);

/****************************************
*Name       : client_receive
*Description: receive message from server
*args       :
              @fd：socket fd
*return     : none
****************************************/
int client_receive(int sockfd, void *buf, size_t len);



#endif
