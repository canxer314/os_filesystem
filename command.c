#include <stdio.h>
#include <stdlib.h>
//#include <windows.h>
#include "filesys.h"

int command()
{
    char cCommand[300];
    const char *cFlagSeg = " ,\,";
    char *p;
    iCommandCount = 0; //������ʼ��
    int iJ = 0; //�� ��ʼ��
    printf("> ");
    for(int i=0; i<path_len; i++)
    {
        printf("/%s",path_dir[i]);
    }
    printf(":");
    gets(cCommand);
    if(strcmp(cCommand, "") == 0)
        gets(cCommand);
    //printf("%s",cCommand);
    p = strtok(cCommand, cFlagSeg);
    while(p)
    {
        //printf("%s\n",p);
        strcpy(caCommand[iCommandCount],p);
        iCommandCount++;
        p=strtok(NULL,cFlagSeg);
    }

    /* command�������
       int i = iCommandCount;
       int temp = i;
       printf("%d\n\n",iCommandCount);
       while(i)
       {

       printf("%s\n",caCommand[temp - i]);
       i--;
       }
    */

    return 1;
}
