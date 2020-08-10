#include<stdio.h>
#include<string.h>
#include"msg.h"
#include"menu.h"
#include"client.h"


void Add_User(int socketfd)
{
	msg_t msg_add;
	msg_t rev_msg;
	msg_add = msg;
	msg_add.cmd = ADD_USR;
	printf("please input the usr you want to add>");
	scanf("%s",msg_add.usr);
	printf("please input the name you want to add>");
	scanf("%s",msg_add.name);
	printf("please input the age you want to add>");
	scanf("%d",&msg_add.age    );
	printf("please input the sex you want to add>");
	scanf("%c",&msg_add.sex    );
	printf("please input the password you want to add>");
	scanf("%s",msg_add.password);
	printf("please input the number you want to add>");
	scanf("%d",&msg_add.number );
	printf("please input the salary you want to add>");
	scanf("%d",&msg_add.salary );
	printf("please input the dept you want to add>");
	scanf("%s",msg_add.dept    );
	/*ready to add to server*/
	client_send(socketfd,&msg_add,sizeof(msg_add));
	client_receive(socketfd,&rev_msg,sizeof(rev_msg));
	if(OK == rev_msg.status){
		printf("add successed!");
	}else{
		printf("add failed!");
	}

}

void Delect_User(int socketfd)
{
	msg_t msg_del;
	msg_t rev_msg;
	msg_del = msg;
	msg_del.cmd = DELECT_USR;
	printf("please input the usr you want to delect>");
	scanf("%s",msg_del.usr);
	client_send(socketfd,&msg_del,sizeof(msg_del));
	client_receive(socketfd,&rev_msg,sizeof(rev_msg));
	if(OK == rev_msg.status){
		printf("delect successed!\n");
	}else{
		printf("delect failed!\n");
	}
}   

void Inquire_Information(int socketfd) 
{
	msg_t rev_msg;
	msg.cmd = INQUIRE_INFO;
	client_send(socketfd,&msg,sizeof(msg));
	client_receive(socketfd,&rev_msg,sizeof(rev_msg));
	printf("1.name:%s\n",    msg.name    );
	printf("2.age:%d\n",     msg.age     );
	printf("3.sex:%c\n",     msg.sex     );
	printf("4.password:%s\n",msg.password);
	printf("5.number:%d\n",  msg.number  );
	printf("6.salary:%d\n",  msg.salary  );
	printf("7.dept:%s\n",    msg.dept    );
}    

void Modify_Information(int socketfd)
{
	int choose     = 0 ;   /*the choose of selection     */
	int menu_maxno = 0 ;   /*max number of menu          */
	msg_t msg_mod = msg;
	msg_t rev_msg;
	Inquire_Information(socketfd);
	/*进入修改循环*/
	while(1){
		do{
			menu_modify();			
			menu_choose();
			scanf("%d",&choose);
			getchar();
		}while(!(choose>=0 && choose < 8));
		if(choose == 1)
		{
			printf("input modity name>");
			scanf("%s",msg_mod.name);
			getchar();
		}
		if(choose == 2)
		{
			printf("input modity age>");
			scanf("%d",&msg_mod.age);
			getchar();
		}
		if(choose == 3)
		{
			printf("input modity sex>");
			scanf("%c",&msg_mod.sex);      /*暂时没加参数检测*/
			getchar();
		}

		if(choose == 4)
		{
			if(msg.pri == MANAGER){
			printf("input modity password>");
			scanf("%s",msg_mod.password); /*暂时没添加参数检测*/
			getchar();
			}else{
				printf("permission error\n");
				break;
			}
		}

		if(choose == 5)
		{
			if(msg.pri == MANAGER){
			printf("input modity number>");
			scanf("%d",&msg_mod.number);
			getchar();
			}else{
				printf("permission error\n");
				break;
			}
		}

		if(choose == 6)
		{
			if(msg.pri == MANAGER){
			printf("input modity number>");
			scanf("%d",&msg_mod.salary);
			getchar();
			}else{
				printf("permission error\n");
				break;
			}
		}

		if(choose == 7)
		{
			if(msg.pri == MANAGER){
				printf("input modity dept>");
				scanf("%s",msg_mod.dept);
				getchar();
			}else{
				printf("permission error\n");
				break;
			}
		}

		if(choose == 0)break;
	}
	/*modify infomation finished，begin to send*/
	client_send(socketfd,&msg_mod,sizeof(msg_mod));
	client_receive(socketfd,&rev_msg,sizeof(rev_msg));
	if(OK == rev_msg.status){
		printf("modify successed!\n");
	}else{
		printf("modity failed!\n");
	}
} 

void Renew_Password(int socketfd)
{
	char pw_buf[10] = {};
	msg_t rev_msg;

	msg.cmd = VERIFY_PASSWORD;
	printf("input the password you want to verify>");
	scanf("%s",pw_buf);               /*参数检测待完善*/
	strncpy(msg.password,pw_buf,10);
	client_send(socketfd,&msg,sizeof(msg));
	client_receive(socketfd,&rev_msg,sizeof(rev_msg));
	if(OK == rev_msg.status){
		printf("renew password successed!\n");
	}else{
		printf("renew password failed!\n");
	}
}  




void Inquire_History(int socketfd)
{
	printf("unfinish!\n");
}   

void manager_function(int socketfd)
{
	int choose     = 0 ;   /*the choose of selection     */
	int menu_maxno = 0 ;   /*max number of menu          */
	while(1){
		do{
			menu_manager_function();
			menu_choose();
			scanf("%d",&choose);
		}while(!(choose > 0)&&(choose <= menu_maxno));

		switch(choose){
		case 1:Add_User(socketfd)           ;break;
		case 2:Delect_User(socketfd)        ;break;
		case 3:Modify_Information(socketfd) ;break;
		case 4:Inquire_Information(socketfd);break;
		case 5:Inquire_History(socketfd)    ;break;
		default:                             break;
		}
		if(choose == 6)break;
	}
}

void user_function(int socketfd)
{
	int choose     = 0 ;   /*the choose of selection     */
	int menu_maxno = 0 ;   /*max number of menu          */
	while(1){
		do{
			menu_user_function();
			menu_choose();
			scanf("%d",&choose);
		}while(!(choose > 0)&&(choose <= menu_maxno));
		switch(choose){
		case 1:Inquire_Information(socketfd);break;
		case 2:Renew_Password(socketfd)     ;break;
		case 3:Modify_Information(socketfd) ;break;
		case 4:Inquire_History(socketfd)    ;break;
		default:                             break;
		}

		if(choose == 5)break;
	}

}

int login_manager(int socketfd)
{
	char usr_buf[10] = {};
	char pw_buf[10] = {};
	msg_t rev_msg;
	int ret = -1;	
	/*fill login manager command */
	msg.cmd = LOG_MANAGER ;
	/*get manager name */
	printf("please input manager name:");
	scanf("%s",usr_buf);
	getchar();
	/*get password*/
	printf("please input password:");
	scanf("%s",pw_buf);
	getchar();
	bzero(msg.usr,10);
	bzero(msg.password,10);
	strncpy(msg.usr,usr_buf,10);
	strncpy(msg.password,pw_buf,10);
	client_send(socketfd,&msg,sizeof(msg));
	client_receive(socketfd,&rev_msg,sizeof(rev_msg));
	if(OK == rev_msg.status){
		msg.pri = MANAGER;
		return OK;
	}
	return NO;
}


int login_user(int socketfd)
{
	char usr_buf[10] = {}; /*user name buffer   */
	char pw_buf[10] = {};  /*password buffer    */
	msg_t rev_msg;         /*receive from server*/
	int ret = -1;	
	msg.cmd = LOG_USR ;
	printf("please input user name:");
	scanf("%s",usr_buf);
	getchar();
	printf("please input password:");
	scanf("%s",pw_buf);
	getchar();
	bzero(msg.usr,10);
	bzero(msg.password,10);
	strncpy(msg.usr,usr_buf,10);
	strncpy(msg.password,pw_buf,10);
	client_send(socketfd,&msg,sizeof(msg));
	client_receive(socketfd,&rev_msg,sizeof(rev_msg));

	if(OK == rev_msg.status){
		msg.pri = USER;
		return OK;
	}
	return NO;


}


void login(int socketfd)
{
	char usr_buf[10] = {};
	char pw_buf[10] = {};
	msg_t rev_msg;
	int ret = -1;
	int choose     = 0 ;   /*the choose of selection     */
	int menu_maxno = 0 ;   /*max number of menu          */
	do{
		menu_login();
		menu_choose();
		scanf("%d",&choose);
	}while(!(choose > 0)&&(choose <= menu_maxno));

	if(1 == choose)
	{
		ret = login_user(socketfd);
		if(OK == ret ){
			user_function(socketfd);
		}else{
			printf("login error\n");
		}
	}

	if(2 == choose)
	{
		ret = login_manager(socketfd);
		if(OK == ret ){
			manager_function(socketfd);
		}else{
			printf("login error\n");
		}
	}
}

int reg_manager(int sockfd)
{
	char usr_buf[10] = {};
	char pw_buf[10] = {};
	msg_t rev_msg;
	int ret = -1;
	msg.cmd = REGISTER_MANAGER ;
	printf("please input manager name:");
	scanf("%s",usr_buf);
	getchar();
	printf("please input password:");
	scanf("%s",pw_buf);
	getchar();
	bzero(msg.usr,10);
	bzero(msg.password,10);
	strncpy(msg.usr,usr_buf,10);
	strncpy(msg.password,pw_buf,10);
	client_send(sockfd,&msg,sizeof(msg));
	client_receive(sockfd,&rev_msg,sizeof(rev_msg));
	if(OK == rev_msg.status){
		return OK;
	}else{
		return NO;
	}

}

int reg_usr(int sockfd)
{
	char usr_buf[10] = {};
	char pw_buf[10] = {};
	msg_t rev_msg;
	int ret = -1;
	msg.cmd = REGISTER_USR ;
	printf("please input user name:");
	scanf("%s",usr_buf);
	getchar();
	printf("please input password:");
	scanf("%s",pw_buf);
	getchar();
	bzero(msg.usr,10);
	bzero(msg.password,10);
	strncpy(msg.usr,usr_buf,10);
	strncpy(msg.password,pw_buf,10);
	client_send(sockfd,&msg,sizeof(msg));
	client_receive(sockfd,&rev_msg,sizeof(rev_msg));
	if(OK == rev_msg.status){
		return OK;
	}else{
		return NO;
	}
}

void reg(int socketfd)
{
	int ret        = 0;
	int choose     = 0 ;   /*the choose of selection     */
	int menu_maxno = 0 ;   /*max number of menu          */
	do{
		menu_register();
		menu_choose();
		scanf("%d",&choose);
		getchar();
	}while(!(choose > 0)&&(choose <= menu_maxno));
	/*register user*/
	if(1 == choose)
	{
		ret = reg_usr(socketfd);
		if(OK == ret)
			printf("regitser user success\n");
		else
			printf("regitser user failed\n");
	}

	/*register manager*/
	if(2 == choose)
	{
		ret = reg_manager(socketfd);
		if(OK == ret)
			printf("regitser manager success\n");
		else
			printf("regitser manager failed\n");
	}	
}

int main(int argc, const char *argv[])
{
	int choose     = 0 ;   /*the choose of selection     */
	int menu_maxno = 0 ;   /*max number of menu          */
	int socketfd   = -1;   /*the file decription ofsocket*/
	int fd = client_init();/*Init client                 */
	while(1){
		do{
			menu_wellcome();
			menu_choose();
			scanf("%d",&choose);
			getchar();
		}while(!(choose > 0)&&(choose <= menu_maxno));

		if(1 == choose){
			reg(fd);
		}

		if(2 == choose){
			login(fd);
		}

		if(3 == choose)
		{
			break;
		}
	}
	client_exit(socketfd);
	return 0;
}

