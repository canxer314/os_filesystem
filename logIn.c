#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "filesys.h"
/*
使用 username 和 password 匹配 的模式
uid 500以上随机
*/

int logIn()
{
    int iCountP = 0;

    /* 输入用户名 密码 */
    struct pawd spTempPwd;
    printf("Please enter a Userame:");
    //scanf("%d",&spTempPwd.p_uid);
    scanf("%s",spTempPwd.username);
    printf("Please enter Password:");
    strcpy(spTempPwd.password, GetPasswd());
    //printf("%s\n",spTempPwd.password);
    //无需检测初始用户组

    /* 查找 引导块 块是否有密码 */
    fseek(fd, 0, SEEK_SET );

    for(iCountP = 0; iCountP < 8; iCountP++)
    {
        if(fread(&pwd[iCountP], sizeof(struct pawd), 1, fd) != 1)
        {
            /* fread 失败 */
            printf("Check User and Password Error!\n");
            return -1;
        }
        if((strcmp(pwd[iCountP].username, spTempPwd.username) == 0) && (strcmp(pwd[iCountP].password, spTempPwd.password) == 0))
        {
            printf("%d Matching\n",iCountP);
            iSignLogin = iCountP;
            iFlagLogin = 1;
        }
    }
    /* 登陆成功 */
    if(iFlagLogin == 1)
        return 1;
    if(iCountP == 8)
    {
        /* 未找到匹配账户或密码 */
        printf("User Name Or Password ERROR!!\n");
        return 0;
    }
}

int logout()
{
    if(iFlagLogin == 1)
    {
        printf("Are you sure you want to exit?! ( Y / N )\n");
        char c;
        scanf("%c",&c);
        if(c == 'Y' || c == 'y')
        {
            iFlagLogin = 0;
            printf("Logged out successfully!\n");
            return 1;
        }
        else
        {
            printf("Cancel the operation!\n");
            return 0;
        }
    }
    else
    {
        printf("you should login at first !\n");
        return 0;
    }
}
