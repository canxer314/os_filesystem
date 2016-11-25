#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include "filesys.h"
#include "time.h"

char passwd[PWDSIZ + 1] = "";

char *GetPasswd()
{
    unsigned char c;
    int i = 0;

    while ((c=getch())!='\r')
    {
        if (i<PWDSIZ && isprint(c))
        {
            passwd[i++] = c;
            putchar('*');
        }
        else if (i>0 && c=='\b')
        {
            --i;
            putchar('\b');
            putchar(' ');
            putchar('\b');
        }
    }
    putchar('\n');
    passwd[i] = '\0';

    return passwd;
}

char *getSmall(char command[20])
{
    int i = 0;
    char smallCommand[20] = "";
    //strcpy(smallCommand , "");
    //smallCommand[20] = 0;
    for(i = 0; i< strlen(command) && command[i] != '\0'; i++)
    {
        if(command[i]> 'A' && command[i]< 'Z')
            smallCommand[i] = command[i] - ('A' - 'a');
        else
            smallCommand[i] = command[i];
    }
    return smallCommand;
}

int show()
{
    int iCountP;
    fseek(fd, 0, SEEK_SET );
    printf("num\t\tUID\t\tGID\t\tusername\tpassword\n");
    for(iCountP = 0; iCountP < 8; iCountP++)
    {
        if(fread(&pwd[iCountP], sizeof(struct pawd), 1, fd) != 1)
        {
            /* fread 失败 */
            printf("Check User and Password Error!\n");
            return -1;
        }
        printf("%d\t\t%d\t\t%d\t\t%s\t\t%s\n",iCountP,pwd[iCountP].p_uid,pwd[iCountP].p_uid,pwd[iCountP].username,pwd[iCountP].password);
    }
}


int writeToca()
{
    memset(caTempFile,0,sizeof(caTempFile));
    char cTemp;
    int i = 0;
    while( (cTemp = getchar()) != '$')
    {
        caTempFile[i++] = cTemp;
    }
    //printf("%s\nstelen:%d\n%d\nsizeof%d\n",caTempFile,strlen(caTempFile),i,sizeof(caTempFile));
    //memcpy();
    return i;
}

int help()
{
    //printf("- - - - - - - - - - - - - - - - - - - - -\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * \n");
    printf(" exit   - - - - - - - -   \texit file system\n");
    printf(" signin   - - - - - - - \tsign in a new user\n");
    printf(" sign   - - - - - - - -   \tequal to 'signin'\n");
    printf(" login    - - - - - - -     \tuser land \n");
    printf(" delete   - - - - - - -      \tdelete + * \n");
    printf("     user   - - - - - -   \tdelete a user\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * \n");

}

// 检索文件名 最后返回该文件的节点号

int searchdir(char *temp)
{
    if(strcmp (temp, "") == 0)
    {
        printf("Please input the content!!\n");
        return 0;
    }
    for( int i = 0 ; i < working_dir.size ; i++)
    {
        //printf("dir  %s  %s\n",working_dir.direct[i].d_name, temp);
        if( strcmp(working_dir.direct[i].d_name, temp) == 0)
        {
            //printf("测试输出 找到的相匹配的i结点: %d\n",working_dir.direct[i].d_ino );
            return working_dir.direct[i].d_ino ;
        }
    }
    //遍历结束 未找到匹配的name
    printf("No matching content!\n");
    return 0;

}

int showdir()
{

    struct tm *local;
    int iDino;
    printf("- - - - - - - - - - - - - - - - - - - - - \n");
    char *change;
    change=(char *)&working_dir;
    read(active_inode,change, sizeof(dir));
    printf(" %d  \n",active_inode);
    for( int i = 0 ; i < working_dir.size ; i++)
    {
        iDino = working_dir.direct[i].d_ino ;
        local=localtime(&dir_dinode[iDino].di_time);
        //是文件
        //printf("%d\n",dir_dinode[iDino].di_mode);
        printf("   inode : %d\n",iDino);
        if( (dir_dinode[iDino].di_mode & DIFILE) != 0)
            printf("%d/%2d/%2d %2d:%2d\t<FILE>\t\t%s\n",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,working_dir.direct[i].d_name);
        else
            printf("%d/%2d/%2d %2d:%2d\t<DIR>\t\t%s\n",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,working_dir.direct[i].d_name);
        //printf("%F",local);

    }
    //遍历结束 未找到匹配的name

    return 0;

}

