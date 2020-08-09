

/*****************************************
*结构体填充定义
*****************************************/

/*******命令定义********************/
#define RIGSTER_MANAGER  "RM"  /*注册管理员    */
#define REGISTER_USR     "RU"  /*注册用户      */
#define LOG_MANAGER      "LM"  /*管理员登录    */
#define LOG_USR          "LU"  /*用户登录      */
#define INQUIRE_INFO     "II"  /*查询信息,管理员和用户查询逻辑不同*/
#define VERIFY_PASSWORD  "VP"  /*修改密码      */
#define VERIFY_INFO      "VI"  /*修改信息      */
#define INQUIRE_HISTORY  "IH"  /*查询历史记录  */
#define ADD_USR          "AI"  /*增加用户信息  */
#define DELECT_USR       "DU"  /*删除用户信息  */

/************性别定义********************/
#define MAN              "M"
#define WONMAN           "W"
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

struct msg{
	char cmd[2];       //命令
	char usr[10];      //用户名
	char pri;          //权限
    char name[10];     //姓名
	int age;           //年龄
	char sex;          //性别（"M"/"W"）
	char password[10]; //密码
	int number;        //工号
	int salary;        //薪水
	char dept[10];     //部门
	int inq_mode;      //查询模式
	int inq_cnt;       //查询个数
	int status;        //返回
}msg;
