#include "filesys.h"
#include "string.h"

#define ROOT_DIR_INODE 1

extern dir dir_new;
extern dir * dir_newp;
int ad_dir( int dinode_no, char * temp );
int rm_dir( int dinode_no, char * temp );
char path_file[ 20 ][ DIRSIZ ];
int divide_time;

void read_to_dir( int inode_no, dir * dir_tmp )
{
    char * tmp;
    tmp = ( char * )dir_tmp;
    read( inode_no, tmp, sizeof( dir ) );//读入一个dir结构体文件到dir_new
}

//将path分割到path_file,并返回分割了多少次
int path_divide( char * path )
{
    int count = 0;
    char * p;
    p = strtok( path, "\\" );
    while ( p )
    {
        strcpy( path_file[ count ], p );
        count++;
        p = strtok( NULL, "\\" );
    }
    return count;
}

//检查该i节点对应的目录文件下是否有名为filename的文件
//找到返回名字相同的目录项对应文件的i节点号，否则返回0
int check_file( int inode, char * filename )
{
    int node = 0;
    read_to_dir( inode, &dir_new );
    for ( int i = 2; i < dir_new.size; i++ )
        if ( strcmp( dir_new.direct[ i ].d_name, filename ) == 0 )
        {
            node = dir_new.direct[ i ].d_ino;
            break;
        }
    return node;
}

//最后一个char不是‘\\’则path是文件，返回0
//若不是，则path就是目录，返回1，path为空返回-1
int check_path_or_file( char * path )
{
    int count = 0;
    if ( path[ count ] == '\0' )
    {
        printf( "ERROR! EMPTY PATH.\n" );
        return -1;
    }
    //if ( path[ count ] != '\\' )
    //return 0;
    count++;
    while ( path[ count ] != '\0' )
    {
        count++;
    }
    count--;
    if ( path[ count ] == '\\' )
        return 1;
    else
        return 0;
}

//从根目录开始找，若path最终是一个文件，返回该文件所在目录的inode
//若path最终是一个目录，就返回它的inode
//若path路径错误，返回0
int check_path( char * path )
{
    int node;
    if ( path[ 0 ] != '\\' )
    {
        printf( "ERROR! root path no exist.\n" );
        return 0;
    }
    else
    {
        int flag;
        flag  = check_path_or_file( path );
        int count = path_divide( path );
        divide_time = count;
        node = ROOT_DIR_INODE;
        for ( int i = 0; i < count - 2; i++ )//count == 4,循环0~2
        {
            node = check_file( node, path_file[ i ] );
            if ( node == 0 )
                return 0;
        }
        if ( flag == 1 )//如果是目录，则再读一遍
        {
            node = check_file( node, path_file[ count - 1 ] );
            if ( node == 0 )
                return 0;
        }
        printf("%d\n",node);
        return node;
    }
}

int copyfile( int dir_inode, char * srcfile, char * destpath )
{
    //if ( access( check_file( dir_inode, srcfile ), 'r' ) == 0 )
    if (0)
    {
        printf( "ERROR! NO access permission.\n" );
        return 0;
    }
    else
    {
        //检查当前源目录文件名
        int node = check_file( dir_inode, srcfile );
        //检查目标目录文件名
        if ( node != 0 )//找到名为srcfile的文件
        {
            if ( check_path_or_file( destpath ) == 1 )//path
            {
                char tmp[ NADDR * BLOCKSIZ ];
                int inode;
                int tmp_node;
                int dest_dir_node = check_path( destpath );
                if ( dest_dir_node == 0 )
                {
                    printf( "ERROR! NO SUCH PATH EXIST.\n" );
                    return -1;
                }
                inode = ialloc();
                //iformat(inode);
                fseek( fd, 2 * BLOCKSIZ + ( node - 1 ) * DINODESIZ, SEEK_SET );
                fread( &dir_dinode[ inode ], sizeof( dinode ), 1, fd );
                read( node, tmp, dir_dinode[ inode ].di_size );
                write( inode, tmp, dir_dinode[ inode ].di_size );
                iput( inode, dir_dinode );
                tmp_node = active_inode;
                read_to_dir( dest_dir_node, &working_dir );
                active_inode = dest_dir_node;
                ad_dir( inode, srcfile );
                read_to_dir( tmp_node, &working_dir );
                active_inode = tmp_node;
                return 1;
            }
            if ( check_path_or_file( srcfile ) == 0 )//file
            {
                printf( "ERROR! NO PATH WAS SPECIFIC.\n" );
                return -1;
            }
        }
        else
        {
            printf( "ERROR! NO SUCH FILE ON THE DISK.\n" );
            return -1;
        }
    }
}

int move_file( int dir_inode, char * srcfile, char * destpath )
{
    //if ( access( check_file( dir_inode, srcfile ), "w" ) == 0 )
    if (0)
    {
        printf( "ERROR! NO access permission.\n" );
        return 0;
    }
    else
    {
        //检查当前源目录文件名
        int node = check_file( dir_inode, srcfile );
        //检查目标目录文件名
        if ( node != 0 )//找到名为srcfile的文件
        {
            if ( check_path_or_file( destpath ) == 1 )//path
            {
                int tmp_node;

                int dest_dir_node = check_path( destpath );
                if ( dest_dir_node == 0 )
                {
                    printf( "ERROR! NO SUCH PATH EXIST.\n" );
                    return -1;
                }
                tmp_node = active_inode;
                delete_dir( node, srcfile );

                read_to_dir( dest_dir_node, &working_dir );
                active_inode = dest_dir_node;
                ad_dir( node, srcfile );

                read_to_dir( tmp_node, &working_dir );
                active_inode = tmp_node;
                return 1;
            }
            if ( check_path_or_file( srcfile ) == 0 )//file
            {
                printf( "ERROR! NO PATH WAS SPECIFIC.\n" );
                return -1;
            }
        }
        else
        {
            printf( "ERROR! NO SUCH FILE ON THE DISK.\n" );
            return -1;
        }
    }
}
//当前目录的inode
int linkfile( int inode, char * srcfile, char * linkfile )
{
    //if ( access( check_file( inode, srcfile ), "w" ) == 0 )
    if (0)
    {
        printf( "ERROR! NO access permission.\n" );
        return 0;
    }
    else
    {
        //检查当前源目录文件名
        int node = check_file( inode, srcfile );
        //检查目标目录文件名
        if ( node != 0 )//找到名为srcfile的文件
        {
            if ( check_path_or_file( linkfile ) == 1 )//path
            {
                printf( "ERROR! no specific the linkname\n" );
                return -1;
            }
            if ( check_path_or_file( linkfile ) == 0 )//file
            {
                printf("Done\n");
                int dir_node;
                int tmp_node = active_inode;
                dir_node = check_path( linkfile );
                read_to_dir( dir_node, &working_dir );
                active_inode = dir_node;
                //working_dir.direct[ dir_new.size ].d_name = path_file[ divide_time - 1 ];
                //working_dir.direct[ dir_new.size ].d_ino = active_inode;
                //working_dir.size++;
                ad_dir( node,  srcfile);
                read_to_dir( tmp_node, &working_dir );
                active_inode = tmp_node;
                return 1;
            }
        }
        else
        {
            printf( "ERROR! NO SUCH FILE ON THE DISK.\n" );
            return -1;
        }
    }
}

int re_name( int dir_node, char * oldname, char * newname )
{
    int flag;
    flag = check_file( dir_node, oldname );
    if ( flag == 0 )
    {
        printf( "ERROR! can not find the file." );
        return -1;
    }
    strcpy( working_dir.direct[ flag].d_name, newname );
    char * temp;
    temp=&working_dir;
    write(working_dir.direct[0].d_ino,temp,sizeof(dir));
    iput( dir_node,dir_dinode);
    return 1;
}
