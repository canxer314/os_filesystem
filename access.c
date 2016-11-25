#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include "filesys.h"

int access(int iInode, char c)
{
    if(c == 'r')
    {
        if(dir_dinode[iInode].di_uid == pwd[iSignLogin].p_uid)
        {
            if( (dir_dinode[iInode].di_mode & UDIREAD ) != 0 )
                return 1;
            else
                return 0;
        }

        else if (dir_dinode[iInode].di_gid == pwd[iSignLogin].p_gid)
        {
            if( (dir_dinode[iInode].di_mode & GDIREAD ) != 0 )
                return 1;
            else
                return 0;
        }

        else if( (dir_dinode[iInode].di_mode & ODIREAD ) != 0 )
        {
            return 1;
        }
        else
            return 0;
    }
    else if( c == 'w')
    {
        if(dir_dinode[iInode].di_uid == pwd[iSignLogin].p_uid)
        {
            if( (dir_dinode[iInode].di_mode & UDIWRITE ) != 0 )
                return 1;
            else
                return 0;
        }

        else if (dir_dinode[iInode].di_gid == pwd[iSignLogin].p_gid)
        {
            if( (dir_dinode[iInode].di_mode & GDIWRITE ) != 0 )
                return 1;
            else
                return 0;
        }

        else if( (dir_dinode[iInode].di_mode & ODIWRITE ) != 0 )
        {
            return 1;
        }
        else
            return 0;
    }
    else
    {
        printf("command error!\n");
        return 0;
    }

}

int grantAccess(int iInode, char iden, char acc)
{
    if(iden == 'g')
    {
        if(acc == 'r')
        {
            dir_dinode[iInode].di_mode += GDIREAD;
            printf("Grant successful!\n");
            return 1;
        }
        else if(acc == 'w')
        {
            dir_dinode[iInode].di_mode += GDIWRITE;
            printf("Grant successful!\n");
            return 1;
        }
        else
        {
            printf("command error!\n");
            return 0;
        }
    }
    else if(iden == 'o')
    {
        if(acc == 'r')
        {
            dir_dinode[iInode].di_mode += ODIREAD;
            printf("Grant successful!\n");
            return 1;
        }
        else if(acc == 'w')
        {
            dir_dinode[iInode].di_mode += ODIWRITE;
            printf("Grant successful!\n");
            return 1;
        }
        else
        {
            printf("command error!\n");
            return 0;
        }
    }
    else
    {
        printf("command error!\n");
        return 0;
    }
}

int changeFileGid(int iInode, int newGid)
{
    dir_dinode[iInode].di_gid = newGid;
    printf("Change Gid successfully!\n");
    return 1;
}

