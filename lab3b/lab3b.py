#!/usr/bin/python

'''
NAME: Bianheng Cao
EMAIL: bianhengcao@gmail.com
'''

import sys
import csv

class superblock:
    def __init__(self, list):
        self.b=int(list[1])
        self.i=int(list[2])
        self.b_size=int(list[3])
        self.i_size=int(list[4])
        self.b_num=int(list[5])
        self.i_num=int(list[6])
        self.i_first=int(list[7])

'''
class group:
    def __init__(self, list):
        self.num=int(list[1])
        self.b=int(list[2])
        self.i=int(list[3])
        self.b_free=int(list[4])
        self.i_free=int(list[5])
        self.b_map=int(list[6])
        self.i_map=int(list[7])
        self.i_first=int(list[8])
        
class free_block:
    def __init__(self, list):
        self.num=int(list[1])

class free_inode:
    def __init__(self, list):
        self.num=int(list[1])
'''

class inode:
    def __init__(self, list):
        self.num=int(list[1])
        self.type=list[2]
        self.mode=int(list[3])
        self.owner=int(list[4])
        self.group=int(list[5])
        self.count=int(list[6])
        self.c_time=list[7]
        self.m_time=list[8]
        self.a_time=list[9]
        self.f_size=int(list[10])
        self.b_num=int(list[11])
        self.blocks=list[12:27]
        self.level=1

class directory:
    def __init__(self, list):
        self.parent=int(list[1])
        self.offset=int(list[2])
        self.i_num=int(list[3])
        self.e_len=int(list[4])
        self.n_len=int(list[5])
        self.name=list[6]

class indirect:
    def __init__(self, list):
        self.i_num=int(list[1])
        self.level=int(list[2])
        self.offset=int(list[3])
        self.i_block=int(list[4])
        self.b_num=int(list[5])

flag=0
s_block= []
f_block=[]
f_inode=[]
i_list = []
d_list = []
in_list = []
valid_blocks={}
i_free = []
i_used = []

def process(file):
    try:
        fd=open(file, 'r')
    except IOError:
        sys.stderr.write("Error opening included file name")
        exit(1)
    lines = csv.reader(fd)
    for line in lines:
        type=line[0]
        if type == 'SUPERBLOCK':
            item=superblock(line)
            s_block.append(item)
        elif type == "GROUP":
            continue
        elif type == 'BFREE':
            f_block.append(int(line[1]))
        elif type == 'IFREE':
            f_inode.append(int(line[1]))
        elif type == 'INODE':
            item = inode(line)
            i_list.append(item)
        elif type == 'DIRENT':
            item=directory(line)
            d_list.append(item)
        elif type == 'INDIRECT':
            item = indirect(line)
            in_list.append(item)
        else:
            sys.stderr.write("Csv file not formatted correctly!")
            exit(1)

def check_blocks(b_num, i_num, offset, level):
    msg="BLOCK"
    if level == 1:
        msg="INDIRECT BLOCK"
    elif level == 2:
        msg= "DOUBLE INDIRECT BLOCK"
    elif level == 3:
        msg= "TRIPLE INDIRECT BLOCK"
    if b_num == 0:
        pass
    elif b_num > s_block[0].b-1:
        print "INVALID " + msg + " " + str(b_num) + " IN INODE " + str(i_num) + " AT OFFSET " + str(offset)
        flag=1
        ret=1
    elif b_num < 8:
        print "RESERVED " + msg + " " + str(b_num) + " IN INODE " + str(i_num) + " AT OFFSET " + str(offset)
        flag = 1
        ret=1
    else:
        if b_num not in valid_blocks:
            valid_blocks[b_num] = [ (i_num, offset, level) ]
        else:
            valid_blocks[b_num].append( (i_num, offset, level) )

def blocks():
    for i in i_list:
        offset = 0
        for b in i.blocks:
            if offset > 11:
                break
            check_blocks(int(b), i.num, offset, 0)
            offset+=1
        check_blocks(int(i.blocks[12]), i.num, 12, 1)
        check_blocks(int(i.blocks[13]), i.num, 268, 2)
        check_blocks(int(i.blocks[14]), i.num, 65804, 3)

    for ind in in_list:
        check_blocks(ind.b_num, ind.i_num, ind.offset, ind.level)

    for num in range(8, s_block[0].b):
        if num not in f_block and num not in valid_blocks:
            print "UNREFERENCED BLOCK " + str(num)
            flag=1
        elif num in f_block and num in valid_blocks:
            print "ALLOCATED BLOCK " + str(num) + " ON FREELIST"
            flag=1
        elif num in valid_blocks and len(valid_blocks[num]) > 1:
            for n, offset, level in valid_blocks[num]:
                msg = "BLOCK"
                if level == 1:
                    msg = "INDIRECT BLOCK"
                if level == 2:
                    msg = "DOUBLE INDIRECT BLOCK"
                elif level == 3:
                    msg = "TRIPLE INDIRECT BLOCK"
                print "DUPLICATE " + msg + " " + str(num) + " IN INODE " + str(n) + " AT OFFSET " + str(offset)
            flag=1

def inodes():
    global i_free
    i_free = f_inode #create a list of actually freed inodes
    for i in i_list:
        if i.num in f_inode:
            print "ALLOCATED INODE " + str(i.num) + " ON FREELIST"
            flag=1
            i_free.remove(int(i.num))
        i_used.append(i)

    first=s_block[0].i_first
    last=s_block[0].i
    for i in range(first, last):
        found=False
        for ino in i_list:
            if i == ino.num:
                found=True
        if i not in f_inode and not found:
            print "UNALLOCATED INODE " + str(i) + " NOT ON FREELIST"
            flag=1
            i_free.append(i)

def dirents():
    dict={}
    num=s_block[0].i
    for dir in d_list:
        if dir.i_num > num:
            print "DIRECTORY INODE " + str(dir.parent) + " NAME " + dir.name + " INVALID INODE " + str(dir.i_num)
            flag=1
        elif dir.i_num in i_free:
            print "DIRECTORY INODE " + str(dir.parent) + " NAME " + dir.name + " UNALLOCATED INODE " + str(dir.i_num)
            flag=1
        else:
            dict[dir.i_num]=dict.get(dir.i_num, 0)+1
    for i in i_used:
        if i.num in dict:
            if dict[i.num] != i.count:
                print "INODE " + str(i.num) + " HAS " + str(dict[i.num]) + " LINKS BUT LINKCOUNT IS " + str(i.count)
                flag=1
        elif i.count != 0:
            print "INODE " + str(i.num) + " HAS 0 LINKS BUT LINKCOUNT IS " + str(i.count)
            flag=1

    links={}
    links[2]=2#forgot root whoops
    for dir in d_list:
        if dir.i_num <= s_block[0].i_num and dir.i_num not in i_free and dir.name != "'..'" and dir.name != "'.'":
            links[dir.i_num]=dir.parent
    for dir in d_list:
        if dir.name == "'.'" and dir.i_num != dir.parent:
                print "DIRECTORY INODE " + str(dir.parent) + " NAME '.' LINK TO INODE " + str(dir.i_num) + " SHOULD BE " + str(dir.parent)
                flag = 1
        elif dir.name == "'..'" and dir.i_num != links[dir.parent]:
            print "DIRECTORY INODE " + str(dir.parent) + " NAME '..' LINK TO INODE " + str(dir.i_num) + " SHOULD BE " + str(dir.parent)
            flag = 1

def main():
    if len(sys.argv) != 2:
        sys.stderr.write("Must include a csv file!")
        exit(1)
    file=sys.argv[1]
    process(file)
    #print len(s_block), len(i_list), len(d_list), len(in_list), len(f_block), len(f_inode)
    blocks()
    inodes()
    dirents()
    if flag == 1:
        exit(2)
    else:
        exit(0)

if __name__ == "__main__":
    main()