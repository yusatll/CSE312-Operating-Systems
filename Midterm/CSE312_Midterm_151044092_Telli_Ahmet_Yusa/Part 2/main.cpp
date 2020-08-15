#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <stdlib.h>


using namespace std;
#define mb_1 1048576
int kalan_size = mb_1;


// Sayilar
int inode_block_icindeki_inode_sayisi = -1;	// hesaplanacak
int inode_block_sayisi = -1; 				// kullanıcıdan gelen değer ile hesaplanır
int inode_sayisi = -1; 						// kullanıcıdan gelecek
int data_block_sayisi = -1;					// inode_sayisi * 3
int undirect_data_block_sayisi = -1;		// = inode_sayisi

// Sizelar
int super_block_size = 64;
int inode_size = 64;
int inode_block_size = -1;			// disardan al. 	
int data_block_size = -1;
int root_size = 0;

// ID
int inode_id = 1;				// 4 basamaklı int
int inode_block_id = 1;			// 2 basamaklı int
int data_block_id = 1;			// 3 basamaklı int
int undirect_data_block_id = 0;	// 3 basamaklı int

// Satır Sayıları
int superblock_satir	= 4;
int inode_satir 		= 9;


char makeSystem_oper[] = "makeFileSystem";
char fileSystemName[] 	= "";

void create_empty_file(FILE * fptr, int bosalan);
void create_super_block(FILE * fptr);
void create_one_inode(FILE * fptr,int i_id);
void create_inode_block(FILE * fptr,int id);
void create_undirect_block(FILE * fptr,int id);
void create_data_block(FILE * fptr,int id);
void create_root_directory(FILE * fptr);
int check_file_oper(const char * str1, const char* str2);
void make_file_system(FILE * fptr, char * blocks_size, char * inodes, char*fileN);
void make_mkdir(FILE * fptr, char * filename);

int main(int argc, char const *argv[])
{
	// makeFileSystem 4 400 fileSystem.data

	FILE * file;
	//cout << "File System Start\n";
	bool makesystem = false;
	bool check_file_names = false;
	const char * exit = "q";
	//while(true)
	//{
		char line[256];
		cin.getline(line,256);
		//fgets(line,256,stdin); // \n ile birlikte alir.
		//cout << line << endl;


		if (check_file_oper(line,exit) == 1)
		{
			cout << "exit\n";
		}
		


	 	int lenght_input = strlen(line);
		char token_line[lenght_input][lenght_input];

		int k=0,counter=0;
		for (int i = 0; i < lenght_input; ++i)
		{
			if (line[i] == ' ')
			{
				token_line[counter][k] = '\0';
				//cout << "token_line: "<< token_line[counter] << endl;
				counter++;
				k=0;
			}
			else
			{
				token_line[counter][k] = line[i];
				k++;
			}

		}

		
		// FILE SYSTEM OLUSTURULDU MU?
		
		if(check_file_oper(token_line[0],makeSystem_oper) == 1)
		{
			// makefilesystem
			strcpy(fileSystemName, token_line[3]);
			cout << "File system name: " << fileSystemName<<endl;
			make_file_system(file, token_line[1],token_line[2],token_line[3]);
		}
		else
		{
			cout << "FIRST YOU NEED TO CREATE A FILE SYSTEM. THIS IS WRONG OPERATION: "<<token_line[0] << endl; 
			cout << "EXIT\n";
			//exit(1);
		}
	
	return 0;
}

void make_file_system(FILE * file, char * blocks_size, char * inodes, char*fileN)
{
	cout << "FILE SYSTEM CREATE:\n";
	//400
	inode_sayisi = atoi(inodes);
	undirect_data_block_sayisi = inode_sayisi;
	// 4096
	data_block_size = atoi(blocks_size) * 1024;
	// 4096
	inode_block_size = data_block_size;	// 4096

	// 64 = 4096 / 64
	inode_block_icindeki_inode_sayisi = inode_block_size / inode_size ;

	inode_block_sayisi = inode_sayisi / inode_block_icindeki_inode_sayisi;

	int artan_inode_sayisi = inode_sayisi - inode_block_icindeki_inode_sayisi * inode_block_sayisi;


	cout << "Toplam inode sayısı: "<< inode_sayisi << endl;
	cout << "1 Inode block'ta bulunan inode sayısı: " << inode_block_icindeki_inode_sayisi << endl;
	cout << "Inode blok sayısı: "<< inode_block_sayisi << endl;
	cout << "Data block Size: " << data_block_size << endl;
	cout << "Undirect Data block sayısı: "<< undirect_data_block_sayisi << endl;
	cout << "Son Inode block'taki inode sayısı: (Kalan varsa) " << artan_inode_sayisi << endl;

	// kalan size = 1MB - superblocksize - inodeların toplam size
	kalan_size = mb_1 - super_block_size - (inode_sayisi * inode_size);
	data_block_sayisi = kalan_size / data_block_size;
	//cout << "Kalan size: " << kalan_size << endl;
	cout << "Oluşturulacak data block sayısı: " << data_block_sayisi<< endl;

	int bosluk_alan = kalan_size - (data_block_sayisi * data_block_size) - strlen(fileN);
	//cout << "boş kalan alan: " << bosluk_alan << endl;
	if (artan_inode_sayisi != 0)
	{
		inode_block_sayisi++;
	}

	// SUPER BLOCK
	// INODE BLOCKS
	// INODES
	// UNDIRECT DATA BLOCKS
	// DATA BLOCKS

	file = fopen(fileN, "w+");
	create_empty_file(file,0);
	fseek(file,0,SEEK_SET);
	create_super_block(file);
	create_root_directory(file);

	for (int i = 1; i <= inode_block_sayisi; ++i)
		create_inode_block(file, i);

	// 1 data block for root directory
	for (int i = 1; i <= data_block_sayisi; ++i)
		create_data_block(file, i);

	//create_empty_file(file, bosluk_alan);
	fseek(file,0,SEEK_END);
	/*
	for (int i = 0; i < bosluk_alan-100; ++i)
		fprintf(file, " ");
	*/
	fprintf(file, "%s\n", fileN);
	fprintf(file, "%d", strlen(fileN));
	fclose(file);
}

void create_empty_file(FILE * fptr, int bos_alan)
{
	//cout << "create_empty_file " << endl;
 
	for (int i = 0; i < mb_1 ; ++i)
	{
		fprintf(fptr, "\n");
	}
}

// ROOT\n 				-> 5 byte
// 123456\n 			-> 7 byte size
// 123\n 				-> 4 byte data block
// Total root directory size = 16 bytes
// data block 1 for root
void create_root_directory(FILE * fptr)
{
	fprintf(fptr, "ROOT\n");
	fprintf(fptr, "%6d\n", root_size );
	fprintf(fptr, "%3d\n", data_block_id);
	data_block_id = 0;
}


// SB :\n 					-> 5 bytes
// Inodes: XXX/XXX\n 		-> 16 bytes Inode sayısı free/total
// Data Block Size: XXXX\n 	-> 22 bytes 
// Data Blocks: XXX/XXX\n 	-> 21 bytes Data block sayısı free/total
// Total Super Block 64 bytes
void create_super_block(FILE * fptr)
{
	//cout << "create_super_block\n";
	fprintf(fptr, "SB :\n");
	fprintf(fptr, "Inodes: %3d/%3d\n",inode_sayisi,inode_sayisi);
	fprintf(fptr, "Data Block Size: %4d\n",data_block_size);
	fprintf(fptr, "Data Blocks: %3d/%3d\n",data_block_sayisi,data_block_sayisi);
}


// I:_1234\n 		-> 8 byte ID
// T\n 				-> 2 byte Valid
// Date: 31/05/2020  15:16:17\n 	-> 27 bytes 
// 123456\n 		-> 7 byte Size
// 123\n 			-> 4 byte Direct[0]
// 123\n 			-> 4 byte Direct[1]
// 123\n 			-> 4 byte Direct[2]
// 123\n 			-> 4 byte Direct[3]
// 123\n 			-> 4 byte Undirect[0]
// Total Inode Size = 64 bytes
void create_one_inode(FILE * fptr, int i_id)
{
	fprintf(fptr, "I: %4d\n",i_id);
	fprintf(fptr, "T\n");
	fprintf(fptr, "Date: %20d\n",0 );
	fprintf(fptr, "123456\n");	// file size
	fprintf(fptr, "%3d\n",data_block_id);	// data block id
	fprintf(fptr, "%3d\n",data_block_id);	// data block id
	fprintf(fptr, "%3d\n",data_block_id);	// data block id
	fprintf(fptr, "%3d\n",data_block_id);	// data block id
	fprintf(fptr, "%3d\n",undirect_data_block_id);	// undirect data block id
	
}


// Inode Block 123\n 			-> 16 bytes
// Free Blocks: 12\n 			-> 16 bytes
// 1234\n 						-> 7 bytes Inode id leri
// inode_block_icindeki_inode_sayisi * inode
// Total Inode Block size = 4KB = 4096 bytes
void create_inode_block(FILE * fptr, int id)
{
	//int free_inodes = inode_block_icindeki_inode_sayisi;
	//fprintf(fptr, "Inode Block:%3d\n",id);
	//fprintf(fptr, "Free Blocks:%3d\n", free_inodes);
	
	for (int i = 0; i < inode_block_icindeki_inode_sayisi; ++i)
	{
		if (inode_id <= inode_sayisi)
		{
			create_one_inode(fptr,inode_id);
			inode_id++;
		}
	}
}


// Undir: 123\n 			-> 11 bytes
// 1234\n 					-> 5 bytes
// Total Undirect Block size = 16 bytes
void create_undirect_block(FILE * fptr, int id)
{
	fprintf(fptr, "Undir: %3d\n", 0);
//	undirect_data_block_id++;
	
	fprintf(fptr, "%4d\n", data_block_id);
	data_block_id++;
}

// 4 KB = 4 * 1024 = 4096
void create_data_block(FILE * fptr, int id)
{
	if(id == 1)
		fprintf(fptr, "%4d\n", 1);
	else
		fprintf(fptr, "%4d\n", 0);

	int size = data_block_size - 6;	// 4xid - 2x\n = 6
	//cout << "data block size: " << size << endl;
	for (int i = 0; i < size; ++i)
	{
		fprintf(fptr, " ");
	}
	
	fprintf(fptr, "\n");
}

int check_file_oper(const char * str1, const char* str2)
{
	//cout << "check_file_oper\n";
	for (int i = 0; i < strlen(str1); ++i)
	{
		if (str1[i] != str2[i])
		{
			//cout << "Not Same\n";
			return -1;
		}
	}
	//cout << "same\n";
	return 1;
}
