/*
Author:     canxer

time:       7/13

function :  i节点函数
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "filesys.h"


int ialloc()//磁盘i节点
{
		int ret;
		//extern filesys fs;
		if ( fs.s_pinode == 0 )
		{
				if ( fs.s_inode[ 0 ] == 0 )
						printf( "错误" );
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

int iget()//内存i节点
{
		return 0;
}

int iput()
{
		return 0;
}

int balloc()//磁盘块分配
{
		return 0;
}

int bfree()
{
		return 0;
}
*/
