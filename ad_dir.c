#include"filesys.h"
int ad_dir(int dinode, char *tempname)
{
    direct firstdirect;
    dir  dir_newtemp;
    char *temp;
    temp = (char *)&dir_new;
    read(active_inode, temp, sizeof(dir));
    strcpy(firstdirect.d_name, tempname);
    dir_new.size++;
    strcpy(dir_new.direct[dir_new.size - 1].d_name, firstdirect.d_name);
    dir_new.direct[dir_new.size - 1].d_ino = dinode;//ÐÂÔö½Úµã±àºÅ
    write(active_inode, temp, sizeof(dir));
    iput(active_inode, dir_dinode);
}
int delete_dir(int dinode, char *tempname)
{
    dir dir_delete;
    char *temp;
    temp = &dir_delete;
    read(working_dir.direct[0].d_ino, temp, sizeof(dir));//将当前工作目录读入dir_delete
    if ((dir_dinode[dinode].di_mode&DIFILE) != 0)  //如果所要删除节点是一个文件节点，就将其指向该文件删除
    {
        for (int i = 0; i < dir_delete.size; i++)
        {
            if (dir_delete.direct[i].d_ino == dinode)
            {
                for (int j = i; j < dir_delete.size-1; j++)
                {
                    strcpy(dir_delete.direct[j].d_name, dir_delete.direct[j + 1].d_name);//将要删除的目录项之后的目录项向前移动一项
                    dir_delete.direct[j].d_ino = dir_delete.direct[j + 1].d_ino;
                }
                dir_delete.size--;//目录项数减一
                for (int i = 0; i < NADDR; i++)
                {
                    if (dir_dinode[dinode].di_addr[i] != 0)
                    {
                        bfree(dir_dinode[dinode].di_addr[i]);//释放该文件占用的磁盘块
                        dir_dinode[dinode].di_addr[i] = 0;
                    }
                }
                iput(dinode, dir_dinode);
                ifree(dinode);//释放该文件对应的i节点
                write(dir_delete.direct[0].d_ino,(char *)&dir_delete,sizeof(dir));
                iput(dir_delete.direct[0].d_ino, dir_dinode);
                return 1;//成功删除文件
            }
        }
    }
    if ((dir_dinode[dinode].di_mode&DIDIR) != 0)  //如果要删除的节点是目录
    {
        temp = &working_dir;//进入要删除的目录，删除该目录内的所有项
        read(dinode, temp, sizeof(dir));
        for (int i = 2; i < working_dir.size; i++)  //删除要删除目录的子目录的所有项,从第三项开始删除，（前两项分别是“.”，“..”）
        {
            delete_dir(working_dir.direct[i].d_ino,working_dir.direct[i].d_name);
        }
        read(dir_delete.direct[1].d_ino,temp,sizeof(dir));//重新将working_dir指回上级目录
        for (int i = 0; i < dir_delete.size; i++)
        {
            if (dir_delete.direct[i].d_ino == dinode)
            {
                for (int j = i; j < dir_delete.size - 1; j++)
                {
                    strcpy(dir_delete.direct[j].d_name, dir_delete.direct[j + 1].d_name);//将要删除的目录项之后的目录项向前移动一项
                    dir_delete.direct[j].d_ino = dir_delete.direct[j + 1].d_ino;
                }
                dir_delete.size--;//目录项数减一
                for (int i = 0; i < NADDR; i++)
                {
                    if (dir_dinode[dinode].di_addr[i] != 0)
                    {
                        bfree(dir_dinode[dinode].di_addr[i]);//释放该目录占用的磁盘块
                        dir_dinode[dinode].di_addr[i] = 0;
                    }
                }
                iput(dinode,dir_dinode);
                ifree(dinode);//释放该目录对应的i节点
                write(dir_delete.direct[0].d_ino, (char *)&dir_delete, sizeof(dir));
                iput(dir_delete.direct[0].d_ino, dir_dinode);
                return 1;//成功删除文件
            }
        }
    }
}
