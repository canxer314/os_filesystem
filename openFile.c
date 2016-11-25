#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "filesys.h"
#include <time.h>

/*
    ���ļ���iInode�ڵ���ļ�
*/
int readfile(int iInode)
{
    /* �ж��Ƿ��½ */
    if(iFlagLogin == 1)
    {
        /* �ж��Ƿ����ļ� */
        if( (dir_dinode[iInode].di_mode & DIFILE) != 0 )
        {
            /* �ж��ļ�Ȩ��  access (iInode ,'r' ) == */
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
    /* �ж��Ƿ��½ */
    if(iFlagLogin == 1)
    {
        /* �ж��Ƿ����ļ� */
        if( (dir_dinode[iInode].di_mode & DIFILE) != 0 )
        {
            /* �ж��ļ�Ȩ�� */
            if (readfile(iInode) == 1 )
            {
                if( access(iInode, 'w') == 1)
                {
                    // ��catemp2 �������Ѷ�ȡ��Ϣ
                    memset(caTempFile2, 0, sizeof(caTempFile2));
                    strcpy(caTempFile2,caTempFile);
                    int iLen = writeToca();
                    if(iLen + strlen(caTempFile2) <= (BLOCKSIZ*10))
                    {
                        /*
                        //�ͷ� ���̿�
                        for(int i = 0 ;i< 10 ;i++)
                        {
                            bfree(dir_dinode[iInode].di_addr[i]);
                        }
                        */
                        //�����ַ���
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

 if(��д�ļ�Ȩ�� && a == 1)
                {
                    int iLenAdd = writeToca();
                    if( iLenAdd + dir_dinode[iInode].di_size <= NADDR * BLOCKSIZ)
                    {

                        //����+����С��һ����
                        if( (iLenAdd + dir_dinode[iInode].di_size % 512 <= BLOCKSIZ ) && ( dir_dinode[iInode].di_size % 512 == 0) )
                        {
                            //read �ѽ��� fseek

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

                        //�������ļ���
                        printf("The text you add is too long! Can'n add the end at file!\n");
                    }
                }
*/

