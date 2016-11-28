#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "filesys.h"
#include <time.h>
int createdir(char *dirname)
{
    direct firstdirect, seconddirect;
    dir  dir_newtemp;
    char *temp;
    if(strcasecmp(caCommand[2],"")==0)
    {
        printf("Ŀ¼������Ϊ�գ������½���Ŀ¼��\n");
        return 0;
    }
    int tempinode_no=ialloc();
    if ((dir_dinode[active_inode].di_mode&DIFILE) != 0)
    {
        active_inode = working_dir.direct[0].d_ino;//�����ǰ��ڵ���һ���ļ��ڵ㣬�ͽ���ָ����ļ�������Ŀ¼�ڵ�
    }
    temp = (char *)&dir_new;
    read(active_inode,temp,sizeof(dir));
    strcpy(firstdirect.d_name, caCommand[2]);
    firstdirect.d_ino = tempinode_no;//�����ڵ���
    dir_new.size++;
    strcpy(dir_new.direct[dir_new.size - 1].d_name, firstdirect.d_name);
    dir_new.direct[dir_new.size - 1].d_ino = firstdirect.d_ino;
    write(active_inode, temp,sizeof(dir));
    iput(active_inode, dir_dinode);
    //����
    temp = (char *)&dir_newtemp;
    read(active_inode,temp,sizeof(dir));
    dir dir_new2;
    //Ϊ�½���Ŀ¼��ӵ�ǰĿ¼����һ��Ŀ¼
    strcpy(firstdirect.d_name, ".");
    firstdirect.d_ino = tempinode_no;//��Ŀ¼�ĵ�ǰĿ¼���Լ����Ľڵ���
    strcpy(seconddirect.d_name, "..");
    seconddirect.d_ino = active_inode;
    dir_dinode[tempinode_no].di_size= DIRNUM * sizeof(direct) + sizeof(int);//�½���Ŀ¼�Ѿ���������
    dir_dinode[tempinode_no].di_uid = pwd[iSignLogin].p_uid;
    dir_dinode[tempinode_no].di_gid = pwd[iSignLogin].p_gid;
    dir_dinode[tempinode_no].di_time = time((time_t*)NULL);
    for (int i = 0; i <NADDR; i++)
    {
        dir_dinode[tempinode_no].di_addr[i] = 0;
    }
    dir_dinode[tempinode_no].di_mode = DIDIR + UDIREAD + UDIWRITE + UDIEXICUTE+GDIREAD;//��Ȩ
    dir_dinode[tempinode_no].di_number = 1;
    strcpy(dir_new2.direct[0].d_name, firstdirect.d_name);
    strcpy(dir_new2.direct[1].d_name, seconddirect.d_name);
    dir_new2.direct[0].d_ino = firstdirect.d_ino;
    dir_new2.direct[1].d_ino = seconddirect.d_ino;
    dir_new2.size = 2;
    temp = (char *)&dir_new2;
    write(tempinode_no, temp, sizeof(dir));
    iput(tempinode_no, dir_dinode);
    //����
    temp = (char *)&dir_newtemp;
    read(tempinode_no, temp, sizeof(dir));
    return tempinode_no; //��Ŀ¼��i�����
}
