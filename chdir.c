#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "filesys.h"
#include <time.h>

int chdir()
{
    if(strcmp(caCommand[1],"")==0)
    {
        printf("跳转的路径不能为空！");
        return 0;
    }
    else if(strcmp(caCommand[1],".")==0)
    {
        return 1;
    }
    else if(strcmp(caCommand[1],"..")==0)
    {
        if(path_len>1)
        {
            strcpy(path_dir[path_len--],"");
            char *temp;
            temp=&working_dir;
            active_inode=working_dir.direct[1].d_ino;
            read(working_dir.direct[1].d_ino,temp,sizeof(dir));
            return 1;
        }

    }
    else
    {
        for(int i=2; i<working_dir.size; i++)
        {
            if((strcmp(caCommand[1],working_dir.direct[i].d_name)==0)&&(dir_dinode[working_dir.direct[i].d_ino].di_mode&DIDIR)!=0)
            {
                strcpy(path_dir[path_len++],caCommand[1]);
                char *temp;
                temp=&working_dir;
                active_inode=working_dir.direct[i].d_ino;
                read(working_dir.direct[i].d_ino,temp,sizeof(dir));
                return 1;
            }
        }
    }
    printf("您所要跳转的目录不存在！\n");
    return 0;
}
