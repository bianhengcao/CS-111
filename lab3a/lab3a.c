/*NAME:Bianheng Cao
EMAIL:bianhengcao@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include "ext2_fs.h"

char* image;
int fd;

void indirect_blocks(int i_num, int level, int offset, int block, int block_size)//recursive function for indirect blocks
{
  int num=block_size/sizeof(uint32_t);
  uint32_t blocks[num];//32 bit addresses
  memset(blocks,0,num);
  int takeoff=offset;
  if(pread(fd, blocks, block_size, 1024+(block-1)*block_size)<0)
    {
      fprintf(stderr, "Failed to read indirect inode blocks!:%s\n", strerror(errno));
      exit(2);
    }
  int i;
  for(i=0;i<block_size/sizeof(uint32_t);i++)
    {
      if(blocks[i]==0)//useless value
	continue;
      fprintf(stdout, "INDIRECT,%u,%u,%u,%u,%u\n",
	      i_num,
	      level,
	      takeoff,
	      block,
	      blocks[i]
	      );
      if(level==1)
	takeoff++;
      if(level==2)//offset values from ext2_doc
	{
	  takeoff+=2;//???? works with trivial at least?
	  indirect_blocks(i_num, 1,takeoff,blocks[i], block_size); 
	}
      if(level==3)
	{
	  takeoff+=0;
	  indirect_blocks(i_num, 2,takeoff,blocks[i], block_size); 
	}
    }
}

void indirect_dir(const struct ext2_inode* inode, int i_num, int level, int block, int block_size, int s_offset)
{
  int num=block_size/sizeof(uint32_t);
  uint32_t blocks[num];//32 bit addresses
  memset(blocks,0,num);
  if(pread(fd, blocks, num, 1024+block_size*(block-1))<0)
    {
      fprintf(stderr, "Failed to read directory blocks!:%s\n", strerror(errno));
      exit(2);
    }
  int i;
  for(i=0;i<num;i++)
    {
      if(blocks[i]==0)
	continue;
      if(level==2)
	indirect_dir(inode, i_num, 1, blocks[i], block_size, s_offset);
      if(level==3)
	indirect_dir(inode, i_num, 2, blocks[i], block_size, s_offset);
      int l=0;
      int offset=0;
      while(l<inode->i_size)
	{
	  struct ext2_dir_entry dir;
	  if(pread(fd, &dir, sizeof(struct ext2_dir_entry), 1024+block_size*(blocks[i]-1)+offset)<0)
	    {
	      fprintf(stderr, "Failed to read directory blocks!:%s\n", strerror(errno));
	      exit(2);
	    }
	  if(dir.file_type==0)
	    break;
	  if(dir.inode!=0)//a value of zero means entry is not used
	    {
	      char name[dir.name_len+1];
	      strcpy(name, dir.name);
	      fprintf(stdout, "DIRENT,%u,%u,%u,%u,%u,'%s'\n",
		      i_num,
		      s_offset+l,
		      dir.inode,
		      dir.rec_len,
		      dir.name_len,
		      name
		      );
	    }
	  l+=dir.rec_len;
	  offset+=dir.rec_len;
	}
    }
}

int main(int argc, char** argv)
{
  if(argc != 2)
    {
      fprintf(stderr, "Wrong number of arguments!\n");
      exit(1);
    }
  image=argv[1];
  fd=open(image, O_RDONLY);
  if(fd<0)
    {
      fprintf(stderr, "Failed to open file system!\n");
      exit(1);
    }
  
  //Superblock
  struct ext2_super_block gene; 
  if(pread(fd, &gene, sizeof(struct ext2_super_block), 1024)<0)//stored at an offest of 1024 from start
    {
      fprintf(stderr, "Failed to read superblock!:%s\n", strerror(errno));
      exit(2);
    }
  if(gene.s_magic!=EXT2_SUPER_MAGIC)
    {
      fprintf(stderr, "Wrong type of file system!\n", strerror(errno));
      exit(2);
    }
  int block_size=EXT2_MIN_BLOCK_SIZE<< gene.s_log_block_size;//I'll need this value later
  fprintf(stdout, "SUPERBLOCK,%u,%u,%u,%u,%u,%u,%u\n",
	  gene.s_blocks_count,
	  gene.s_inodes_count,
	  block_size,
	  gene.s_inode_size,
	  gene.s_blocks_per_group,
	  gene.s_inodes_per_group,
	  gene.s_first_ino
	  );

  //Group
  int num=(gene.s_blocks_count-1)/gene.s_blocks_per_group + 1;//To round up for last block
  struct ext2_group_desc* arr=malloc(num*sizeof(struct ext2_group_desc));
  if(pread(fd, arr, num * sizeof(struct ext2_group_desc), 1024 + block_size)<0)
    {
      fprintf(stderr, "Failed to read group block descriptors!:%s\n", strerror(errno));
      exit(2);
    }
  int i;
  for(i=0;i<(num-1);i++)
    {
      fprintf(stdout,"GROUP,%d,%u,%u,%u,%u,%u,%u,%u\n",
	      i,
	      gene.s_blocks_per_group,
	      gene.s_inodes_per_group,
	      arr[i].bg_free_blocks_count,
	      arr[i].bg_free_inodes_count,
	      arr[i].bg_block_bitmap,
	      arr[i].bg_inode_bitmap,
	      arr[i].bg_inode_table
	      );
    }
  fprintf(stdout,"GROUP,%d,%u,%u,%u,%u,%u,%u,%u\n",
	  num-1,
	  gene.s_blocks_count-(num-1)*gene.s_blocks_per_group,
	  gene.s_inodes_count-(num-1)*gene.s_inodes_per_group,
	  arr[i].bg_free_blocks_count,
	  arr[i].bg_free_inodes_count,
	  arr[i].bg_block_bitmap,
	  arr[i].bg_inode_bitmap,
	  arr[i].bg_inode_table
	  );//for last element
  
  //Free Block
  for(i=0;i<num;i++)
    {
      int j;
      for(j=0;j<block_size;j++)
	{
	  uint8_t itr;//can't have signed bit in the way
	  if(pread(fd, &itr, 1, arr[i].bg_block_bitmap * block_size +j)<0)
	    {
	      fprintf(stderr, "Failed to read bitmap!:%s\n", strerror(errno));
	      exit(2);
	    }
	  int a;
	  uint8_t mask=1;
	  for(a=0;a<8;a++)//read bit by bit
	    {
	      if((itr&mask) == 0)//0 means free block
		fprintf(stdout, "BFREE,%d\n", i*gene.s_blocks_per_group+j*8+a+1);
	      mask=mask<<1;
	    }
	}
    }
  
  //Free I-node
  for(i=0;i<num;i++)
    {
      int j;
      for(j=0;j<block_size;j++)
	{
	  uint8_t itr;//can't have signed big in the way
	  if(pread(fd, &itr, 1, arr[i].bg_inode_bitmap*block_size +j)<0)
	    {
	      fprintf(stderr, "Failed to read I-node bitmap!:%s\n", strerror(errno));
	      exit(2);
	    }
	  int a;
	  uint8_t mask=1;
	  for(a=0;a<8;a++)//read bit by bit
	    {
	      if((itr&mask) == 0)//0 means free block
		fprintf(stdout, "IFREE,%d\n", i*gene.s_blocks_per_group+j*8+a+1);
	      mask=mask<<1;
	    }
	}
    }
  
  //I-node
  for(i=0;i<num;i++)
    {
      struct ext2_inode inode;
      int i_num=EXT2_ROOT_INO;
      while(i_num<gene.s_inodes_count)
	{
	  int flag=0;//need flag to prevent further reading of inodes that may be valid
	  int j;
	  int invalid=1;		  
	  for(j=0;j<block_size;j++)//check if allocated
	    {
	      uint8_t itr;
	      if(pread(fd, &itr, 1, arr[i].bg_inode_bitmap*block_size+j)<0)
		{
		  fprintf(stderr, "Failed to read I-node bitmap!:%s\n", strerror(errno));
		  exit(2);
		}
	      uint8_t mask=1;
	      int a;
	      for(a=0;a<8;a++)
		{
		  if(i_num == i*gene.s_inodes_per_group+j*8+a+1)
		    {		  
		      flag=1;
		      if((itr&mask) == 0)
			invalid=0;
		      break;
		    }
		  if(i_num>gene.s_inodes_per_group)
		    {
		      fprintf(stderr, "Inode bitmap is corrupted!\n");
		      exit(2);
		    }
		  mask=mask<<1;
		}
	      if(flag)
		break;
	    }
	  if(!invalid)
	    {
	      i_num++;
	      continue;
	    }
	  long whack=1024+(arr[i].bg_inode_table-1)*block_size+(i_num-1)*sizeof(struct ext2_inode);//bg_inode_table gives block ID, so the offest is superblock size, num of blocks afterward, and then the specific inode right after(that we are iterrating through)
	  if(pread(fd, &inode, sizeof(struct ext2_inode), whack)<0)
	    {
	      fprintf(stderr, "Failed to read I-node bitmap!:%s\n", strerror(errno));
	      exit(2);
	    }
	  char type='?';
	  if(inode.i_mode & 0xA000)//Symbolic link
	    type='s';
	  else if(inode.i_mode & 0x8000)//Regular file
	    type='f';
	  else if(inode.i_mode & 0x4000)//Directory
	    type='d';
	  fprintf(stdout, "INODE,%d,%c,%o,%u,%u,%u,",
		  i_num,
		  type,
		  inode.i_mode & (0x01C0|0x0038|0x0007),//USR:0x0100+0x0080+0x0040, GROUP:0x0020+0x0010+0x0008, OTHER:0x0004+0x0002+0x0001
		  inode.i_uid,
		  inode.i_gid,
		  inode.i_links_count
		  );  
	  char buf[20];
	  struct tm* t;
	  time_t tz=inode.i_ctime;
	  t=gmtime(&tz);
	  strftime(buf, sizeof(buf), "%m/%d/%y %H:%M:%S,", t);
	  fprintf(stdout,"%s", buf);
	  tz=inode.i_mtime;
	  t=gmtime(&tz);
	  strftime(buf, sizeof(buf), "%m/%d/%y %H:%M:%S,", t);
	  fprintf(stdout,"%s", buf);
	  tz=inode.i_atime;
	  t=gmtime(&tz);
	  strftime(buf, sizeof(buf), "%m/%d/%y %H:%M:%S,", t);
	  fprintf(stdout,"%s", buf);
	  fprintf(stdout,"%d,%d", inode.i_size, inode.i_blocks);
	  for(j=0;j<15;j++)
	    fprintf(stdout,",%u",inode.i_block[j]);
	  fprintf(stdout,"\n");//whoops
	  if(i_num==EXT2_ROOT_INO)
	    i_num=gene.s_first_ino;//go to first nonreserved ino after root
	  else
	    i_num++;
	}
    }
  //Directory
   for(i=0;i<num;i++)
    {
      struct ext2_inode inode;
      int i_num=EXT2_ROOT_INO;
      while(i_num<gene.s_inodes_count)
	{
	  int in=0;
	  int invalid=1;
	  int j;
	  for(j=0;j<block_size;j++)//check if allocated
	    {
	      uint8_t itr;
	      if(pread(fd, &itr, 1, arr[i].bg_inode_bitmap*block_size+j)<0)
		{
		  fprintf(stderr, "Failed to read I-node bitmap!:%s\n", strerror(errno));
		  exit(2);
		}
	      uint8_t mask=1;
	      int a;
	      for(a=0;a<8;a++)
		{
		  if(i_num == i*gene.s_inodes_per_group+j*8+a+1)
		    {		  
		      in=1;
		      if((itr&mask) == 0)
			invalid=0;
		      break;
		    }
		  if(i_num>gene.s_inodes_per_group)
		    {
		      fprintf(stderr, "Inode bitmap is corrupted!\n");
		      exit(2);
		    }
		  mask=mask<<1;
		}
	      if(in)
		break;
	    }
	  if(!invalid)
	    {
	      i_num++;
	      continue;
	    }
	  long whack=1024+(arr[i].bg_inode_table-1)*block_size+(i_num-1)*sizeof(struct ext2_inode);
	  if(pread(fd, &inode, sizeof(struct ext2_inode), whack)<0)
	    {
	      fprintf(stderr, "Failed to read I-node bitmap!:%s\n", strerror(errno));
	      exit(2);
	    }
	  if(inode.i_mode & 0x4000)//Directory
	    {
	      int k;
	      for(k=0;k<EXT2_NDIR_BLOCKS;k++)
		{
		  int l=0;
		  int offset=0;
		  while(l<inode.i_size)
		    {
		      struct ext2_dir_entry dir;
		      if(pread(fd, &dir, sizeof(struct ext2_dir_entry), 1024+block_size*(inode.i_block[k]-1)+offset)<0)
			{
			  fprintf(stderr, "Failed to read directory blocks!:%s\n", strerror(errno));
			  exit(2);
			}
		      if(dir.file_type==0)
			break;
		      if(dir.inode!=0)//a value of zero means entry is not used
			{
			  char name[dir.name_len+1];
			  strcpy(name, dir.name);
			  fprintf(stdout, "DIRENT,%u,%u,%u,%u,%u,'%s'\n",
				  i_num,
				  l,
				  dir.inode,
				  dir.rec_len,
				  dir.name_len,
				  name
				  );
			}
		      l+=dir.rec_len;
		      offset+=dir.rec_len;
		    }
		}
	      if(inode.i_block[EXT2_IND_BLOCK]!=0)//requires recursion for indirect directories
		indirect_dir(&inode, i_num, 1, inode.i_block[EXT2_IND_BLOCK], block_size, 12);
	      if(inode.i_block[EXT2_DIND_BLOCK]!=0)
		indirect_dir(&inode, i_num, 3, inode.i_block[EXT2_DIND_BLOCK], block_size,268);
	      if(inode.i_block[EXT2_TIND_BLOCK]!=0)
		indirect_dir(&inode, i_num, 2, inode.i_block[EXT2_TIND_BLOCK], block_size,65804);
	    }
	  if(i_num==EXT2_ROOT_INO)
	    i_num=gene.s_first_ino;//go to first nonreserved ino after root
	  else
	    i_num++;
	}
    }
   //Indirect block
   for(i=0;i<num;i++)
    {
      struct ext2_inode inode;
      int i_num=EXT2_ROOT_INO;
      while(i_num<gene.s_inodes_count)
	{
	  int in=0;
	  int invalid=1;
	  int j;
	  for(j=0;j<block_size;j++)//check if allocated
	    {
	      uint8_t itr;
	      if(pread(fd, &itr, 1, arr[i].bg_inode_bitmap*block_size+j)<0)
		{
		  fprintf(stderr, "Failed to read I-node bitmap!:%s\n", strerror(errno));
		  exit(2);
		}
	      uint8_t mask=1;
	      int a;
	      for(a=0;a<8;a++)
		{
		  if(i_num == i*gene.s_inodes_per_group+j*8+a+1)
		    {		  
		      in=1;
		      if((itr&mask) == 0)
			invalid=0;
		      break;
		    }
		  if(i_num>gene.s_inodes_per_group)
		    {
		      fprintf(stderr, "Inode bitmap is corrupted!\n");
		      exit(2);
		    }
		  mask=mask<<1;
		  }
	      if(in)
		break;
	    }
	  if(!invalid)
	    {
	      i_num++;
	      continue;
	    }
	  long whack=1024+(arr[i].bg_inode_table-1)*block_size+(i_num-1)*sizeof(struct ext2_inode);
	  if(pread(fd, &inode, sizeof(struct ext2_inode), whack)<0)
	    {
	      fprintf(stderr, "Failed to read I-node bitmap!:%s\n", strerror(errno));
	      exit(2);
	    }
	  
	  if(inode.i_block[EXT2_IND_BLOCK]!=0)
	    indirect_blocks(i_num,1, 12, inode.i_block[EXT2_IND_BLOCK], block_size);
	  if(inode.i_block[EXT2_DIND_BLOCK]!=0)
	    indirect_blocks(i_num,2, 268, inode.i_block[EXT2_DIND_BLOCK], block_size);
	  if(inode.i_block[EXT2_TIND_BLOCK]!=0)
	    indirect_blocks(i_num,3,65804, inode.i_block[EXT2_TIND_BLOCK], block_size);
	  
	  if(i_num==EXT2_ROOT_INO)
	    i_num=gene.s_first_ino;//go to first nonreserved ino after root
	  else
	    i_num++;
	}
    }
   free(arr);
   exit(0);
}

