#include"functions.h"

//文件重命名
int changeName(char oldName[], char newName[])
{
	for (int i = 0; i < disk->superblock.iNodeMount; i++)
	{
		if (strcmp(oldName, disk->dirUnits[i].fileName) == 0)
		{
			strcpy(disk->dirUnits[i].fileName, newName);
			return 0;
		}
	}
	cout << "File not found\n";
	return -1;
}


//显示帮助信息
void help() {
	printf("Below are some help information:\n\n");
	printf("To create a Mini-FS storeage space:\t\t    create\n");
	printf("To load Mini-FS	in run-time memory:\t\t    mount\n");
	printf("To format Mini-FS storage space:   \t\t    fmt\n");
	printf("To list all files in Mini-FS:      \t\t    ls\n");
	printf("To delete a file from Mini-FS:     \t\t    delete\n");
	printf("To display a file`s attribution:   \t\t    att\n");
	printf("To show command tips information:  \t\t    help\n");
	printf("To rename an existing file in Mini-FS:              rename\n");
	printf("To close Mini-FS and back to operating system:      close\n");
	printf("To copy a file from operating system into Mini-FS:  copyin\n");
	printf("To copy a file from Mini-FS into operating system:  copyout\n");
}

//显示空间文件属性
int Att(char filename[]) {
	for (int i = 0; i < disk->superblock.iNodeMount; i++) {
		if (strcmp(disk->dirUnits[i].fileName, filename) == 0) {
			printf("%s ", filename);
			printf("%d\n", disk->inode_array[disk->dirUnits[i].inodeNumber].fileSize);
			return 0;
		}
	}
	printf("No such file.\n");
	return -1;
}


//格式化系统
void fmt() {
	//extern int addr_superblock_start ;
	//extern int addr_fat_start = 0;
	//extern int addr_inodemap_start = 0;
	//extern int addr_inodearray_start = 0;
	//extern int addr_dirUnits_start = 0;
	//extern int addr;
	//p = fopen("D:/mini-FS/mini.fs", "wb");
	//for (int i = 0; i < inode_count; i++)
	//{
	//	disk->dirUnits[i].inodeNumber = -1;
	//	disk->dirUnits[i].fileName[0] = 0;
	//}
	//for (int i = 0; i < reserved_block_mount; i++)
	//	disk->FAT[i] = 1;
	//for (int i = reserved_block_mount; i < block_mount; i++)
	//	disk->FAT[i] = 0;
	//for (int i = 0; i < inode_count; i++)
	//	disk->inode_Map[i] = 0;
	//for (int i = 0; i < inode_count; i++)
	//{
	//	disk->inode_array[i].fileSize = 0;
	//	disk->inode_array[i].startBlockNum = 0;
	//}
	//disk->superblock.blockMount = block_mount;
	//disk->superblock.fatBlockMount = sizeof(disk->FAT) / block_size;
	//disk->superblock.iNodeBlockMount = sizeof(sizeof(disk->inode_array)) / block_size;
	//disk->superblock.iNodeMount = data_block_mount;
	//disk->superblock.num1stDataBlock = reserved_block_mount + 1;

	////kaitou
	////superblock
	//addr_superblock_start = 0;
	//fseek(p, addr_superblock_start, SEEK_SET);
	//fwrite(&(disk->superblock), sizeof(disk->superblock), 1, p);


	////FAT
	//addr_fat_start = block_size * 10;
	//fseek(p, addr_fat_start, SEEK_SET);
	//fwrite((disk->FAT), sizeof((disk->FAT)), 1, p);

	////inode_map
	//addr_inodemap_start = block_size * 300;
	//fseek(p, addr_inodemap_start, SEEK_SET);
	//fwrite(disk->inode_Map, sizeof((disk->inode_Map)), 1, p);

	////inode_array
	//addr_inodearray_start = block_size * 2000;
	//fseek(p, addr_inodearray_start, SEEK_SET);
	//fwrite((disk->inode_array), sizeof((disk->inode_array)), 1, p);

	////dirUnits
	//addr_dirUnits_start = block_size * 8000;
	//fseek(p, addr_dirUnits_start, SEEK_SET);
	//fwrite(disk->dirUnits, sizeof(disk->dirUnits), 1, p);
	//addr = reserved_block_mount * 4096;

	////jiewei

	//fseek(p, system_size - 1, SEEK_SET);
	//char end = EOF;
	//fwrite(&end, 1, 1, p);
	//fclose(p);
	/*create();*/
	
}

//删除文件内容
int releaseFile(int inodeNum) {
	iNode* Myinode = &disk->inode_array[inodeNum];
	int fileBlock[data_block_mount];
	memset(fileBlock, 0, sizeof(fileBlock));
	for (int i = 0, j = Myinode->startBlockNum; i < Myinode->fileSize; i++, j = disk->FAT[j]) {
		fileBlock[i] = disk->FAT[j];
	}
	int i = 0;
	while (fileBlock[i] != 0) {
		disk->FAT[fileBlock[i]] = 0;
		i++;
	}
	Myinode->fileSize = 0;
	return 0;
}


//删除目录表中文件对应目录项
int deleteDirUnit(int unitIndex) {
	int dirAmount = disk->superblock.iNodeMount;
	for (int i = unitIndex; i < dirAmount; i++) {
		disk->dirUnits[i] = disk->dirUnits[i + 1];
	}
	disk->superblock.iNodeMount--;
	return 0;
}

//删除文件
int deleteFile(char fileName[]) {

	int fileNum = disk->superblock.iNodeMount;
	int i;
	int flag = 0;
	for (i = 0; i<fileNum; i++) {
		if (strcmp(disk->dirUnits[i].fileName, fileName) == 0) {
			//找到文件
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		printf("File not found!\n");
		return -1;
	}
	dirUnit myUnit = disk->dirUnits[i];
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
		if (disk->FAT[i] == 0 && startBlock == -1)
			startBlock == i;
		//找到一块可用块
		if (disk->FAT[i] == 0) {
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
	iNode *currentInode = &disk->inode_array[iNodeNum];
	currentInode->fileSize = fileSize;
	currentInode->startBlockNum = fileBlockNum;
	return 0;
}

//为文件创建目录项
int addDirUnit(char fileName[], int inodeNum) {
	int fileNum = disk->superblock.iNodeMount;
	if (fileNum == inode_count) {
		printf("Dir is full,please delete some files!\n");
		return -1;
	}
	for (int i = 0; i<fileNum; i++) {
		if (strcmp(disk->dirUnits[i].fileName, fileName) == 0) {
			printf("File already exists!\n");
			return -1;
		}
	}
	dirUnit* newDirUnit = &disk->dirUnits[fileNum];
	disk->superblock.iNodeMount++;
	strcpy(disk->dirUnits[fileNum].fileName, fileName);
	disk->dirUnits[fileNum].inodeNumber = inodeNum;
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
		if (disk->inode_Map[i] == 0)
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
		if (disk->FAT[j] == 0) {
			disk->FAT[i] = j;
		}
	}
	//修改目录
	if (addDirUnit(fileName, inodeBlock) == -1)
		return -1;
	return 0;
}
