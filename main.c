#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "filesys.h"
#include <time.h>
int main()
{
    char *temp;
    fd=fopen ("filesystem.db", "rb+");
    path_len=1;
    if(fd == NULL)
    {
        printf("\nNot find filesystem, format the disk \n");
        char fm;
        printf("\nY/N \n");
        scanf("%c", &fm);
        if ((fm == 'Y')|| (fm == 'y'))
        {
            format();
            fd=fopen ("filesystem.db", "rb+");
        }
        else
        {
            exit(0);
        }
    }
    else
    {
        printf("\nfind filesystem,open system. \n");
    }

    /*  打开超级块 */
    fseek( fd, BLOCKSIZ, SEEK_SET );
    fread(&fs, sizeof(filesys), 1, fd);
    if(fs.s_isize != 0)
    {
        printf("超级块已加载！\n");
    }
    else
    {
        printf("超级块加载失败！\n");
    }

    //inode 加载
    for(int i = 0 ;  i < FILEBLK ; i ++)
    {
        iget(i, &dir_dinode);
        strcpy(path_dir[i],"");
    }
    strcpy(path_dir[0],"root");
    active_inode = 1;//初始化当前活动节点和活动目录
    temp = (char *)&working_dir;
    read(1, temp, sizeof(dir));


    //Sleep(1000);
    int iComCount = 0;
    while(command() == 1)   //switch 不支持 字符串
    {
        //printf("%s\n",caCommand[iComCount]);

        if(strcmp(getSmall(caCommand[iComCount]), "t") == 0)
        {
            searchdir("..");
        }

        else if(strcmp(getSmall(caCommand[iComCount]), "exit") == 0)
        {
            fseek( fd, BLOCKSIZ, SEEK_SET );
            fwrite( &fs, sizeof( filesys ), 1, fd );
            for(int i = 0 ;  i < FILEBLK ; i ++)
            {
                iput(i, &dir_dinode);
            }
            fclose(fd);
            printf("ByeBye!\n");
            exit(0);
        }
        else if(strcmp(getSmall(caCommand[iComCount]), "format") == 0)
        {
            if(fd!=NULL)fclose(fd);
            printf("\nFormat the disk?\n");
            char fm;
            printf("\nY/N \n");
            scanf("%c", &fm);
            if ((fm == 'Y')|| (fm == 'y'))
            {
                format();
                fd=fopen ("filesystem.db", "rb+");
                /*  打开超级块 */
                fseek( fd, BLOCKSIZ, SEEK_SET );
                fread(&fs, sizeof(filesys), 1, fd);
                if(fs.s_isize != 0)
                {
                    printf("超级块已加载！\n");
                }
                else
                {
                    printf("超级块加载失败！\n");
                }
                //inode 加载
                for(int i = 0 ;  i < FILEBLK ; i ++)
                {
                    iget(i, &dir_dinode);
                }
                active_inode = 1;//初始化当前活动节点和活动目录
                temp = (char *)&working_dir;
                read(1, temp, sizeof(dir));
            }
        }
        else if(strcmp(getSmall(caCommand[iComCount]), "login") == 0)
        {
            if(iFlagLogin == 0 )
                logIn();
            else
            {
                printf("You have already landed!\n");
            }
        }

        else if(strcmp(getSmall(caCommand[iComCount]), "logout") == 0)
        {
            if(iFlagLogin == 1 )
                logout();
            else
            {
                printf("You need login at first!\n");
            }
        }

        /*else if(strcmp(getSmall(caCommand[iComCount])  , "delete") == 0)
        {
            if(strcmp(getSmall(caCommand[iComCount+1])  , "user") == 0)
                deleteuser();
            else
            {
                printf("please enter available command!\n");
            }
        }*/
        else if(strcmp(getSmall(caCommand[iComCount]), "delete") == 0)
        {
            if(iFlagLogin == 1)
            {
                if(strcmp(getSmall(caCommand[iComCount+1]), "user") == 0)
                    deleteuser();
                if(strcmp(getSmall(caCommand[iComCount+1]), "file") == 0)
                    deletefile();
                else if(strcmp(getSmall(caCommand[iComCount+1]), "dir") == 0)
                {
                    deletedir(caCommand[2]);
                }
                else
                    printf("\'%s\' isn't available command!\n",caCommand[1]);
            }
            else
            {
                printf("Please Login At First!\n");
            }
        }
        else if(strcmp(getSmall(caCommand[iComCount]), "help") == 0)
        {
            help();
        }

        else if(strcmp(getSmall(caCommand[iComCount]), "signin") == 0)
        {
            signIn();
        }

        else if(strcmp(getSmall(caCommand[iComCount]), "sign") == 0)
        {
            signIn();
        }

        else if(strcmp(getSmall(caCommand[iComCount]), "show") == 0)
        {
            show();
        }

        else if(strcmp(getSmall(caCommand[iComCount]), "mkdir") == 0)
        {
            createdir(caCommand[2]);
        }

        else if(strcmp(getSmall(caCommand[iComCount]), "dir") == 0)
        {
            showdir();
        }

        else if(strcmp(getSmall(caCommand[iComCount]), "cd") == 0)
        {
            chdir();
        }
        else if(strcmp(getSmall(caCommand[iComCount]), "ls") == 0)
        {
            printf("%d",fs.s_nfree);
        }
        else if(strcmp(getSmall(caCommand[iComCount]), "copy") == 0)
        {
            copyfile(active_inode,caCommand[1],caCommand[2]);
        }
        else if(strcmp(getSmall(caCommand[iComCount]), "link") == 0)
        {
            linkfile(active_inode,caCommand[1],caCommand[2]);
        }
        else if(strcmp(getSmall(caCommand[iComCount]), "move") == 0)
        {
            move_file(active_inode,caCommand[1],caCommand[2]);
        }
        else if(strcmp(getSmall(caCommand[iComCount]), "rn") == 0)
        {
            re_name(active_inode,caCommand[1],caCommand[2]);
        }
        else if(strcmp(getSmall(caCommand[iComCount]), "create") == 0)
        {
            if(strcmp(getSmall(caCommand[iComCount+1]), "file") == 0)
                creatfile();
            else if(strcmp(getSmall(caCommand[iComCount+1]), "dir") == 0)
            {
                createdir(caCommand[2]);
            }
            else
                printf("\'%s\' isn't available command!\n",caCommand[1]);

        }

        else if(strcmp(getSmall(caCommand[iComCount]), "read") == 0)
        {
            if(strcmp(getSmall(caCommand[iComCount+1]), "file") == 0)
                readfile(searchdir(caCommand[iComCount + 2]));
            else
                printf("\'%s\' isn't available command!\n",caCommand[1]);

        }

        else if(strcmp(getSmall(caCommand[iComCount]), "write") == 0)
        {
            if(strcmp(getSmall(caCommand[iComCount+1]), "file") == 0)
                writefile(searchdir(caCommand[iComCount + 2]));
            else
                printf("\'%s\' isn't available command!\n",caCommand[1]);

        }

        else if(strcmp(getSmall(caCommand[iComCount]), "chmod") == 0)
        {


        }

        else if(strcmp(getSmall(caCommand[iComCount]), "chgrp") == 0)
        {


        }

        else if(strcmp(caCommand[iComCount], "") == 0)
        {
            continue;
        }

        else
        {
            printf("\'%s\' isn't available command!\n",caCommand[0]);
        }

        /* clean */
        strcpy(caCommand[0], "");

    }



    return 0;
}
