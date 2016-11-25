#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "filesys.h"
#include <time.h>

/*
    读文件，iInode节点的文件
*/
int readfile(int iInode)
{
    /* 判断是否登陆 */
    if(iFlagLogin == 1)
    {
        /* 判断是否是文件 */
        if( (dir_dinode[iInode].di_mode & DIFILE) != 0 )
        {
            /* 有读文件权限  access (iInode ,'r' ) == */
            if(1)
            {
                memset(caTempFile, 0, sizeof(caTempFile));
                if( read(iInode, caTempFile, dir_dinode[iInode].di_size) )
                {
                    printf("You have the permission to read the file!\nThe content of the file is\n");
                    printf(" - - - - - - - - - - - - - - - - - - - -\n");
                    printf("%s\n",caTempFile);
                    printf(" - - - - - - - - - - - - - - - - - - - -\n");
                    return 1;
                }
                else
                {
                    printf("read Error!\n");
                    return 0 ;
                }


            }
            else
            {
                printf("You do not have permission to read the file!\n");
                return -2;
            }


        }
        else
        {
            printf("%s is not a file!\n", caCommand[2]);
            return -1;
        }

    }
    else
    {
        printf("Please Login At First!\n");
        return 0;
    }
}
int writefile(int iInode)
{
    /* 判断是否登陆 */
    if(iFlagLogin == 1)
    {
        /* 判断是否是文件 */
        if( (dir_dinode[iInode].di_mode & DIFILE) != 0 )
        {
            /* 有读文件权限 */
            if (readfile(iInode) == 1 )
            {
                if( access(iInode, 'w') == 1)
                {
                    // 用catemp2 来保存已读取信息
                    memset(caTempFile2, 0, sizeof(caTempFile2));
                    strcpy(caTempFile2,caTempFile);
                    int iLen = writeToca();
                    if(iLen + strlen(caTempFile2) <= (BLOCKSIZ*10))
                    {
                        /*
                        //释放 磁盘块
                        for(int i = 0 ;i< 10 ;i++)
                        {
                            bfree(dir_dinode[iInode].di_addr[i]);
                        }
                        */
                        //连接字符串
                        strcat(caTempFile2, caTempFile);
                        write(iInode, caTempFile2,strlen(caTempFile2));
                        dir_dinode[iInode].di_size = strlen(caTempFile2);
                        return 1;
                    }
                    else
                    {
                        printf("The Add is too long!\n");
                    }

                }
                else
                {
                    printf("You do not have permission to read the file!\n");
                    return -2;
                }
            }
            else
            {
                printf("You do not have permission to read the file!\n");
                return -2;
            }



        }
        else
        {
            printf("%s is not a file!\n");
            return -1;
        }

    }
    else
    {
        printf("Please Login At First!\n");
        return 0;
    }
}
/* old read
                int iCountB = 1;
                if(dir_dinode[iInode].di_size % 512 == 0)
                {
                    iCountB += dir_dinode[iInode].di_size/512 - 1;
                }
                else
                    iCountB += dir_dinode[iInode].di_size/512;

                if(iCountB == 0)
                {
                    printf("This is a empty file, can't open!\n");
                }
                else
                {

                    ZeroMemory(caTempFile,sizeof(caTempFile));
                    for(int i = 0 ; i < iCountB ; i ++)
                    {
                        fseek(fd, DATASTART + dir_dinode[iInode].di_addr[i] * BLOCKSIZ ,SEEK_SET);
                        memcpy(caTempFile[i * BLOCKSIZ], fd ,BLOCKSIZ);
                    }
*/
/*

 if(有写文件权限 && a == 1)
                {
                    int iLenAdd = writeToca();
                    if( iLenAdd + dir_dinode[iInode].di_size <= NADDR * BLOCKSIZ)
                    {

                        //输入+已有小于一个块
                        if( (iLenAdd + dir_dinode[iInode].di_size % 512 <= BLOCKSIZ ) && ( dir_dinode[iInode].di_size % 512 == 0) )
                        {
                            //read 已进行 fseek

                        }
                        int iCountBW = 1;
                        if( dir_dinode[iInode].di_size % 512 == 0)
                        {
                            iCountBW +=  dir_dinode[saCount].di_size/512 - 1;
                        }
                        else
                        {

                        }


                    }
                    else
                    {

                        //可增加文件名
                        printf("The text you add is too long! Can'n add the end at file!\n");
                    }
                }
*/

