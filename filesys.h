#ifndef FILESYS_H_INCLUDED
#define FILESYS_H_INCLUDED

/*
   ��ʼ��ʱ���൱�ڴ�i�ڵ㿪ʼ��ʼд �� distart��ʼ�� ��ûд���ĵط���ʼд��ȥ
   hinode ��ϣ�� �ڵ㶼�������ڵ�ָ�� hash�������i�ڵ㶼������

   temp
   1.�Ƚ���һ��i�ڵ� ָ����������Ŀ¼ 012...���û� ��ʼ�����û�������
   2.forѭ�� ������  ����50�� �������ӷ�

   ÿ��i�ڵ��Ӧ����һ����
   i�ڵ� ������  i_addr������������ �������������10  ÿ����������ֻ��һ������� ����һ���ļ���һi�ڵ� һ��i�ڵ�ʮ���������� Ҳ����ʮ�������
   �ļ���С ���10��  10*512�ֽ�=5K
   512��i�ڵ� �� 512�����Ӧ
 **/
/*  file function

   size_t fread ( void *buffer, size_t size, size_t count, FILE *stream) ;
   �� ��
   buffer
   ���ڽ������ݵ��ڴ��ַ
   size
   Ҫ����ÿ����������ֽ�������λ���ֽ�
   count
   Ҫ��count�������ÿ��������size���ֽ�.
   stream
   ������

   size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream);
   ע�⣺��������Զ�������ʽ���ļ����в��������������ı��ļ�
   ����ֵ������ʵ��д������ݿ���Ŀ
   ��1��buffer����һ��ָ�룬��fwrite��˵����Ҫ��ȡ���ݵĵ�ַ��
   ��2��size��Ҫд�����ݵĵ��ֽ�����
   ��3��count:Ҫ����д��size�ֽڵ�������ĸ�����
   ��4��stream:Ŀ���ļ�ָ�룻
   ��5������ʵ��д������������count��

   size_t fread( void * buffer , size_t size , size_t count , FILE * stream );
   ������ �ܣ�
   ������һ���ļ����ж�����,��ȡ count��Ԫ�� ,ÿ��Ԫ�� size�ֽ�.������óɹ����� ֵ����count���粻�ɹ���
    ����ʵ�ʶ�ȡ��Ԫ�ظ�����С��count.
   �� ��
   ����buffer
   �������ڽ������ݵ��ڴ��ַ,��С������ size* count�ֽ�.
   ����size
   ��������Ԫ�صĴ�С����λ���ֽ�
   ����count
   ����Ԫ�صĸ���,ÿ��Ԫ����size�ֽ�.
   ����stream
   ����������
 */

/* pwd + user = 64 * 8 = 512 һ��
 */


#include <stdio.h>
#define BLOCKSIZ 512   //ÿ���ֽ���
#define SYSOPENFILE 40 //�ļ�ϵͳ���ļ�����������
#define DIRNUM 63 //ÿ���ļ�Ŀ¼�����������Ŀ¼�������ļ�����
#define DIRSIZ 14 //ÿ��Ŀ¼�����ֲ�����ռ�ֽ��������i�ڵ��2���ֽ�
#define USERNAMESIZ 16//�û�����С
#define PWDSIZ 12 //������
#define PWDNUM 32 //������32�������¼
#define NOFILE 20 //ÿ���û�����ɴ�20���ļ������û����ļ���������
#define NADDR 10   //ÿ��i�ڵ����ָ��10�飬add[0]~add[9]
#define NHINO 127  //��128��Hash�����ṩ����i�ڵ�(����Ϊ2����)
#define USERNUM 10 //�������10���û���¼
#define DINODESIZ 64             //ÿ������i�ڵ���ռ�ֽ�
#define DINODEBLK 64             //���д��̽ڵ㹲ռ64�������
#define FILEBLK 512              //����512���ļ������
#define NICFREE 50               //�������п��п���������ֵ
#define NICINOD 31               //�������п��нڵ�����ֵ
#define DINODESTART 2 * BLOCKSIZ //i�ڵ���ʼ��ַ
#define DATASTART ( 2 + DINODEBLK ) * BLOCKSIZ //Ŀ¼���ļ�����ʼ��ַ
/*di_node��ȡȨ��*/
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
#define DISKFULL 65535 //������
#define SEEK_SET 0     //�׵�ַ,fseek origin

typedef struct inode   //�����ڵ�
{
    struct inode * i_forw;
    struct inode * i_back;
    char i_flag;    //״̬ 'Y'��ʾ�ѱ�ʹ��
    unsigned int i_ino; //�����ڵ���
    unsigned int i_count; //���ʼ���
    unsigned short di_uid;
    unsigned short di_gid;
    unsigned short di_mode;    //��ȡȨ��,��ȡȨ�����һ��ָʾ�ļ��Ƿ�ΪĿ¼�ļ����磺drwxrw-r-- 1Ϊ�ļ� 0ΪĿ¼
    unsigned long di_size; //�ļ���С
    unsigned short di_number; //�����ļ�������Ϊ0ʱ����ɾ�����ļ�
    unsigned long di_time;
    unsigned int di_addr[ NADDR ]; //������
} inode;

typedef struct dinode   //���������ڵ�64�ֽڣ���512��i�ڵ㣬��64��
{
    unsigned short di_uid;
    unsigned short di_gid;
    unsigned short
    di_mode;    //��ȡȨ��,��ȡȨ�����һ��ָʾ�ļ��Ƿ�ΪĿ¼�ļ����磺drwxrw-r--
    unsigned long di_size; //�ļ���С
    unsigned short di_number; //�����ļ�������Ϊ0ʱ����ɾ�����ļ�
    unsigned long di_time;
    unsigned int di_addr[ NADDR ]; //������
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

typedef struct direct          //i�ڵ�ָ��� �����ڿ��е�Ŀ¼��ṹ
{
    char d_name[ DIRSIZ ]; //Ŀ¼��
    unsigned short d_ino; //Ŀ¼��
} direct;

typedef struct filesys                    //������427�ֽڣ�����512�ֽ�
{
    unsigned short s_isize;  //�����ڵ���
    unsigned long s_fsize;   //���ݿ����
    unsigned int s_ninode;   //���������ڵ���
    unsigned int s_nfree;    //���п����
    unsigned short s_pfree;  //���п�ָ�� ջ��
    unsigned int s_free[ NICFREE ]; //���п��ջ
    unsigned short s_pinode; //���������ڵ�ָ��
    unsigned short s_inode[ NICINOD ]; //���������ڵ�����
    //unsigned int s_rinode;   //���������ڵ�
    char s_fmod;             //�������޸ı�־
} filesys;

typedef struct pawd   //�û�����
{
    unsigned short p_uid;
    unsigned short p_gid;
    char username[ USERNAMESIZ ];
    char password[ PWDSIZ ];
} pawd;

typedef struct dir   //Ŀ¼
{
    int size;
    struct direct direct[ DIRNUM ];
} dir;

typedef struct hinode   //�����ڴ������ڵ��hash��
{
    struct inode * i_forw;
} hinode;

typedef struct file              //ϵͳ�򿪱�
{
    char f_flag;     //�ļ�������־
    unsigned int f_count; //���ü���
    struct inode * f_inode; //ָ���ڴ������ڵ�
    unsigned long f_off; //��дָ��
} file;

typedef struct user
{
    unsigned short u_default_mode;
    unsigned short u_uid;     //�û���־
    unsigned short u_gid;     //�û����־
    unsigned short u_ofile[ NOFILE ]; //�û����ļ���
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
char caTempFile[NADDR * BLOCKSIZ]; //�����ļ�ʱ����������
char caTempFile2[NADDR * BLOCKSIZ]; //�����ļ�ʱ����������
int active_inode;

int format();
int chdir();
int write(int iInode, char * tmp, int size);
int read(int iInode, char * tmp, int size);
int ad_dir(int dinode_no, char *temp);
#endif //FILESYS_H_INCLUDED
