/****************************************
*Name        : menu.c        
*Data        : 2020.08.07
*Description : display menu
*Author      : JiaqingWan
*Modification: None
*
****************************************/

/************the includes**************/
#include<stdio.h>

/****************************************
*Name       : menu_wellcome()
*Description: display the wellcom menu
*args       : 
             @none
*return     :int：return  max number of 
			 the choose 
****************************************/
int menu_wellcome()
{
	printf("***********wellcome***********\n");
	printf("1.Register\n");
	printf("2.Login\n");
	printf("3.Quit\n");
	printf("******************************\n");
	return 3;
}

/****************************************
*Name       :menu_register
*Description:display the register menu
*args       :
             @none
*return     :int：return  max number of 
			 the choose 
****************************************/
int menu_register()
{
	printf("***********register***********\n");
	printf("1.Register User\n");
	printf("2.Register Manager\n");
	printf("******************************\n");
	return 2;
}

/****************************************
*Name       : menu_login
*Description: display the login menu
*args       : 
              @none
*return     : return  max number of 
			  the choose 
****************************************/
int menu_login()
{
	printf("*************login************\n");
	printf("1.User Login\n");
	printf("2.Manager Login\n");
	printf("******************************\n");
	return 2;
}

/****************************************
*Name       : menu_user_function
*Description: display the user function 
              menu
*args       :
              none
*return     : int：return  max number of 
			  the choose 
****************************************/
int menu_user_function()
{
	printf("*************usr**************\n");
	printf("1.Inquire Info\n");
	printf("2.Renew Password\n");
	printf("3.Modify Information\n");
	printf("4.Inquire History\n");
	printf("5.Quit\n");
	printf("******************************\n");
	return 4;
}

/****************************************
*Name       : menu_manager_function
*Description: display the manager function 
              menu
*args       :
              none
*return     :int：return  max number of 
			  the choose 
****************************************/
int menu_manager_function()
{
	printf("**********manager*************\n");
	printf("1.Add User\n");
	printf("2.Delect User\n");
	printf("3.Modify Information\n");
	printf("4.Inquire Information\n");
	printf("5.Inquire History\n");
	printf("6.Quit\n");
	printf("******************************\n");
	return 6;
}

/****************************************
*Name       : menu_choose
*Description: display choose command
*args       :
              none
*return     :
              none 
****************************************/
void  menu_choose()
{
	printf("Your Choose>");
}

#if  0

int main(int argc, const char *argv[])
{
	 menu_wellcome();
	 menu_register();
	 menu_login();
	 menu_user_function();
	 menu_manager_function();
	  menu_choose();
	return 0;
}


#endif
