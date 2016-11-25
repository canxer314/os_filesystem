#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "filesys.h"
/*  gid δ��ֵ
    uid ��500��ʼ
    �����û�δ����
*/
int signIn()
{
    int iCountS = 0;                    //������
    char caUname[8][USERNAMESIZ];       //���������ʱ��
    int iSignFlag = 0;                  //��ע���־
    int iSignMark;                      //pwd 8�����ĸ�������ע��
    int iUidMax = 1;                    //uid���� 0Ϊ�����û� �����
    fseek(fd, 0, SEEK_SET );
    for(iCountS = 0; iCountS < 8; iCountS++)
    {
        if(fread(&pwd[iCountS], sizeof(struct pawd), 1, fd) != 1)
        {
            /* fread ʧ�� */
            printf("Check User and Password Error!\n");
            return -1;
        }
        strcpy(caUname[iCountS], pwd[iCountS].username);

        /* uid = ��ǰ���ֵ */
        if(pwd[iCountS].p_uid > iUidMax)
            iUidMax = pwd[iCountS].p_uid;

        if((strcmp(pwd[iCountS].username, "") == 0)&& (strcmp(pwd[iCountS].password, "") == 0))
        {
            if(iSignFlag != 1)
                printf("You can Sign Up\n");
            iSignMark = iCountS;
            iSignFlag = 1;
        }
    }
    if(iSignFlag == 0)
    {
        /* �û�����8�� */
        printf("User Is Full!!\n");
        return 0;
    }
    else
    {
        /* Sign In */
        struct pawd spTempPwd;
        iSignFlag = 0;
        while(iSignFlag == 0)
        {
            printf("Please enter new UserName:");
            scanf("%s",spTempPwd.username);
            for(iCountS = 0; iCountS < 8 ; iCountS++)
            {
                if(strcmp(spTempPwd.username, caUname[iCountS]) == 0)
                {
                    printf("The WserName has been occupied!\n");
                    break;
                }
            }
            if(iCountS == 8)
                iSignFlag = 1;
        }
        while(1)
        {
            printf("Please enter Password:");
            strcpy(spTempPwd.password, GetPasswd());
            printf("Please repeat the password:");
            char caTempP[PWDSIZ];
            strcpy(caTempP, GetPasswd());
            //printf("%s\n",caTempP);
            if(strcmp(caTempP,spTempPwd.password)==0)
            {
                printf("Password Matching!\n");
                break;
            }
            else
            {
                printf("Two input password does not match!\n");
            }
        }
        if(iUidMax != 0 && iUidMax <500)
            spTempPwd.p_uid = 500;
        else
            spTempPwd.p_uid = iUidMax+1;
        /* save to filesystem */
        pwd[iSignMark] = spTempPwd;
        fseek(fd, 0, SEEK_SET );
        for(iCountS = 0 ; iCountS <8 ; iCountS++)
        {
            fwrite(&pwd[iCountS],sizeof(pawd),1,fd);
        }
        printf("Registered successfully!\n");

    }
}


