//
//  main.cpp
//  Test1
//
//  Created by 王岩 on 2018/9/8.
//  Copyright © 2018 王岩. All rights reserved.
//

#include "OS.h"
#include"functions.h"
using namespace std;

//函数声明

//帮助信息

void copyout(const char*filename, char SName[]);//拉出文件

void help();

void creat();

void mount();

int format();

int close();

void copyin(const char*filename);//拉入文件

int rm(char *filename);

void exitsys();

void ls();
//全局变量

FILE *p; //外存起始位置
string cmd;
char command[50]; //文件名标识符
char SName[30];
char filename[50];

int main()
{
	printf("Welcome to Mini-FS file system.\nFor help,use command: ""help"".\nOr type in your command directly.\n\n");
	while (1) {
		printf("Mini-FS-->");//???
		cin >> cmd;
		if (cmd == "create") {
			printf("\n");
			//cin >> SName;
			creat();
			printf("Create Mini-FS storage succeed!\n\n");
		}
		else if (cmd == "mount"){
			printf("\n");
			mount();
			printf("\n");
			printf("Mount Mini-FS storage succeed!\n\n");
		}
		else if (cmd == "copyin"){
			printf("\n");
			//cout<<""//???
			cin >> SName;
			copyin(SName);
			printf("Copy file %s in succeed!\n\n",SName);
		}
		else if (cmd == "copyout"){
			printf("\n");
			cin >> filename >> SName;//???
			copyout(filename, SName);
			printf("\n");
		}
		else if (cmd == "rename") {
			printf("\n");
			char oldName[20];
			char newName[20];
			printf("Please input file name which is going to be changed.\n");
			scanf("%s", oldName);
			printf("Please input a new file name.\n");
			scanf("%s", newName);
			changeName(oldName, newName);
			printf("\n");
		}
		else if (cmd == "help") {
			printf("\n");
			help();
			printf("\n");
		}
		else if (cmd == "att") {
			printf("\n");
			char fileName[20];
			printf("Please input a file name to dispaly its attribute.\n");
			scanf("%s", fileName);
			Att(fileName);
			printf("\n");
		}
		else if (cmd == "fmt") {
			printf("\n");
			fmt();
			printf("\n");
		}
		else if (cmd == "newfile") {
			printf("\n");
			char fileName[20];
			int fileSize;//输入的是文件字节数
			printf("Please input new file`s name.\n");
			scanf("%s", fileName);
			printf("Please input file size.\n");
			scanf("%d", fileSize);
			newFile(fileName, fileSize / block_size);
			printf("\n");
		}
		else if (cmd == "delete") {
			printf("\n");
			char fileName[20];
			printf("Please input file name to delete a file.\n");
			scanf("%s", fileName);
			deleteFile(fileName);
			printf("\n");
		}
		else if (cmd == "ls") {
			printf("\n");
			if (listFile() == 0) {
				printf("Finished\n\n");
			}
			else{
				printf("No file in Mini-FS\n\n");
			}
		}
		else {
			cout << cmd;
			printf(":\nIllegal command!\n\n");
		}
	}
}
