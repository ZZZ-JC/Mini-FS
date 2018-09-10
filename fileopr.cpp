#include"OS.h"

extern DISK *disk;
int makeNewFile(const char* filename, int size)
{
	//i表示目录表中该文件的位置
	int i = 0;
	for (i = 0; i<inode_count; i++) {
		if (disk->dirUnits[i].inodeNumber == -1) {
			strcpy(disk->dirUnits[i].fileName, filename);
			break;
		}
	}
	int j = 0;
	//j表示该文件目录项中的inodeNumber
	for (j = 0; j<inode_count; j++)
	{
		if (disk->inode_Map[j] == 0)
		{
			disk->dirUnits[i].inodeNumber = j;
			break;
		}
	}
	int sum = 0;
	for (int p = reserved_block_mount; p < block_mount; p++)
	{
		if (disk->FAT[p] == 0)
		{
			for (int q = p + 1; q < block_mount; q++)
			{
				if (disk->FAT[q] == 0)
				{
					disk->FAT[p] = q;
					if (sum == 0)
						disk->inode_array[j].startBlockNum = p;
					sum++;
					if (sum == size) {
						return i;
					}
				}
				p = q;
			}
		}
	}
	return -1;
}

long filesize(FILE *fp)
{
	long size;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	return size;
}

int newfileout(char name[])
{
	int i = 0;
	for (i = 0; i<inode_count; i++) {
		if (strcmp(name, disk->dirUnits[i].fileName) == 0)
		{
			break;
		}
	}
	return i;

}
