#ifndef FILESYS_H_INCLUDED
#define FILESYS_H_INCLUDED

/*
   初始化时候相当于从i节点开始开始写 从 distart开始找 到没写过的地方开始写进去
   hinode 哈希表 节点都是索引节点指针 hash表吧所有i节点都穿起来

   temp
   1.先建立一个i节点 指向数据区主目录 012...存用户 初始化把用户都建立
   2.for循环 超级块  链接50个 成组链接法

   每个i节点对应后面一个块
   i节点 主索引  i_addr【】二级索引 二级索引块最大10  每个二级索引只想一个物理块 所以一个文件有一i节点 一个i节点十个二级索引 也就是十个物理块
   文件大小 最多10块  10*512字节=5K
   512个i节点 和 512个块对应
 **/
/*  file function

   size_t fread ( void *buffer, size_t size, size_t count, FILE *stream) ;
   参 数
   buffer
   用于接收数据的内存地址
   size
   要读的每个数据项的字节数，单位是字节
   count
   要读count个数据项，每个数据项size个字节.
   stream
   输入流

   size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream);
   注意：这个函数以二进制形式对文件进行操作，不局限于文本文件
   返回值：返回实际写入的数据块数目
   （1）buffer：是一个指针，对fwrite来说，是要获取数据的地址；
   （2）size：要写入内容的单字节数；
   （3）count:要进行写入size字节的数据项的个数；
   （4）stream:目标文件指针；
   （5）返回实际写入的数据项个数count。

   size_t fread( void * buffer , size_t size , size_t count , FILE * stream );
   　　功 能：
   　　从一个文件流中读数据,读取 count个元素 ,每个元素 size字节.如果调用成功返回 值大于count。如不成功，
    返回实际读取的元素个数，小于count.
   参 数
   　　buffer
   　　用于接收数据的内存地址,大小至少是 size* count字节.
   　　size
   　　单个元素的大小，单位是字节
   　　count
   　　元素的个数,每个元素是size字节.
   　　stream
   　　输入流
 */

/* pwd + user = 64 * 8 = 512 一块
 */


#include <stdio.h>
#define BLOCKSIZ 512   //每块字节数
#define SYSOPENFILE 40 //文件系统打开文件表的最大项数
#define DIRNUM 63 //每个文件目录所包含的最大目录项数（文件数）
#define DIRSIZ 14 //每个目录项名字部分所占字节数，另加i节点号2个字节
#define USERNAMESIZ 16//用户名大小
#define PWDSIZ 12 //口令字
#define PWDNUM 32 //最多可设32个口令登录
#define NOFILE 20 //每个用户做多可打开20个文件，即用户打开文件的最大次数
#define NADDR 10   //每个i节点最多指向10块，add[0]~add[9]
#define NHINO 127  //共128个Hash链表，提供索引i节点(必须为2的幂)
#define USERNUM 10 //最多允许10个用户登录
#define DINODESIZ 64             //每个磁盘i节点所占字节
#define DINODEBLK 64             //所有磁盘节点共占64个物理块
#define FILEBLK 512              //共有512个文件物理块
#define NICFREE 50               //超级块中空闲块数组的最大值
#define NICINOD 31               //超级块中空闲节点的最大值
#define DINODESTART 2 * BLOCKSIZ //i节点起始地址
#define DATASTART ( 2 + DINODEBLK ) * BLOCKSIZ //目录，文件区起始地址
/*di_node存取权限*/
#define DIEMPTY 0x0000
#define DIFILE 0x1000
#define DIDIR 0x2000
/*user*/
#define UDIREAD 0x0001
#define UDIWRITE 0x0002
#define UDIEXICUTE 0x0004
/*group*/
#define GDIREAD 0x0010
#define GDIWRITE 0x0020
#define GDIEXICUTE 0x0040
#define ODIREAD 0x0100
#define ODIWRITE 0x0200
#define ODIEXICUTE 0x0400
/*other*/
#define READ 1
#define WRITE 2
#define EXICUTE 3
#define DEFAULTMODE 00777
#define IUPDATE 00002 /*i_flag*/
#define SUPDATE 00001 /*s_fmod*/
/*f_flag*/
#define FREAD 00001
#define FWRITE 00002
#define FAPPEND 00004
#define DISKFULL 65535 //磁盘满
#define SEEK_SET 0     //首地址,fseek origin

typedef struct inode   //索引节点
{
    struct inode * i_forw;
    struct inode * i_back;
    char i_flag;    //状态 'Y'表示已被使用
    unsigned int i_ino; //索引节点编号
    unsigned int i_count; //访问计数
    unsigned short di_uid;
    unsigned short di_gid;
    unsigned short di_mode;    //存取权限,存取权限里第一个指示文件是否为目录文件。如：drwxrw-r-- 1为文件 0为目录
    unsigned long di_size; //文件大小
    unsigned short di_number; //关联文件数，当为0时，则删除该文件
    unsigned long di_time;
    unsigned int di_addr[ NADDR ]; //物理块号
} inode;

typedef struct dinode   //磁盘索引节点64字节，共512个i节点，共64块
{
    unsigned short di_uid;
    unsigned short di_gid;
    unsigned short
    di_mode;    //存取权限,存取权限里第一个指示文件是否为目录文件。如：drwxrw-r--
    unsigned long di_size; //文件大小
    unsigned short di_number; //关联文件数，当为0时，则删除该文件
    unsigned long di_time;
    unsigned int di_addr[ NADDR ]; //物理块号
} dinode;


typedef struct dinode_free
{
    unsigned short dinode_count;
    unsigned short dinode_Free[ NICINOD ];
} dinode_free;

typedef struct block_free
{
    unsigned int blk_count;
    unsigned int blk_free[ NICFREE ];
} block_free;

typedef struct direct          //i节点指向的 保存在块中的目录项结构
{
    char d_name[ DIRSIZ ]; //目录名
    unsigned short d_ino; //目录号
} direct;

typedef struct filesys                    //超级块427字节，分配512字节
{
    unsigned short s_isize;  //索引节点数
    unsigned long s_fsize;   //数据块块数
    unsigned int s_ninode;   //空闲索引节点数
    unsigned int s_nfree;    //空闲块块数
    unsigned short s_pfree;  //空闲块指针 栈深
    unsigned int s_free[ NICFREE ]; //空闲块堆栈
    unsigned short s_pinode; //空闲索引节点指针
    unsigned short s_inode[ NICINOD ]; //空闲索引节点数组
    //unsigned int s_rinode;   //铭记索引节点
    char s_fmod;             //超级块修改标志
} filesys;

typedef struct pawd   //用户密码
{
    unsigned short p_uid;
    unsigned short p_gid;
    char username[ USERNAMESIZ ];
    char password[ PWDSIZ ];
} pawd;

typedef struct dir   //目录
{
    int size;
    struct direct direct[ DIRNUM ];
} dir;

typedef struct hinode   //查找内存索引节点的hash表
{
    struct inode * i_forw;
} hinode;

typedef struct file              //系统打开表
{
    char f_flag;     //文件操作标志
    unsigned int f_count; //引用计数
    struct inode * f_inode; //指向内存索引节点
    unsigned long f_off; //读写指针
} file;

typedef struct user
{
    unsigned short u_default_mode;
    unsigned short u_uid;     //用户标志
    unsigned short u_gid;     //用户组标志
    unsigned short u_ofile[ NOFILE ]; //用户打开文件表
} user;

/* command count */
int iCommandCount;
/* command array */
char caCommand[ 10 ][ 20 ];
struct pawd pwd[ PWDNUM ];
int iFlagLogin;
int iSignLogin;
FILE * fd;
filesys fs;
dir sDir;
dir dir_new;
char path_dir[FILEBLK][DIRSIZ];
int path_len;
dir working_dir;
dinode dir_dinode[FILEBLK];
char caTempFile[NADDR * BLOCKSIZ]; //创建文件时候的最大输入
char caTempFile2[NADDR * BLOCKSIZ]; //创建文件时候的最大输入
int active_inode;

int format();
int chdir();
int write(int iInode, char * tmp, int size);
int read(int iInode, char * tmp, int size);
int ad_dir(int dinode_no, char *temp);
#endif //FILESYS_H_INCLUDED
