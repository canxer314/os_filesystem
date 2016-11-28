#include <stdio.h>
#include "filesys.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

int creatfile()
{
    if(iFlagLogin == 1)//�ѵ�¼
    {
        int iLen = 0;
        int iCountB = 1;    //�ܹ����ٿ�
        int iCountI;        //�����i�ڵ��
        char caFileName[20];

        int seconds = time((time_t*)NULL);
        //printf("%d\t%s\n",seconds,ctime(&seconds));

        /* �ļ��� */
        strcpy(caFileName, caCommand[2]);

        /* �������� */
        iLen = writeToca();

        ///��ȡһ��i�ڵ� ��ʽ�� ��ȡ
        iCountI = ialloc();
        iformat(iCountI);
        int active_inode = iCountI;


        /* dir_dinode[active_inode].i_count = 1;    //���ʼ���
           dir_dinode[active_inode].i_flag = 'Y'; //��ʹ��
           dir_dinode[active_inode].i_ino = iCountI;*/

        dir_dinode[active_inode].di_mode =DIFILE + UDIREAD + UDIWRITE +UDIEXICUTE; //������Ȩ��
        dir_dinode[active_inode].di_gid = pwd[iSignLogin].p_gid;
        dir_dinode[active_inode].di_uid = pwd[iSignLogin].p_uid;
        dir_dinode[active_inode].di_time = seconds;
        dir_dinode[active_inode].di_size = iLen;
        dir_dinode[active_inode].di_number = 1;  //�����ļ�����
        /*
          if(dir_dinode[active_inode].di_size % 512 == 0)
          {
          iCountB += dir_dinode[active_inode].di_size/512 - 1;
          }
          else
          iCountB += dir_dinode[active_inode].di_size/512;

          for(int i = 0; i< iCountB ; i++)
          {
          dir_dinode[active_inode].di_addr[i] = balloc();
          //�����������ݽ�ȥ
          fseek(fd, DATASTART + dir_dinode[active_inode].di_addr[i] * BLOCKSIZ ,SEEK_SET);
          memcpy(fd , caTempFile[i * BLOCKSIZ] , BLOCKSIZ);
          }
        */

        write(iCountI, caTempFile, dir_dinode[active_inode].di_size );
        // printf("create file inode :%d ",iCountI );
        iput(iCountI, dir_dinode);
        //���²�����Ŀ¼
        ad_dir(iCountI, caFileName);

        return iCountI;
    }
    else
    {
        printf("Please Login At First!\n");
        return 0 ;
    }
}

int write(int iInode, char * tmp, int size)
{
    int iCountB = 1;
    for(int i = 0 ; i<10 ; i++)
    {
        bfree(dir_dinode[iInode].di_addr[i]);
    }
    if(size % 512 == 0)
    {
        iCountB += size/512 - 1;
    }
    else
        iCountB += size/512;

    if(iCountB == 0)
    {
        printf("This is a empty file!\n");
    }
    else
    {
        for(int i = 0 ; i < iCountB ; i ++)
        {
            dir_dinode[iInode].di_addr[i] = balloc();
            /* �����������ݽ�ȥ */
            fseek(fd, DATASTART + (dir_dinode[iInode].di_addr[i]-1) * BLOCKSIZ,SEEK_SET);
            //printf("write block %d\t\n",dir_dinode[iInode].di_addr[i]);
            fwrite(tmp + i * BLOCKSIZ, BLOCKSIZ, 1, fd);
        }
    }
}

int read(int iInode, char * tmp, int size)
{

    int iCountB = 1;
    //printf("iCountB :%d\n",iCountB);
    if(size % 512 == 0)
    {
        iCountB += size/512 - 1;
    }
    else
        iCountB += size/512;

    if(iCountB == 0)
    {
        printf("This is a empty file!\n");
        return 0;
    }
    else
    {
        // printf("for qian");
        for(int i = 0 ; i < iCountB -1; i ++)
        {
            fseek(fd, DATASTART + (dir_dinode[iInode].di_addr[i]-1) * BLOCKSIZ,SEEK_SET);
            //printf("read block %d\t\t\n",dir_dinode[iInode].di_addr[i]);
            fread(tmp + i * BLOCKSIZ, BLOCKSIZ, 1, fd);
        }
        fseek(fd, DATASTART + (dir_dinode[iInode].di_addr[iCountB -1 ]-1) * BLOCKSIZ,SEEK_SET);
        //printf("fd:%d\n");
        //printf("read block %d\t\t\n",dir_dinode[iInode].di_addr[iCountB -1]);
        fread(tmp + ( iCountB -1 ) * BLOCKSIZ, size % BLOCKSIZ, 1, fd);
        return 1;
    }
}
