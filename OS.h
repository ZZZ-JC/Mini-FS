//
//  OS.h
//  Test1
//
//  Created by 王岩 on 2018/9/9.
//  Copyright © 2018 王岩. All rights reserved.
//

#ifndef OS_h
#define OS_h
#include <cstdio>
#include <memory.h>
#include <string>
#include <iostream>

#include <time.h>


//常量定义
#define system_size (1<<30) //系统大小,1G
#define block_size (1<<12) //块大小,4K
#define block_mount (system_size/block_size) //块个数
#define reserved_block_mount (block_mount/16) //保留块的数目，即disk结构体逻辑上应该占的块数
#define data_block_mount (block_mount - reserved_block_mount) //数据块数目


//目录项
struct dirUnit {
	char fileName[59];
	int inodeNumber;
};

//目录表


//inode
struct iNode {
	int startBlockNum; //文件数据起始块编号
	long fileSize; //文件的大小
};

const int inode_count = data_block_mount;

//superblock
struct superBlock
{
	int iNodeMount; //inode数量
	int blockMount; //系统总共有多少块，对应fat表有多大
	int iMapBlockMount; //inodeMap（标志inode_array使用情况）占多少块
	int num1stDataBlock; //第一块数据块的块号是多少
	int iNodeBlockMount; //inode占多少块
						 //int rootInode; //根目录的root的inode号是多少
	int fatBlockMount; //fat表占多少块

};

struct DISK {
	superBlock superblock;
	int FAT[block_mount];
	int inode_Map[inode_count];
	iNode inode_array[inode_count];
	dirUnit dirUnits[inode_count];
};

#endif /* OS_h */
