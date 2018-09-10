//
//  creat.cpp
//  Test1
//
//  Created by 王岩 on 2018/9/9.
//  Copyright © 2018 王岩. All rights reserved.
//

#include <stdio.h>

#include "OS.h"

//全局变量
extern FILE *p; //外存起始位置
extern std::string cmd;
DISK *disk = new DISK;
extern char command[50]; //文件名标识符
extern char SName[60];
int makeNewFile(const char* filename, int size);
long filesize(FILE *fp);
int newfileout(char name[]);
int addr_superblock_start = 0;
int addr_fat_start = 0;
int addr_inodemap_start = 0;
int addr_inodearray_start = 0;
int addr_dirUnits_start = 0;
int addr;

void create();
void mount();
void copyin(const char*filename);//复制文件
void copyout(const char*filename, char SName[]);
