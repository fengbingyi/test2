

/*****************************************
*结构体填充定义
*****************************************/

/*******命令定义********************/
#define REGISTER_MANAGER  1  /*注册管理员    */
#define REGISTER_USR      2  /*注册用户      */
#define LOG_MANAGER       3  /*管理员登录    */
#define LOG_USR           4  /*用户登录      */
#define INQUIRE_INFO      5  /*查询信息,管理员和用户查询逻辑不同*/
#define VERIFY_PASSWORD   6  /*修改密码      */
#define VERIFY_INFO       7  /*修改信息      */
#define INQUIRE_HISTORY   8  /*查询历史记录  */
#define ADD_USR           9  /*增加用户信息  */
#define DELECT_USR        10 /*删除用户信息  */

/************性别定义********************/
#define MANAGER          "manager"
#define USER             "user"

/************性别定义********************/
#define MAN              'M'
#define WONMAN           'W'
//************部门定义*******************/
#define DEPT1            1
#define DEPT2            2
#define DEPT3            3
#define DEPT4            4
/*********查询方式定义（inq_mode）*******/
#define ALL_INFO      1  //查询所有信息
#define FIR_TEN_INFO  2  //查询前10条
#define LST_TEN_INFO  3  //查询后10条
#define LST_FIFTY_HIS 4  //查询后50条
#define SEL_INFO      5  //查询选定条数的信息
/********************************/
#define OK  1
#define NO  0

typedef struct mymsg{
	
	char usr[20];       //用户名
	char pri[10];           //权限
    char name[20];      //姓名
	char sex[10];           //性别（"M"/"W"）
	char password[20];  //密码
	char dept[20];      //部门
	int  cmd;           //命令
	int  age;           //年龄
	int  number;        //工号
	int  salary;        //薪水
	int  inq_mode;      //查询模式
	int  inq_cnt;       //查询个数
	int  status;        //返回
}msg_t;

/*定义一个默认的变量供客户端测试使用*/
msg_t msg = {
	.cmd         = LOG_USR,
	.usr[0]      = 'N',
	.pri         = 'U',
	.name[0]     = 'N',     
    .age         = 18,         
    .sex         = MAN,          
    .password[0] = 'N', 
    .number      = 1,       
    .salary      = 10000,       
    .dept[0]     = 'N',     
    .inq_mode    = ALL_INFO,     
    .inq_cnt     = 10,      
    .status      = 1,
};
