#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "filesys.h"

int deleteuser()
{
    int iCountD = 0;
    if(iFlagLogin == 1) //已登录 用户信息保存在pwd数组中
    {
        printf("This is You User Information:\n");
        printf("- - - - - - - - - - - - - - - - - - - - -\n");
        printf("UserID\t\tUserName\t\n");
        printf("%d\t\t%s\n",pwd[iSignLogin].p_uid,pwd[iSignLogin].username);
        printf("- - - - - - - - - - - - - - - - - - - - -\n");
        printf("Are you sure delete your user account  ( Y / N or others ) ? \n");
        char cC;
        if((cC = getch()) == 'Y' || cC == 'y')
        {
            /* pwd delete */
            pwd[iSignLogin].p_gid = 0;
            pwd[iSignLogin].p_uid = 0;
            strcpy(pwd[iSignLogin].password, "");
            strcpy(pwd[iSignLogin].username, "");
            /* 文件保存 */
            fseek(fd, 0, SEEK_SET );
            for(iCountD = 0 ; iCountD <8 ; iCountD++)
            {
                fwrite(&pwd[iCountD],sizeof(pwd),1,fd);
            }

            iFlagLogin = 0;

            printf("Delete User  successfully!\n");
            return 1;
        }
        else
        {
            printf("Operation has been cancelled!\n");
            return 0;
        }
    }
    else
    {
        printf("//You need login !\n");
        return 0;
    }
}


int deletefile()
{
    for(int i=0; i<working_dir.size; i++)
    {
        if(strcmp(caCommand[2],working_dir.direct[i].d_name)==0&&(dir_dinode[working_dir.direct[i].d_ino].di_mode&DIFILE)!=0)
        {
            if((dir_dinode[working_dir.direct[i].d_ino].di_mode&(UDIWRITE|GDIWRITE|ODIWRITE)!=0))
            {
                delete_dir(working_dir.direct[i].d_ino,caCommand[2]);
                return 1;
            }
            else
            {
                printf("您没有删除该文件的权限");
                return 0;
            }
        }
    }
    printf("没有此文件");
    return 0;
}

int deletedir()
{
    for(int i=0; i<working_dir.size; i++)
    {
        if((strcmp(caCommand[2],working_dir.direct[i].d_name)==0)&&(dir_dinode[working_dir.direct[i].d_ino].di_mode&DIDIR)!=0)
        {
            if((dir_dinode[working_dir.direct[i].d_ino].di_mode&(UDIWRITE|GDIWRITE|ODIWRITE)!=0))
            {
                delete_dir(working_dir.direct[i].d_ino,caCommand[2]);
                return 1;
            }
            else
            {
                printf("您没有删除该文件的权限");
                return 0;
            }
        }
    }
    printf("没有此文件");
    return 0;
}

