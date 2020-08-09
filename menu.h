#ifndef __MENU_H__
#define __MENU_H__

/****************************************
*Name       : menu_wellcome()
*Description: display the wellcom menu
*args       : 
             @none
*return     :int：return  max number of 
			 the choose 
****************************************/
int menu_wellcome();

/****************************************
*Name       :menu_register
*Description:display the register menu
*args       :
             @none
*return     :int：return  max number of 
			 the choose 
****************************************/
int menu_register();

/****************************************
*Name       : menu_login
*Description: display the login menu
*args       : 
              @none
*return     : return  max number of 
			  the choose 
****************************************/
int menu_login();

/****************************************
*Name       : menu_user_function
*Description: display the user function 
              menu
*args       :
              none
*return     : int：return  max number of 
			  the choose 
****************************************/
int menu_user_function();

/****************************************
*Name       : menu_manager_function
*Description: display the manager function 
              menu
*args       :
              none
*return     :int：return  max number of 
			  the choose 
****************************************/
int menu_manager_function();

/****************************************
*Name       : menu_choose
*Description: display choose command
*args       :
              none
*return     :
              none 
****************************************/
void  menu_choose();


#endif