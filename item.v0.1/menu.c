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
	printf("************员工管理系统************\n");
	printf("1.注册\n");
	printf("2.登录\n");
	printf("3.退出\n");
	printf("************************************\n");
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
	printf("**************注册************\n");
	printf("1.普通用户注册\n");
	printf("2.管理员注册\n");
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
	printf("**************登录************\n");
	printf("1.普通用户登录\n");
	printf("2.管理员登录\n");
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
	printf("*************用户**************\n");
	printf("1.查询本人基本信息\n");
	printf("2.重置密码\n");
	printf("3.修改本人信息\n");
	printf("4.查询历史记录\n");
	printf("5.退出\n");
	printf("*******************************\n");
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
	printf("************管理员*************\n");
	printf("1.增添用户\n");
	printf("2.删除用户\n");
	printf("3.修改用户信息\n");
	printf("4.查询用户信息\n");
	printf("5.查询历史记录\n");
	printf("6.返回上一级\n");
	printf("******************************\n");
	return 6;
}



/****************************************
*Name       : menu_modify
*Description: modify information
*args       :
              none
*return     :
              none 
****************************************/
int  menu_modify()
{
	printf("******************************\n");
	printf("M is manager ,U is user\n");
	printf("1.name(M&U) 2.age(M&U) 3.sex(M&U) 4.password(M) \n");
	printf("5.number(M) 6.salary(M) 7.dept(M) 0.finish modify\n");
	printf("******************************\n");
	return 8;
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
	printf("请选择>");
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
