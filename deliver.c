#include"filesys.h"
int ialloc()//磁盘i节点
{
    int ret;
    if (fs.s_pinode == 0)
    {
        if (fs.s_inode[0] == 0)
            printf("ERROR! Inode has been used out.\n");
        else
        {
            dinode_free di_free;
            ret = fs.s_inode[0];
            /*printf("%d_", fs.s_inode[fs.s_pinode]);
            printf("%d \n", fs.s_pinode);*/
            fseek(fd, (2 * BLOCKSIZ + (ret - 1) * DINODESIZ), SEEK_SET);
            fread(&di_free, sizeof(dinode_free), 1, fd);
            for (int i = 0; i < NICINOD; i++)
                fs.s_inode[i] = di_free.dinode_Free[i];
            fs.s_pinode = di_free.dinode_count;
            fs.s_ninode--;
        }
    }
    else
    {
        ret = fs.s_inode[fs.s_pinode];
        /*printf("%d_", fs.s_inode[fs.s_pinode]);
        printf("%d\n", fs.s_pinode);*/
        fs.s_pinode--;
        fs.s_ninode--;
    }
    return ret;
}

void ifree(int block_no)
{
    if (fs.s_pinode == NICINOD - 1)
    {
        dinode_free di_free;
        fseek(fd, (2 * BLOCKSIZ + (block_no - 1) * DINODESIZ), SEEK_SET);
        for (int i = 0; i < NICINOD; i++)
            di_free.dinode_Free[i] = fs.s_inode[i];
        di_free.dinode_count = fs.s_pinode;
        fwrite(&di_free, sizeof(dinode_free), 1, fd);
        fs.s_pinode = 0;
        fs.s_inode[fs.s_pinode] = block_no;
    }
    else
    {
        fs.s_pinode++;
        fs.s_inode[fs.s_pinode] = block_no;
        fs.s_ninode++;
    }
}

int iget(int block_no, dinode * m_inode)//分配内存i节点
{
    dinode di_free;
    fseek(fd, (2 * BLOCKSIZ) + (block_no - 1)*DINODESIZ, SEEK_SET);
    fread(&di_free, sizeof(dinode), 1, fd);
    //m_inode[block_no].i_ino = block_no;
    m_inode[block_no].di_uid = di_free.di_uid;
    m_inode[block_no].di_gid = di_free.di_gid;
    m_inode[block_no].di_mode = di_free.di_mode;
    m_inode[block_no].di_size = di_free.di_size;
    m_inode[block_no].di_number = di_free.di_number;
    m_inode[block_no].di_time = di_free.di_time;
    for (int i = 0; i < NADDR; i++)
        m_inode[block_no].di_addr[i] = di_free.di_addr[i];
    return block_no;
}

void iput(int block_no, dinode * m_inode)//把内存i节点的值传回磁盘i节点
{
    dinode di_free;
    di_free.di_uid = m_inode[block_no].di_uid;
    di_free.di_gid = m_inode[block_no].di_gid;
    di_free.di_mode = m_inode[block_no].di_mode;
    di_free.di_size = m_inode[block_no].di_size;
    di_free.di_number = m_inode[block_no].di_number;
    di_free.di_time = m_inode[block_no].di_time;
    for (int i = 0; i < NADDR; i++)
        di_free.di_addr[i] = m_inode[block_no].di_addr[i];
    fseek(fd, (2 * BLOCKSIZ) + (block_no - 1)*DINODESIZ, SEEK_SET);
    fwrite(&di_free, sizeof(dinode), 1, fd);
}

int balloc()
{
    //磁盘块分配
    if (fs.s_nfree == 0)
    {
        printf("Disk FULL!");
        return 0;
    }
    else
    {
        unsigned int temp = 0;
        if (fs.s_pfree == 0)
        {
            block_free b_free;
            temp = fs.s_free[0];
            fseek(fd, (temp-1 + 2+ DINODEBLK)*BLOCKSIZ, SEEK_SET);//找到当前组长块的起始地址
            fread(&b_free, sizeof(block_free), 1, fd);//将该组长块的内容读入超级块
            fs.s_pfree = b_free.blk_count;
            for (int i = 0; i < NICFREE; i++)
            {
                fs.s_free[i] = b_free.blk_free[i];
            }
            fs.s_nfree--;
        }
        else
        {
            fs.s_nfree--;
            temp=fs.s_pfree--;
            temp = fs.s_free[temp];
            fs.s_free[fs.s_pfree+1] = 0;
        }
        return temp;
    }
    int ret = 0;
    if (fs.s_pfree== 0)
    {
        if (fs.s_free[0] == 0)
            printf("磁盘已满\n");
        else
        {
            block_free b_free;
            ret = fs.s_free[0];
            fseek(fd, (2 +ret -1+DINODEBLK)* BLOCKSIZ, SEEK_SET);
            fread(&b_free, sizeof(block_free), 1, fd);
            for (int i = 0; i < NICFREE; i++)
                fs.s_free[i] = b_free.blk_free[i];
            fs.s_pfree = b_free.blk_count;
            fs.s_nfree--;
        }
    }
    else
    {
        ret = fs.s_free[fs.s_pfree];
        fs.s_free[fs.s_pfree] = 0;
        fs.s_pfree--;
        fs.s_nfree--;
    }
    return ret;
}

int bfree(int b_no)
{
    if(b_no == 0) return -1;
    unsigned int temp = 0;
    block_free b_free;
    if (fs.s_nfree == FILEBLK)
    {
        //printf("ERROR! Disk FULL!\n");
        return 0;
    }
    if (fs.s_pfree == NICFREE-1)
    {
        temp = b_no;
        fseek(fd, (temp-1 + 2 + DINODEBLK)*BLOCKSIZ, SEEK_SET);//找到当前组长块的起始地址
        for (int i = 0; i < NICFREE; i++)
        {
            b_free.blk_free[i] = fs.s_free[i];
        }
        b_free.blk_count = fs.s_pfree;
        fwrite(&b_free, sizeof(block_free), 1, fd);
        fs.s_nfree++;
        fs.s_pfree = 0;
        fs.s_free[0] = b_no;
    }
    else
    {
        fs.s_nfree++;
        fs.s_pfree++;
        fs.s_free[fs.s_pfree] = b_no;
    }
    return 1;
    if (fs.s_pfree == NICFREE - 1)
    {
        fseek(fd, (2+DINODEBLK+b_no- 1) *  BLOCKSIZ, SEEK_SET);
        for (int i = 0; i < NICINOD; i++)
            b_free.blk_free[i] = fs.s_free[i];
        b_free.blk_count = fs.s_pfree;
        fwrite(&b_free, sizeof(block_free), 1, fd);
        fs.s_pfree = 0;
        fs.s_inode[fs.s_pfree] = b_no;
        fs.s_nfree++;
    }
    else
    {
        fs.s_pfree++;
        fs.s_free[fs.s_pfree] = b_no;
        fs.s_nfree++;
    }
    return 1;
}

int iformat(unsigned short iNode)  //初始化磁盘i节点
{
    dinode d_node;
    if (iNode > 512)
    {
        printf("ERROR! No such node!");
        return 0;
    }
    else
    {
        d_node.di_uid = 0;
        d_node.di_gid = 0;
        d_node.di_mode = 00007;//初始化的权限为创建者拥有所有权限
        d_node.di_number = 0;
        d_node.di_time = 0;
        d_node.di_size = 0;
        for (int i = 0; i < NADDR; i++)
        {
            d_node.di_addr[i] = 0;
        }
        fseek(fd, 2 * BLOCKSIZ + iNode*DINODESIZ, SEEK_SET);
        fwrite(&d_node, sizeof(dinode), 1, fd);
        return 1;
    }
}
