#include <stdio.h>
#include "filesys.h"
#include <stdlib.h>
#include <string.h>

int format()
{
    static unsigned short fileblock = FILEBLK;//512
    //int nicfreeblock = NICFREE;//50
    //int nicinhod = NICINOD;//31
    //int remaining_block = ( FILEBLK + 1 ) % NICINOD;
    dinode_free difree;
    //printf( "%ld", sizeof( dinode_free ) );
    block_free b_free;

    fd = fopen( "filesystem.db", "wb" );
    if ( fd == NULL )
    {
        printf( "ERROR£¡ Can not create filesystem!!" );
        exit( 1 );//ÖÐÖ¹³ÌÐò
    }
    //void * tmp = malloc( ( 2 + DINODEBLK + FILEBLK ) * BLOCKSIZ );
    char * tmp[ ( 2 + DINODEBLK + FILEBLK ) * BLOCKSIZ ];
    /*if ( tmp == NULL )
       {
            printf( "\nTry creating filesystem file, but failed! \n" );
            exit( 0 );
       }*/
    fwrite( tmp, 2 + DINODEBLK + FILEBLK, BLOCKSIZ, fd );

    fs.s_isize = FILEBLK;
    fs.s_fsize = FILEBLK;
    fs.s_nfree = FILEBLK;
    fs.s_ninode = FILEBLK;
    fs.s_fmod = 'n';

    fseek( fd, ( 2 + DINODEBLK ) * BLOCKSIZ, SEEK_SET );

    if ( fileblock >= NICINOD )
    {
        fseek( fd, -NICINOD * DINODESIZ, SEEK_CUR );
        difree.dinode_Free[ 0 ] = 0;
        int i;
        for ( i = 1; i < NICINOD; i++ )
        {
            difree.dinode_Free[ i ] = fileblock;
            fileblock--;
        }
        difree.dinode_count = NICINOD - 1;
        fwrite( &difree, sizeof( dinode_free ), 1, fd );
        fseek( fd, -DINODESIZ, SEEK_CUR );
    }

    while ( fileblock >= NICINOD )
    {
        fseek( fd, -NICINOD * DINODESIZ, SEEK_CUR );
        for (  int i = 0; i < NICINOD; i++ )
        {
            difree.dinode_Free[ i ] = fileblock;
            fileblock--;
        }
        difree.dinode_count = NICINOD - 1;
        fwrite( &difree, sizeof( dinode_free ), 1, fd );
        fseek( fd, -DINODESIZ, SEEK_CUR );
    }

    if ( fileblock == 0 )
    {
        fs.s_pinode = difree.dinode_count;
        int i;
        for (  i = 0; i < NICINOD; i++ )
            fs.s_inode[ i ] = difree.dinode_Free[ i ];
    }
    else
    {
        fs.s_pinode = -1;
        int j = fileblock;
        int i;
        for (  i = 0; i < j; i++ )
        {
            fs.s_pinode++;
            fs.s_inode[ i ] = fileblock;
            fileblock--;
        }
    }

    fileblock = FILEBLK;
    fseek( fd, 0, SEEK_END );
    if ( fileblock >= NICFREE )
    {
        fseek( fd, -NICFREE * BLOCKSIZ, SEEK_CUR );
        b_free.blk_free[ 0 ] = 0;
        int i;
        for (  i = 1; i < NICFREE; i++ )
        {
            b_free.blk_free[ i ] = fileblock;
            fileblock--;
        }
        b_free.blk_count = NICFREE - 1;
        fwrite( &b_free, sizeof( block_free ), 1, fd );
        fseek( fd, -sizeof( block_free ), SEEK_CUR );
    }

    while ( fileblock >= NICFREE )
    {
        int i;
        fseek( fd, -NICFREE * BLOCKSIZ, SEEK_CUR );
        for ( i = 0; i < NICFREE; i++ )
        {
            b_free.blk_free[ i ] = fileblock;
            fileblock--;
        }
        b_free.blk_count = NICFREE - 1;
        fwrite( &b_free, sizeof( block_free ), 1, fd );
        fseek( fd, -sizeof( block_free ), SEEK_CUR );
    }

    if ( fileblock == 0 )
    {
        fs.s_pfree = b_free.blk_count;
        int i;
        for (  i = 0; i < NICFREE; i++ )
            fs.s_free[ i ] = b_free.blk_free[ i ];
    }
    else
    {
        fs.s_pfree = -1;
        int j = fileblock;
        int i;
        for (  i = 0; i < j; i++ )
        {
            fs.s_pfree++;
            fs.s_free[ i ] = fileblock;
            fileblock--;
        }
    }
    /*fseek( fd, blocksiz, seek_set );
    fwrite( &fs, sizeof( filesys ), 1, fd );*/

    unsigned short tempinode_no;
    char * temp;
    //dir dir_new, *dir_new1;
    tempinode_no = ialloc();
    //balloc();
    iformat(tempinode_no);
    direct firstdirect, seconddirect;
    strcpy(firstdirect.d_name, ".");
    firstdirect.d_ino = tempinode_no;
    strcpy(seconddirect.d_name, "..");
    seconddirect.d_ino = tempinode_no;
    dir_dinode[tempinode_no].di_size =DIRNUM*sizeof(direct)+sizeof(int);
    dir_dinode[tempinode_no].di_uid = 0;
    dir_dinode[tempinode_no].di_gid = 0;
    dir_dinode[tempinode_no].di_mode = UDIREAD + UDIWRITE + UDIEXICUTE + GDIREAD;
    dir_dinode[tempinode_no].di_number = 1;
    strcpy(dir_new.direct[0].d_name, firstdirect.d_name);
    strcpy(dir_new.direct[1].d_name, seconddirect.d_name);
    dir_new.direct[0].d_ino = firstdirect.d_ino;
    dir_new.direct[1].d_ino = seconddirect.d_ino;
    dir_new.size = 2;
    temp = (char *)&dir_new;
    write(tempinode_no, temp, sizeof(dir));
    iput(tempinode_no, dir_dinode);

    active_inode = tempinode_no;
    temp = (char *)&working_dir;
    read(active_inode, temp,sizeof(dir));

    fseek( fd, BLOCKSIZ, SEEK_SET );
    fwrite( &fs, sizeof( filesys ), 1, fd );
    printf( "The disk has been formated successfully!\n" );

    fclose( fd );

    return 0;
}
