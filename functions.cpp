#include"functions.h"

//文件重命名
int changeName(char oldName[], char newName[])
{
	for (int i = 0; i < disk.superblock.iNodeMount; i++)
	{
		if (strcmp(oldName, disk.dirUnits[i].fileName) == 0)
		{
			strcpy(disk.dirUnits[i].fileName, newName);
			return 0;
		}
	}
	cout << "File not found\n";
	return -1;
}


//显示帮助信息
void help() {
	printf("The following are some operations and the details:\n");
	printf("creat\t Creat a new storage space.\t\n");
	printf("cd\t Toggle directory.\t\n");
	printf("add\t Add a local file to the Mini-FS space.\t\n");
	printf("fmt\t Formatting the current Mini-FS space.\t\n");
	printf("dr\t Display the file directory in the space.\t\n");
	printf("cp\t Copy the file.\t\n");
	printf("dl\t Delete the file in the space.\t\n");
	printf("tp\t Display the text file in the space.\t\n");
	printf("more\t Paging displays text files in space.\t\n");
	printf("att\t Display spatial file properties.\t\n");
	printf("help\t Display informationof help.\t\n");
	printf("dlDir\t Delete folder.\t\n");
	printf("pth\t Display the path for the current file.\t\n");
	printf("newfldr\t Creat a new folder.\t\n");
	printf("newfile\t Creat a new file.\t\n");
	printf("rname\t Rename the file.\t\n");
	printf("close\t Close the space and return to the operating system.\t\n");
}

//显示空间文件属性
int Att(char filename[]) {
	for (int i = 0; i < disk.superblock.iNodeMount; i++) {
		if (strcmp(disk.dirUnits[i].fileName, filename) == 0) {
			printf("%s ", filename);
			printf("%d\n", disk.inode_array[disk.dirUnits[i].inodeNumber].fileSize);
			return 0;
		}
	}
	printf("No such file.\n");
	return -1;
}


//格式化系统
void fmt() {
	memset(disk.FAT, 0, sizeof(disk.FAT));
	memset(disk.inodeMap, 0, sizeof(disk.inodeMap));
	disk.superblock.iNodeMount = 0;
}

//删除文件内容
int releaseFile(int inodeNum) {
	iNode* Myinode = &disk.inode_array[inodeNum];
	int fileBlock[data_block_mount];
	memset(fileBlock, 0, sizeof(fileBlock));
	for (int i = 0, j = Myinode->startBlockNum; i < Myinode->fileSize; i++, j = disk.FAT[j]) {
		fileBlock[i] = disk.FAT[j];
	}
	int i = 0;
	while (fileBlock[i] != 0) {
		fileBlock[i] = 0;
		i++;
	}
	Myinode->fileSize = 0;
	return 0;
}


//删除目录表中文件对应目录项
int deleteDirUnit(int unitIndex) {
	int dirAmount = disk.superblock.iNodeMount;
	for (int i = unitIndex; i < dirAmount; i++) {
		disk.dirUnits[i] = disk.dirUnits[i + 1];
	}
	disk.superblock.iNodeMount--;
	return 0;
}

//删除文件
int deleteFile(char fileName[]) {

	int fileNum = disk.superblock.iNodeMount;
	int i;
	int flag = 0;
	for (i = 0; i<fileNum; i++) {
		if (strcmp(disk.dirUnits[i].fileName, fileName) == 0) {
			//找到文件
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		printf("File not found!\n");
		return -1;
	}
	dirUnit myUnit = disk.dirUnits[i];
	int inodeNum = myUnit.inodeNumber;
	releaseFile(inodeNum);
	deleteDirUnit(i);
}

//检查是否有足够数量的block
int getBlock(int blockNumber) {
	//第一个可用块
	int startBlock = -1;
	int num = 0;
	for (int i = block_mount / 16; i < block_mount; i++) {
		//FAT表并不从0号block开始,前1/16的block作为保留块
		if (disk.FAT[i] == 0 && startBlock == -1)
			startBlock == i;
		//找到一块可用块
		if (disk.FAT[i] == 0) {
			++num;
		}
	}
	if (num >= blockNumber)
		return startBlock;
	else
		return -1;
}


//为文件创建inode
int createInode(int iNodeNum, int fileBlockNum, int fileSize) {
	iNode *currentInode = &disk.inode_array[iNodeNum];
	currentInode->fileSize = fileSize;
	currentInode->startBlockNum = fileBlockNum;
	return 0;
}

//为文件创建目录项
int addDirUnit(char fileName[], int inodeNum) {
	int fileNum = disk.superblock.iNodeMount;
	if (fileNum == inode_count) {
		printf("Dir is full,please delete some files!\n");
		return -1;
	}
	for (int i = 0; i<fileNum; i++) {
		if (strcmp(disk.dirUnits[i].fileName, fileName) == 0) {
			printf("File already exists!\n");
			return -1;
		}
	}
	dirUnit* newDirUnit = &disk.dirUnits[fileNum];
	disk.superblock.iNodeMount++;
	strcpy(disk.dirUnits[fileNum].fileName, fileName);
	disk.dirUnits[fileNum].inodeNumber = inodeNum;
	return 0;
}


//创建新文件,fileSize是文件需占用盘块数
int newFile(char fileName[], int fileSize) {
	int i;
	for (i = 0; i < strlen(fileName); i++) {
		if (fileName[i] == '.')
			break;
	}
	//规定文件名不超过8位
	if (i > 8) {
		printf("file name too long!\n");
		return -1;
	}
	//扩展名不能超过3位
	if (strlen(fileName) - i - 1>3) {
		printf("extend name too long!\n");
		return -1;
	}

	int inodeBlock = -1;
	for (int i = 0; i < inode_count; i++) {
		if (disk.inodeMap[i] == 0)
			//找到空闲inode
			inodeBlock = i;
	}
	if (inodeBlock == -1) {
		printf("No more block could be used!\n");
		return -1;
	}
	//申请数据块
	int fileBlock = getBlock(fileSize);
	if (fileBlock == -1) {
		printf("No enough block could be used!\n");
		return -1;
	}
	//修改目录项
	createInode(inodeBlock, fileBlock, fileSize);
	for (int i = fileBlock, j = block_mount / 16, k = 0; j<block_mount&&k<fileSize; i = j, j++, k++) {
		if (disk.FAT[j] == 0) {
			disk.FAT[i] = j;
		}
	}
	//修改目录
	if (addDirUnit(fileName, inodeBlock) == -1)
		return -1;
	return 0;
}
