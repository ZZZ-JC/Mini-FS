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
	while (1) {
		cin >> cmd;
		if (cmd == "create") {
			//cin >> SName;
			creat();
		}
		else if (cmd == "mount"){
			mount();
		}
		else if (cmd == "copyin"){
			//cout<<""//???
			cin >> SName;
			copyin(SName);
		}
		else if (cmd == "copyout"){
			cin >> filename >> SName;//???
			copyout(filename, SName);
		}
		else if (cmd == "rename") {
			char oldName[20];
			char newName[20];
			printf("Please input file name which is going to be changed.\n");
			scanf("%s", oldName);
			printf("Please input a new file name.\n");
			scanf("%s", newName);
			changeName(oldName, newName);
		}
		else if (cmd == "help") {
			help();
		}
		else if (cmd == "att") {
			char fileName[20];
			printf("Please input a file name to dispaly its attribute.\n");
			scanf("%s", fileName);
			Att(fileName);
		}
		else if (cmd == "fmt") {
			fmt();
		}
		else if (cmd == "newfile") {
			char fileName[20];
			int fileSize;//输入的是文件字节数
			printf("Please input new file`s name.\n");
			scanf("%s", fileName);
			printf("Please input file size.\n");
			scanf("%d", fileSize);
			newFile(fileName, fileSize / block_size);
		}
		else if (cmd == "deletefile") {
			char fileName[20];
			printf("Please input file name to delete a file.\n");
			scanf("%s", fileName);
			deleteFile(fileName);
		}
	}
}
