#include"OS.h"
#include<stdio.h>
#include<string.h>
#include<iostream>

using namespace ::std;
extern DISK* disk;
extern FILE* fp;

//文件重命名
int changeName(char oldName[], char newName[]);

//显示帮助信息
void help();

//显示文件属性
int Att(char filename[]);

//格式化系统
void fmt();

//删除文件内容
int releaseFile(int inodeNum);

//删除目录表中文件对应目录项
int deleteDirUnit(int unitIndex);

//删除文件
int deleteFile(char fileName[]);

//检查是否有足够数量的block
int getBlock(int blockNumber);

//为文件创建inode
int createInode(int iNodeNum, int fileBlockNum, int fileSize);

//为文件创建目录项
int addDirUnit(char fileName[], int inodeNum);

//创建新文件
int newFile(char fileName[], int fileSize);
