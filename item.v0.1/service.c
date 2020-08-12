#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include <signal.h>
#include <time.h>
#include "msg.h"


#define IP "192.168.0.105"
#define PORT "6666"
#define   N  32


#define  DATABASE  "my.db"


int do_client(int acceptfd, sqlite3 *db);                        //子进程处理客户端 1
void do_manager_register(int acceptfd, msg_t *msg, sqlite3 *db); //注册管理员       1
void do_usr_register(int acceptfd, msg_t *msg, sqlite3 *db);     //注册用户         1
int do_usr_login(int acceptfd, msg_t *msg, sqlite3 *db);         //管理员登录       1
int do_manager_login(int acceptfd, msg_t *msg, sqlite3 *db);     //用户登录         1
int do_inquir_info(int acceptfd, msg_t *msg, sqlite3 *db); 	     //查询
void do_update_info(int acceptfd, msg_t *msg, sqlite3 *db);      //修改信息
void do_delete_user(int acceptfd, msg_t *msg, sqlite3 *db);      //删除一条信息 包括账号密码
void do_add_userinfo(int acceptfd, msg_t *msg, sqlite3 *db);      //添加用户信息= 注册+添加信息
void renew_password(int acceptfd, msg_t *msg, sqlite3 *db);      //修改信息
int callback(void* arg,int f_num,char** f_value,char** f_name);
int get_date(char *date);


// ./server  192.168.0.105  6666


int main(int argc, const char *argv[])
{

	int sockfd;
	struct sockaddr_in  serveraddr;
	int n;
	msg_t  msg;
	sqlite3 *db;
	int acceptfd;
	pid_t pid;	
	char *errmsg;
	char sql[128];

	/*
	   if(argc != 3)
	   {
	   printf("Usage:%s serverip  port.\n", argv[0]);
	   return -1;
	   }
	   */

	//创建数据库 创建表
	//打开数据库
	if(sqlite3_open(DATABASE, &db) != SQLITE_OK)
	{
		printf("%s\n", sqlite3_errmsg(db));
		printf("数据库打开失败!\n");
		return -1;
	}
	printf("数据库打开-----------------成功!\n");


	//创建表
	sprintf(sql,"create table info (number INTEGER PRIMARY KEY autoincrement,usr char,password char,pri char,name char,age int,sex char,salary int, dept char);");
	if(sqlite3_exec(db, sql,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("%s\n",errmsg);
	}else{
		printf("info 表创建----------------成功!\n");
	}



	//创建socket
	if((sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0)
	{
		perror("fail to socket.\n");
		return -1;
	}
	printf("创建套接字SOCKFD-----------成功!\n");




	//地址快速重用
	int b_reuse = 1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&b_reuse,sizeof(int))<0)
	{
		perror("setsockopt");
		exit(1);
	}
	printf("设置地址快速重用-----------成功!\n");



	//填充结构体
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	serveraddr.sin_port = htons(atoi(PORT));




	//绑定
	if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("fail to bind.\n");
		return -1;
	}
	printf("绑定地址-------------------成功!\n");




	// 将套接字设为监听模式
	if(listen(sockfd, 5) < 0)
	{
		printf("fail to listen.\n");
		return -1;
	}
	printf("套接字监听模式设置---------成功!\n");



	//处理僵尸进程
	signal(SIGCHLD, SIG_IGN);



	//死循环
	while(1)
	{

		if((acceptfd = accept(sockfd, NULL, NULL)) < 0)
		{
			perror("fail to accept");
			return -1;
		}

		//创建子进程
		if((pid = fork()) < 0)
		{
			perror("fail to fork");
			return -1;
		}
		else if(pid == 0)  // 儿子进程
		{
			//处理客户端具体的消息
			close(sockfd);
			do_client(acceptfd, db);

		}
		else  // 父亲进程,用来接受客户端的请求的
		{
			close(acceptfd);
		}
	}

	return 0;
}





/********************************************************************************
 *函数名：do_client(int acceptfd, sqlite3 *db)
 *
 *功能  ：处理客户端请求函数
 *
 *参数  ：int acceptfd文件描述符        sqlite3 *db句柄
 *
 *返回值：0成功
 *
 * *****************************************************************************/
int do_client(int acceptfd, sqlite3 *db)
{
	msg_t msg;
	//接收客户端的msg
	while(recv(acceptfd, &msg, sizeof(msg_t), 0) > 0)
	{
		printf("cmd:%d\n", msg.cmd);
		switch(msg.cmd)
		{
		case REGISTER_MANAGER:
			printf("开始注册管理员\n");
			do_manager_register(acceptfd, &msg, db);
			break;
		case REGISTER_USR:
			printf("开始注册普通用户\n");
			do_usr_register(acceptfd, &msg, db);
			break;
		case LOG_USR:
			printf("普通用户登录\n");
			do_user_login(acceptfd, &msg, db);
			break;
		case LOG_MANAGER:
			printf("管理员登录\n");
			do_admin_login(acceptfd, &msg, db);
			break;
		case INQUIRE_INFO:
			printf("查询信息\n");
			do_inquir_info(acceptfd,&msg,db);
			break;
		case VERIFY_PASSWORD:
			printf("修改密码信息\n");
			renew_password(acceptfd,&msg,db);
			break;
		case INQUIRE_HISTORY:
			printf("没实现");
			break;
		case ADD_USR:
			printf("添加用户或者管理员信息\n");
			do_add_userinfo(acceptfd,&msg,db);
			break;
		case DELECT_USR:
			printf("删除用户或者管理员信息\n");
			do_delete_user(acceptfd,&msg,db);	
			break;
		default:
			printf("操作指令非法.\n");
		}

	}

	printf("客户端已退出.\n");
	close(acceptfd);

	return 0;
}


/********************************************************************************
 *函数名：renew_password
 *
 *功能  ：重置密码
 *
 *参数  ：int acceptfd文件描述符  msg_t *msg 通信双方协议  sqlite3 *db句柄
 *
 *返回值：无
 *
 * *****************************************************************************/
void renew_password(int acceptfd, msg_t *msg, sqlite3 *db)      //修改密码信息
{
	char * errmsg;
	char sql[128];
	char **result;   //用来指向sql语句执行的结果
	int nrow,ncloumn;//nrow 满足条件的行数 ncloumn满足条件的列数

	//查询 表中 满足usr = "***" 的 所有记录
	sprintf(sql, "select *  from info where usr='%s';",msg->usr);
	if(sqlite3_get_table(db, sql, &result, &nrow, &ncloumn, &errmsg)!= SQLITE_OK)
	{
		printf("%s\n", errmsg);
		printf("查询失败\n");
	}else{
		//查询到的有满足条件的记录
		if(nrow > 0) //有要删除的用户
		{
			printf("查询此到用户\n");
			sprintf(sql, "update  info set password ='%s' where usr='%s';",msg->password,msg->usr);
			if(sqlite3_exec(db,sql, NULL, NULL, &errmsg) != SQLITE_OK)
			{
				printf("%s\n", errmsg);
				msg->status = NO;
			}	
			msg->status = OK;
			printf("修改密码成功 ");
			send(acceptfd, msg, sizeof(msg_t), 0);
			printf(" send msg success!\n");
		}else
			//没有查询到满足条件
		{

			msg->status = NO;
			printf("无此用户 修改密码失败");
			send(acceptfd, msg, sizeof(msg_t), 0);
			printf("  send msg success!\n");

		}
		sqlite3_free_table(result);
	}
}


/********************************************************************************
 *函数名：do_delete_user
 *
 *功能  ：删除一条信息 包括账号密码
 *
 *参数  ：int acceptfd文件描述符  msg_t *msg 通信双方协议  sqlite3 *db句柄
 *
 *返回值:无
 *
 * *****************************************************************************/
void do_delete_user(int acceptfd, msg_t *msg, sqlite3 *db)      {
	char * errmsg;
	char sql[128];
	char **result;
	int nrow,ncloumn;

	sprintf(sql, "select *  from info where usr='%s';",msg->usr);
	if(sqlite3_get_table(db, sql, &result, &nrow, &ncloumn, &errmsg)!= SQLITE_OK)
	{
		printf("%s\n", errmsg);
		printf("查询失败\n");
	}else{
		//有要删除的用户
		if(nrow > 0) 
		{
			printf("查询此到用户\n");
			sprintf(sql, "delete from info where usr='%s';",msg->usr);
			if(sqlite3_exec(db,sql, NULL, NULL, &errmsg) != SQLITE_OK)
			{
				printf("%s\n", errmsg);
				msg->status = NO;
			}	
			msg->status = OK;
			printf("删除用户或者管理员信息成功");
			send(acceptfd, msg, sizeof(msg_t), 0);
			printf("msg发送成功\n");
		}else
		{

			msg->status = NO;
			printf("无此用户 删除失败");
			send(acceptfd, msg, sizeof(msg_t), 0);	
			printf("msg发送成功\n");

		}
		sqlite3_free_table(result);
	}

}

/********************************************************************************
 *函数名：do_add_userinfo
 *
 *功能  ：添加用户信息
 *
 *参数  ：int acceptfd文件描述符  msg_t *msg 通信双方协议  sqlite3 *db句柄
 *
 *返回值:无
 *
 * *****************************************************************************/
void do_add_userinfo(int acceptfd, msg_t *msg, sqlite3 *db)     
{	
	char * errmsg;
	char sql[128];
	int nrow,ncloumn;
	char ** result;

	//查询是否存在此用户
	sprintf(sql, "select * from info where usr = '%s';", msg->usr);
	if(sqlite3_get_table(db, sql, &result, &nrow, &ncloumn, &errmsg)!= SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}

	//不存在则添加此用户信息
	if(nrow==0)
	{
		sprintf(sql,"insert into info (usr,password,name,age,sex,salary, dept) values('%s','%s','%s',%d,'%s',%d,'%s');",msg->usr,msg->password,msg->name,msg->age,msg->sex,msg->salary,msg->dept);
		printf("%s\n", sql);

		//插入数据到数据库
		if(sqlite3_exec(db,sql, NULL, NULL, &errmsg) != SQLITE_OK)
		{
			printf("%s\n", errmsg);
			msg->status = NO;
			printf("插入数据失败");
			if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
			{
				perror("fail to send");
				return ;
			}
			printf("msg发送成功!\n");
			return;
		}

		msg->status = OK;
		printf("插入数据成功");
		if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
		{
			perror("fail to send");
			return ;
		}
		printf("发送msg成功\n");

	}
	else
	{
		msg->status = NO;
		printf("用户已经存在");
		if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
		{
			perror("fail to send");
			return ;
		}
		printf("msg发送成功!\n");

	}
}



/********************************************************************************
 *函数名：do_update_info 
 *
 *功能  ：修改信息
 *
 *参数  ：int acceptfd文件描述符  msg_t *msg 通信双方协议  sqlite3 *db句柄
 *
 *返回值:无
 *
 * *****************************************************************************/
void do_update_info(int acceptfd, msg_t *msg, sqlite3 *db)
{
	char * errmsg;
	char sql[128];
	int nrow,ncloumn;
	char ** result;

	//查询是否存在此用户
	sprintf(sql, "select * from info where usr = '%s';", msg->usr);
	if(sqlite3_get_table(db, sql, &result, &nrow, &ncloumn, &errmsg)!= SQLITE_OK)
	{
		printf("%s\n", errmsg);
		return;

	}else{

		if(nrow > 0)
		{	
			sprintf(sql,"update info set pri='user',name='%s',age=%d,sex='%s',salary=%d,dept='%s' ;",msg->name,msg->age,msg->sex,msg->salary,msg->dept);
			printf("%s\n", sql);

			//插入数据到数据库
			if(sqlite3_exec(db,sql, NULL, NULL, &errmsg) != SQLITE_OK)
			{
				printf("修改信息失败\n");
				msg->status = NO;
			}
			else
			{
				printf("修改信息成功");

				msg->status = OK;
			}

			if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
			{
				perror("msg发送失败\n");
				return ;
			}

		}else{
			//无此用户 就不用执行Update

			msg->status = NO;

		}
		printf("msg发送成功!\n");

		return ;
	}
}

/********************************************************************************
 *函数名：do_manager_register
 *
 *功能  ：注册管理员账户
 *
 *参数  ：int acceptfd文件描述符  msg_t *msg 通信双方协议  sqlite3 *db句柄
 *
 *返回值:无
 *
 * *****************************************************************************/
void do_manager_register(int acceptfd, msg_t *msg, sqlite3 *db)
{

	char * errmsg;
	char sql[128];
	int nrow,ncloumn;
	char ** result;

	//查询是否存在此用户
	sprintf(sql, "select * from info where usr = '%s';", msg->usr);
	if(sqlite3_get_table(db, sql, &result, &nrow, &ncloumn, &errmsg)!= SQLITE_OK)
	{
		printf("%s\n", errmsg);
		msg->status = NO;

	}else{
		//存在此用户
		if(nrow > 0)
		{	
			msg->status = NO;
			printf("已存在此管理员用户,注册失败");

		}else{
			//不存在则添加管理员
			sprintf(sql, "insert into info (usr,pri,password,name,age,sex,salary,dept) values('%s','admin','%s','admin',18,'man',99999,'admin');", msg->usr, msg->password);
			printf("%s\n", sql);

			//插入数据到数据库
			if(sqlite3_exec(db,sql, NULL, NULL, &errmsg) != SQLITE_OK)
			{
				printf("%s\n", errmsg);
				msg->status = NO;
			}
			else
			{
				msg->status = OK;
				printf("管理员注册成功");
			}

		}	
	}
	if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
	{
		perror("发送msg到客户端失败");
	}
	printf("msg发送成功\n");
	sqlite3_free_table(result);

	return ;


}


/********************************************************************************
 *函数名：do_usr_register
 *
 *功能  ：注册普通员工账户
 *
 *参数  ：int acceptfd文件描述符  msg_t *msg 通信双方协议  sqlite3 *db句柄
 *
 *返回值:无
 *
 * *****************************************************************************/
void do_usr_register(int acceptfd, msg_t *msg, sqlite3 *db)
{
	char * errmsg = NULL;
	char sql[128]={};
	int nrow=-1,ncloumn=-1;
	char ** result = NULL;
	//查询是否存在此用户
	sprintf(sql, "select * from info where usr = '%s';", msg->usr);

	printf("msg->usr = %s\n",msg->usr);
	if(sqlite3_get_table(db, sql, &result, &nrow, &ncloumn, &errmsg)!= SQLITE_OK)
	{
		printf("%s\n", errmsg);
		msg->status = NO;
	}else{
		//存在此用户
		if(nrow != 0)
		{	
			msg->status = NO;
			printf("已存在此普通用户，注册失败");

		}else{

			sprintf(sql, "insert into info (usr,pri,password,name,age,sex,salary,dept) values('%s','user','%s','user',18,'man',99999,'user');", msg->usr, msg->password);
			printf("%s\n", sql);

			//插入数据到数据库
			if(sqlite3_exec(db,sql, NULL, NULL, &errmsg) != SQLITE_OK)
			{
				printf("%s\n", errmsg);
			}
			else
			{
				msg->status = OK;
				printf("普通用户注册成功");
			}

		}	
	}
	if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
	{
		perror("发送msg到客户端失败");
	}
	printf("msg发送成功\n");
	sqlite3_free_table(result);
	return ;

}

/********************************************************************************
 *函数名：do_user_logina
 *
 *功能  ：用户登录
 *
 *参数  ：int acceptfd文件描述符  msg_t *msg 通信双方协议  sqlite3 *db句柄
 *
 *返回值:无
 *
 * *****************************************************************************/

int do_user_login(int acceptfd, msg_t *msg , sqlite3 *db)
{
	char sql[128] = {};
	char *errmsg;
	int nrow;
	int ncloumn;
	char **result;
	sprintf(sql, "select * from info where  usr = '%s' and password = '%s' and pri = 'user';", msg->usr, msg->password);
	printf("%s\n", sql);


	//db句柄
	//sql：数据库操作语句 
	//resultp:用来指向sql语句执行的结果
	//pnRow：满足条件的行数
	//pnColumn：满足条件的列数
	//pzErrmsg：错误信息地址
	if(sqlite3_get_table(db, sql, &result, &nrow, &ncloumn, &errmsg)!= SQLITE_OK)
	{
		printf("%s\n", errmsg);
		msg->status = NO;	
	}
	else
	{
		printf("查询成功----");
		// 查询成功，数据库中拥有此用户
		if(nrow == 1)
		{
			msg->status = OK;	
			printf("用户存在登陆成功");

		}else
		{
			msg->status = NO;	
			printf("无此用户登录失败");
		}	
		if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
		{
			perror("发送msg到客户端失败");
		}
		printf("msg发送成功\n");
	}
	sqlite3_free_table(result);

	return 0;
}
/********************************************************************************
 *函数名：do_admin_login
 *
 *功能  ：管理员登录
 *
 *参数  ：int acceptfd文件描述符  msg_t *msg 通信双方协议  sqlite3 *db句柄
 *
 *返回值: 成功 0 
 *
 * *****************************************************************************/

int do_admin_login(int acceptfd, msg_t *msg , sqlite3 *db)
{
	char sql[128] = {};
	char *errmsg;
	int nrow;
	int ncloumn;
	char **result;
	sprintf(sql, "select * from info where  usr = '%s' and password = '%s' and pri = 'admin';", msg->usr, msg->password);
	printf("%s\n", sql);


	//db句柄
	//sql：数据库操作语句 
	//resultp:用来指向sql语句执行的结果
	//pnRow：满足条件的行数
	//pnColumn：满足条件的列数
	//pzErrmsg：错误信息地址
	if(sqlite3_get_table(db, sql, &result, &nrow, &ncloumn, &errmsg)!= SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("查询成功----");
		// 查询成功，数据库中拥有此用户
		if(nrow == 1)
		{
			msg->status = OK;	
			printf("管理员存在登陆成功");

		}else
		{
			msg->status = NO;	
			printf("无此管理员登录失败");
		}	
		if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
		{
			perror("发送msg到客户端失败");
		}
		printf("msg发送成功\n");
	}
	sqlite3_free_table(result);

	return 0;
}



int get_date(char *date)
{
	time_t t;
	struct tm *tp;

	time(&t);

	//进行时间格式转换
	tp = localtime(&t);

	sprintf(date, "%d-%d-%d %d:%d:%d", tp->tm_year + 1900, tp->tm_mon+1, tp->tm_mday, 
			tp->tm_hour, tp->tm_min , tp->tm_sec);
	printf("get date:%s\n", date);

	return 0;
}


// 得到查询结果，并且需要将查询到的一条数据发送给客户端
int callback(void* arg,int f_num,char** f_value,char** f_name)
{/*
	// info, usr, pri,name,age,sex,password,number, salary,dept
	int acceptfd;
	acceptfd = *((int *)arg);

	msg_t msg;

	strcpy(msg.usr,f_value[0]);
	strcpy(msg.pri,f_value[1]);
	strcpy(msg.name,f_value[2]);
	msg.age    = (*f_value[3]);
	strcpy(msg.sex,f_value[4]);
	strcpy(msg.password,f_value[5]);
	msg.number = (*f_value[6]);
	msg.salary = (*f_value[7]);
	strcpy(msg.dept,f_value[8]);


	send(acceptfd, &msg, sizeof(msg_t), 0);
	*/
	return 0;
}

/********************************************************************************
 *函数名： do_inquir_info
 *
 *功能  ：查询信息
 *
 *参数  ：int acceptfd文件描述符  msg_t *msg 通信双方协议  sqlite3 *db句柄
 *
 *返回值: 成功 0 
 *
 * *****************************************************************************/
int do_inquir_info(int acceptfd, msg_t *msg, sqlite3 *db) 	 //用户查询
{

	char sql[128] = {};
	char *errmsg;
	char ** result;
	int nrow,ncloumn;
	msg_t sendmsg;
	sprintf(sql, "select * from info where usr = '%s';", msg->usr);
	printf("%s\n", sql);

	//db句柄
	//sql：数据库操作语句 
	//resultp:用来指向sql语句执行的结果
	//pnRow：满足条件的行数
	//pnColumn：满足条件的列数
	//pzErrmsg：错误信息地址
	//查询数据库
	if(sqlite3_get_table(db, sql, &result, &nrow, &ncloumn, &errmsg)!= SQLITE_OK)
	{
		printf("%s\n", errmsg);
		sendmsg.status=NO;
		printf("查询失败");
	}
	if(nrow < 1)
	{
		sendmsg.status=NO;
		printf("没有记录\n");
	}else{

		int i=0;
		for(i=0;i<(ncloumn);i++)
		{
			printf("%-10s",result[i]);
		}
		puts("");
		for(i=ncloumn;i<(2*ncloumn);i++)
		{
			printf("%-10s",result[i]);
		}
		puts("");
		i=0;
		sendmsg.number = atoi(result[ncloumn+(i++)]);
		strcpy(sendmsg.usr,result[ncloumn+(i++)]);
		strcpy(sendmsg.password,result[ncloumn+(i++)]);
		strcpy(sendmsg.pri,result[ncloumn+(i++)]);
		strcpy(sendmsg.name,result[ncloumn+(i++)]);
		sendmsg.age = atoi(result[ncloumn+(i++)]);
		strcpy(sendmsg.sex,result[ncloumn+(i++)]);
		sendmsg.salary = atoi(result[ncloumn+(i++)]);
		strcpy(sendmsg.dept,result[ncloumn+(i++)]);

	}

	// 所有的记录查询发送完毕之后，给客户端发出一个结束信息
	sendmsg.status  =  OK;
	send(acceptfd, &sendmsg, sizeof(msg_t), 0);
	printf("查询成功msg发送成功！\n");

	return 0;
}



int do_admin_query(int acceptfd, msg_t *msg, sqlite3 *db)     //管理员查询
{

	/*
	   char sql[128] = {"select * from info;"};
	   char *errmsg;


	//查询数据库
	if(sqlite3_exec(db, sql, callback,(void *)&acceptfd, &errmsg)!= SQLITE_OK)
	{
	printf("%s\n", errmsg);
	}
	else
	{
	printf("Query info done.\n");
	}

	// 所有的记录查询发送完毕之后，给客户端发出一个结束信息
	msg->status  =  OK;

	send(acceptfd, msg, sizeof(msg_t), 0);

*/
	return 0;
}


