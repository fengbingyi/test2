#include<stdio.h>
#include"msg.h"
#include"menu.h“
#include"client.h"

void login(void)
{
	do{
		menu_login();
		menu_choose();
		scanf("%d",&choose);
	}while(!(choose > 0)&&(choose <= menu_maxno));
	
}

int reg_manager()
{
	msg.cmd = REGISTER_MANAGER;
	printf("please input ")
	msg.password = 
	return OK;
}

int reg_usr()
{
	printf("please input user name>");
	scanf("%s",msg.);
	return OK;
}

void reg(void)
{
	int ret = 0;
	do{
		menu_register();
		menu_choose();
		scanf("%d",&choose);
	}while(!(choose > 0)&&(choose <= menu_maxno));
	/*register user*/
	if(1 == choose)
	{
		ret = reg_usr();
		if(OK == ret)
			printf("regitser user success\n");
		else
			printf("regitser user failed\n");
	}
	
	/*register manager*/
	if(2 == choose)
	{
		ret = reg_manager();
		if(OK == ret)
			printf("regitser user success\n");
		else
			printf("regitser user failed\n");
	}	
}

int main(int argc, const char *argv[])
{
	int choose     = 0 ;   /*the choose of selection     */
	int menu_maxno = 0 ;   /*max number of menu          */
	int socketfd   = -1;   /*the file decription ofsocket*/
	//fd = client_init();
	while(1){
		do{
			menu_wellcome();
			menu_choose();
			scanf("%d",&choose);
		}while(!(choose > 0)&&(choose <= menu_maxno));

		if(1 == choose){
			reg();
		}

		if(2 == choose){
			login();
		}

		if(3 == choose)
		{
			break;
		}
	}
	//client_exit(socketfd);
	return 0;
}

