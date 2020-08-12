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
	char *errmsg;
	pid_t pid;
	char sql[128];
	/*
	   if(argc != 3)
	   {
	   printf("Usage:%s serverip  port.\n", argv[0]);
	   return -1;
	   }
	   */


	//打开数据库
	if(sqlite3_open(DATABASE, &db) != SQLITE_OK)
	{
		printf("%s\n", sqlite3_errmsg(db));
		printf("数据库打开失败\n");
		return -1;
	}
	printf("数据库打开成功\n");
	sprintf(sql,"create table info (number INTEGER PRIMARY KEY autoincrement,usr char,password char,pri char,name char,age int,sex char,salary int, dept char);");
	if(sqlite3_exec(db, sql,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("%s\n",errmsg);
	}else{
		printf("info 表创建成功\n");
	}



	//创建socket
	if((sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0)
	{
		perror("fail to socket.\n");
		return -1;
	}
	printf("创建套接字SOCKFD-----------成功\n");
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


int do_client(int acceptfd, sqlite3 *db)
{
	msg_t msg;
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



			/*			
						case H:
						do_history(acceptfd, &msg, db);
						break;
						*/	
		default:
			printf("Invalid data msg.\n");
		}

	}

	printf("client exit.\n");
	close(acceptfd);
	exit(0);

	return 0;
}

void renew_password(int acceptfd, msg_t *msg, sqlite3 *db)      //修改密码信息
{
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
		{

			msg->status = NO;
			printf("无此用户 修改密码失败");
			send(acceptfd, msg, sizeof(msg_t), 0);
			printf("  send msg success!\n");

		}
		sqlite3_free_table(result);
	}





}


void do_delete_user(int acceptfd, msg_t *msg, sqlite3 *db)      //删除一条信息 包括账号密码
{
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

		if(nrow > 0) //有要删除的用户
		{
			printf("查询此到用户\n");
			sprintf(sql, "delete from info where usr='%s';",msg->usr);
			if(sqlite3_exec(db,sql, NULL, NULL, &errmsg) != SQLITE_OK)
			{
				printf("%s\n", errmsg);
				msg->status = NO;
			}	
			msg->status = OK;
			printf("删除用户或者管理员信息成功 send msg success\n");
			send(acceptfd, msg, sizeof(msg_t), 0);
		}else
		{

			msg->status = NO;
			printf("无此用户 删除失败");
			send(acceptfd, msg, sizeof(msg_t), 0);
			printf("  send msg success!\n");

		}
		sqlite3_free_table(result);
	}

}
void do_add_userinfo(int acceptfd, msg_t *msg, sqlite3 *db)      //添加用户信息= 注册+添加信息
{	
	char * errmsg;
	char sql[128];
	int nrow,ncloumn;
	char ** result;
	sprintf(sql, "select * from info where usr = '%s';", msg->usr);
	if(sqlite3_get_table(db, sql, &result, &nrow, &ncloumn, &errmsg)!= SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	if(nrow==0)
	{
		sprintf(sql,"insert into info (usr,password,pri,name,age,sex,salary, dept) values('%s','%s','%s','%s',%d,'%s',%d,'%s');",msg->usr,msg->password,msg->pri,msg->name,msg->age,msg->sex,msg->salary,msg->dept);
		printf("%s\n", sql);

		//插入数据到数据库
		if(sqlite3_exec(db,sql, NULL, NULL, &errmsg) != SQLITE_OK)
		{
			printf("%s\n", errmsg);
			msg->status = NO;
			if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
			{
				perror("fail to send");
				return ;
			}
			printf("add failed user existed  send msg success!\n");
			return;
		}

		msg->status = OK;
		if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
		{
			perror("fail to send");
			return ;
		}
		printf("add info  success and  send msg success!\n");


	}
	else
	{
		msg->status = NO;
		if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
		{
			perror("fail to send");
			return ;
		}
		printf("add failed user existed  send msg success!\n");


	}
}

void do_update_info(int acceptfd, msg_t *msg, sqlite3 *db)      //修改信息
{
	char * errmsg;
	char sql[128];
	sprintf(sql, "insert into info (usr,pri,password) values('%s','manager','%s');", msg->usr, msg->password);
	printf("%s\n", sql);

	//插入数据到数据库
	if(sqlite3_exec(db,sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
		msg->status = NO;
	}
	else
	{
		printf("client  registera ok!\n");

		msg->status = OK;
	}

	if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
	{
		perror("fail to send");
		return ;
	}
	printf("manager register success and  send msg success!\n");

	return ;
}


void do_manager_register(int acceptfd, msg_t *msg, sqlite3 *db)
{
	char * errmsg;
	char sql[128];
	sprintf(sql, "insert into info (usr,pri,password) values('%s','manager','%s');", msg->usr, msg->password);
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
	}

	if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
	{
		perror("发送msg到客户端失败");
		return ;
	}
	printf("管理员注册成功 msg 发送到客户端  成功\n");

	return ;
}

void do_usr_register(int acceptfd, msg_t *msg, sqlite3 *db)
{
	char * errmsg;
	char sql[128];
	sprintf(sql, "insert into info (usr,pri,password) values('%s','user','%s');", msg->usr, msg->password);
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
	}

	if(send(acceptfd, msg, sizeof(msg_t), 0) < 0)
	{
		perror("发送msg到客户端失败");
		return ;
	}
	printf("用户注册成功 msg 发送到客户端  成功\n");
	return ;
}

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
	}
	else
	{
		printf("get_table ok!\n");
		// 查询成功，数据库中拥有此用户
		if(nrow == 1)
		{
			msg->status = OK;
			send(acceptfd, msg, sizeof(msg_t), 0);
			printf("Query user  success ,user exist! send msg success!\n");
		}

		if(nrow == 0) // 密码或者用户名错误
		{
			msg->status = NO;
			send(acceptfd, msg, sizeof(msg_t), 0);
			printf("Query user failed ,user  not exist! send msg success!\an");
		}
		sqlite3_free_table(result);





	}

	return 0;
}

int do_admin_login(int acceptfd, msg_t *msg , sqlite3 *db)
{
	char sql[128] = {};
	char *errmsg;
	int nrow;
	int ncloumn;
	char **result;
	sprintf(sql, "select * from info where usr = '%s' and password = '%s' and pri = 'manager';", msg->usr, msg->password);
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
		printf("get_table ok!\n");
	}

	// 查询成功，数据库中拥有此用户
	if(nrow == 1)
	{
		msg->status = OK;
		send(acceptfd, msg, sizeof(msg_t), 0);
		printf("Query manager success,manager  exist! send msg success!\n");
	}

	if(nrow == 0) // 密码或者用户名错误
	{
		msg->status = NO;
		send(acceptfd, msg, sizeof(msg_t), 0);
		printf("Query manager failed,manager not  exist! send msg success!\n");
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
		printf("没有记录\n");
		sendmsg.status=NO;
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
	printf("send msg success！\n");

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


