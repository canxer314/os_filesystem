/*
Author:     canxer

time:       7/13

function :  i�ڵ㺯��
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "filesys.h"


int ialloc()//����i�ڵ�
{
		int ret;
		//extern filesys fs;
		if ( fs.s_pinode == 0 )
		{
				if ( fs.s_inode[ 0 ] == 0 )
						printf( "����" );
				else
				{
						ret = fs.s_inode[ 0 ];
						fseek( fd, ( 2 * BLOCKSIZ + ret * DINODESIZ ) ,SEEK_SET);
						fseek(fd,sizeof(unsigned short),SEEK_CUR);
						fread(fs.s_pinode)
				}
		}
		else
		{
				ret = fs.s_inode[ fs.s_pinode ];
				fs.s_pinode--;
				return ret;
		}
		printf( "%ld\n", fs.s_fsize );
		return 0;
}

int ifree()
{
		return 0;
}

int iget()//�ڴ�i�ڵ�
{
		return 0;
}

int iput()
{
		return 0;
}

int balloc()//���̿����
{
		return 0;
}

int bfree()
{
		return 0;
}
*/
