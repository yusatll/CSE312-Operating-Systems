#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>

using namespace std;
FILE * file;

int data_block_id = 2;
int root_dir_counter = -1;
int free_inodes = 0;
int free_data_block = 0;

int inode_block_size = -1;			// disardan al. 	
int data_block_size = -1;

int super_block_size = 64;
int inode_size = 64;
int root_size = 16;
int inode_sayisi = -1;
int data_block_sayisi = -1;
int inode_id = 1;
int start_db = 0;		// = root_size + super_block_size + (inode_size * inode_sayisi);
char usr[] = "usr";
char ysa[] = "ysa";
char fileSystemName[] 	= "";

// Operations
char fso_oper[] 		= "fileSystemOper";
char mkdir_oper[]		= "mkdir";
char rmdir_oper[] 		= "rmdir";
char write_oper[] 		= "write";
char read_oper[]		= "read";
char list_oper[] 		= "list";
char del_oper[] 		= "del";
char lnsym_oper[] 		= "lnsym";
char dumpe2fs_oper[]	= "dumpe2fs";

void get_date(char * d);
int get_data_block_id();
int update_inode();
int uygun_inode_bul();
void update_file(char * c,int d);
int check_file_oper(const char * str1, const char* str2);
void make_mkdir(char * filename);
int check_root_dir(char * str);
void inode_a_yaz(char* c,int id);
void update_root_dir(int i, char * s);
void make_rmdir(char * filename);
void get_inode_info(int id);
void make_list_l();
void make_read(char * dest, char * target);
void make_write(char * dest, char * target);
void tokenize(char * target);
int find_file_id(char * str);
int find_db_id(int a);
int update_inode_size(int a, int b);
int delete_inode(int a);
int delete_data_block(int a);
void update_super_block(char * filename);
void make_dumpe();
int update_file_2(char * c);
void update_root_block(int i);
int check_inode_dir(int id, char * str);
int main(int argc, char const *argv[])
{	

	char line[256];
	cin.getline(line,256);
	//fgets(line,256,stdin); // \n ile birlikte alir.
	cout << line << endl;


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
	token_line[counter][k] = '\0';


	//cout << "cc "<<token_line[counter][k]<< endl;

// İŞLEMLERE BAŞLAMADAN ÖNCE FİLEDAN GEREKLİ BİLGİLERİ OKUYORUZ.
	//strcpy(fileSystemName, token_line[2]);
	int len_filename=0;
	file = fopen(token_line[1], "r+");
	fseek(file,-2,SEEK_END);
	fscanf(file,"%d",&len_filename);

	fseek(file, - (len_filename+3), SEEK_END);
	fscanf(file,"%s",fileSystemName);

	cout << "File system name:" << fileSystemName<<endl;
	rewind(file);
	
	fseek(file,13,SEEK_SET);
	fscanf(file,"%d",&free_inodes);

	// take inode_sayisi
	fseek(file, 1, SEEK_CUR);
	fscanf(file,"%d",&inode_sayisi);
	start_db = root_size + super_block_size + (inode_size * inode_sayisi);
	//free_inodes = inode_sayisi;
	//cout << "inode_sayisi: " <<inode_sayisi << endl;
	// read data_block_size
	fseek(file, 18, SEEK_CUR);
	fscanf(file,"%d",&data_block_size);
	//cout << "data_block_size: "  << data_block_size << endl; 
	inode_block_size = data_block_size;

	fseek(file,14,SEEK_CUR);
	fscanf(file,"%d",&free_data_block);

	fseek(file,1,SEEK_CUR);
	fscanf(file,"%d",&data_block_sayisi);

	//cout << free_inodes << " / " << inode_sayisi << endl;
	//cout << free_data_block << " / " << data_block_sayisi << endl;

	//if(check_file_oper(token_line[0],fso_oper) == 1){}
	
	// fileSystemOper ile gelen filename öncekiyle aynı olmalı.
	if (check_file_oper(token_line[1], fileSystemName) == 1)	
	// bu file sistem oluşturulmuş
	{
		if(check_file_oper(token_line[2], mkdir_oper) == 1)// MKDIR
			make_mkdir(token_line[3]);
		else if(check_file_oper(token_line[2],rmdir_oper) == 1)
			make_rmdir(token_line[3]);
		else if(check_file_oper(token_line[2],list_oper) == 1)
			make_list_l();
		else if(check_file_oper(token_line[2],write_oper) == 1)
			make_write(token_line[4], token_line[3]);
		else if(check_file_oper(token_line[2], del_oper) == 1)
			make_rmdir(token_line[3]);
		else if(check_file_oper(token_line[2],read_oper) == 1)
			make_read(token_line[3],token_line[4]);
		else if(check_file_oper(token_line[2],dumpe2fs_oper) == 1)
			make_dumpe();

		
		fclose(file);
		update_super_block(token_line[1]);
	}
	else
	{
		cout << "Bu isimde file system bulunamadı. -> " << token_line[1] << endl;	
	}
	

	return 0;
}

void make_dumpe()
{
	cout << "dumpe2fs operation\n";
	cout << "Total Inode Blocks: " << inode_sayisi << endl;
	cout << "Free  Inode Blocks: " << free_inodes << endl;
	cout << "Total  Data Blocks: " << data_block_sayisi << endl;
 	cout << "Free 	Data Blocks: " << free_data_block << endl;
 	make_list_l();
}

void make_write(char * dest, char * target)
{
	cout << "write operation\n";
	char ctarget[40];
	strcpy(ctarget, target);
	//cout << dest << " daki bilgileri buraya yaz: " << target << endl;
	make_mkdir(target);
	tokenize(ctarget);
	//cout << "tokenize sonra " << ctarget << endl;
	int id = find_file_id(ctarget);
	int db_id = find_db_id(id);
	//cout << "data block id:" << db_id << endl;

	// file dan okunan değeri go_db yerine yaz.
	
	FILE * f = fopen(dest,"r");
	
	fseek (f , 0 , SEEK_END);
	int fsize = ftell (f);
	rewind (f);

	update_inode_size(id, fsize+4);

	char * buffer = (char*) malloc (sizeof(char)*fsize +5);
	size_t result = fread (buffer,1,fsize,f);
	//cout << "result " << result << endl;
	sprintf(buffer,"%s%s",buffer," EOF\n");
	//cout << "buffer " << buffer << endl;

	int go_db = start_db + ((db_id-1) * data_block_size) +5;
	fseek(file,go_db,SEEK_SET);
	fprintf(file, "%s", buffer);
	//fprintf(file, "%s\n", "\0");

	free(buffer);
	fclose(f);
}

int update_inode_size(int id, int size)
{
	int go = root_size + super_block_size + ((id-1) * inode_size) +37;
	fseek(file,go,SEEK_SET);
	fprintf(file,"%6d",size);
}

// gelen id ye gidip onun ilk datablock id sini döndürür
int find_db_id(int a)
{
	int db = -1;
	int go = root_size + super_block_size + ((a-1) * inode_size) + 44;
	//cout << "go: " << go << endl; 
	fseek(file,go,SEEK_SET);
	fscanf(file,"%d",&db);
	return db;
}

// data blocklardan file ı bul. id return et.
int find_file_id(char * str)
{
	fseek(file, start_db, SEEK_SET);
	char line[30] = " ";
	int id = -1;
	while(check_file_oper(line,"0") != 1)
	{
		fscanf(file,"%d",&id);
		fscanf(file,"%s",line);
		//cout << id<<" line: " << line << endl;
		
		if(check_file_oper(line,str) == 1)
			break;
			
	}
	//cout << line <<" id: " << id<< endl;
	return id;
}


void make_read(char * dest, char * target)
{
	cout << "read operation.\n";
	tokenize(dest);
	//cout << "tokenize sonra " << dest << endl;
	
	int id = find_file_id(dest);
	//cout << dest << " id: " << id << endl;
	int db_id = find_db_id(id);
	//cout << "data block id:" << db_id << endl;
	// datablock dan okunan değeri file a yerine yaz.
	int go_db = start_db + ((db_id-1) * data_block_size) +5;
	//fseek(file,go_db,SEEK_SET);
	//cout << "go: " << go_db<< endl;
	
	FILE * f = fopen(target, "w");
	char line[30] = " ";
	fseek(file,go_db,SEEK_SET);
	do
	{
		//fscanf(file,"%s",line);
		fgets(line,3,file);
		//cout <<" line: " << line << endl;
		
		if(check_file_oper(line,"EOF") == 1)
			break;
		fprintf(f, "%s",line );
		//cout << "line: "<< line << endl;
	}while(check_file_oper(line,"EOF") != 1);
	//cout << "now: " << ftell(file)<< endl;

}

void make_list_l()
{
	//cout << "ls -l operation\n";
	int start_root = super_block_size + 5;
	fseek(file,start_root, SEEK_SET);
	fscanf(file,"%d",&root_dir_counter);

	int go = start_db + 5;
	int id = -1;
	char str[40];
	char s;
	fseek(file,go,SEEK_SET);
	//cout << "counter " << root_dir_counter << endl;
	for (int i = 0; i < root_dir_counter; ++i)
	{
		fscanf(file, "%d",&id);
		fscanf(file,"%s",str);
		cout << id <<  "  ";
		fpos_t save;
		fgetpos(file, &save);
		get_inode_info(id);
		cout << str << endl;
		fsetpos(file,&save);
		fscanf(file,"%c",&s);
	}
}

void get_inode_info(int id)
{
	int go = super_block_size + root_size + ((id-1) * inode_size);
	fseek(file, go, SEEK_SET);
	//cout << id << " baslangıcı -> " << go<< endl; 
	fseek(file,16,SEEK_CUR);
	char date[21] = " ";

	fgets(date,21,file);
	//cout << "date: " << date << endl;
	fseek(file, ftell(file)+1, SEEK_SET);
	char size[7] = " ";
	fgets(size, 7, file);
	//cout << "size: " << size << endl;
	cout << size << " " << date << " ";
}

void make_mkdir(char * filename)
{
	cout << "mkdir operation\n";
	int dosya_sayisi = 0;
	for (int i = 0; i < strlen(filename); ++i)
	{
		if (filename[i] == '/')
		{
			dosya_sayisi++;
		}
	}
	//cout << dosya_sayisi << " kadar dosya var.\n";
	char files[dosya_sayisi][strlen(filename)];
	char * ch = strtok(filename, "“”\"/");
	int t = 0;
	while(ch != NULL){
		//cout << "ch: " << ch << endl; 
		strcpy(files[t], ch);
		t++;
		ch = strtok(NULL, "“”\"/");
	}

	
	if(check_root_dir(files[0]) == -1 && dosya_sayisi != 1){
		cout << files[0] << " önceden oluşturulmamış.\n";
	}
	else
	{
	for (int i = 0; i < t; ++i)
		{
			//cout << "files " << files[i] <<endl;
			fpos_t save;
			fgetpos(file, &save);
			//update_file(files[i],dosya_sayisi);
			int r = update_file_2(files[i]); // r = -1 ise root ta yok.
			//cout << r << " r " << files[i] << endl;
			if(r != -1)
			{
				int inode_check = check_inode_dir(r,files[++i]);
				//cout << inode_check<<" inode_check " << files[i]<< endl;
				if (inode_check != -1){
					inode_a_yaz(files[++i],inode_check);
				}
				else{
					inode_a_yaz(files[i],r);
				}
			}
			else
				update_root_block(1);

			fsetpos(file,&save);
		}
	}

	//update_root_block(1);

}

int check_inode_dir(int id, char * str)
{
	char temp[30];
	rewind(file);
	// root_size dir e bişeyler yazmak için.
	//start_db = root_size + super_block_size + (inode_size * inode_sayisi);
	int go = ((id-1)* data_block_size) + start_db;
	//cout << "start_db " << start_db << endl;

	fseek(file,start_db, SEEK_SET);
	fpos_t root;
	
	//cout << "db baslangıc: " << ftell(file) << endl;
	char db[] = "";

	fgets(db,5,file);
	int root_dir_sonu = start_db + 4096;
	//cout << "db " << db << endl;
	if (check_file_oper(db, "   1") == 1)
	{
		//cout << "root_size block\n";
		fseek(file,ftell(file)+1,SEEK_SET);
		fgetpos(file,&root);
		//cout << ftell(file) << endl;
		char line[30] = "NULL";
		while(check_file_oper(line,"0") == -1 && ftell(file) != root_dir_sonu)
		{	
			strcpy(temp, line);
			fscanf(file,"%s",line);
			//cout << "line " << line << endl;
			if(check_file_oper(line, str) == 1)	// bu dosya var önceden.
			{
				//cout << temp << " bu dosya var önceden.\n";
				int var_olan_dosyanin_inode_id = -5;
				sscanf(temp,"%d", &var_olan_dosyanin_inode_id);
				//cout << "bu dosya bu id de  var. " << var_olan_dosyanin_inode_id << endl;
				return var_olan_dosyanin_inode_id;
			}

		}
		
		//rewind(file);
	}
	//fseek(file,ftell(file)-data_block_size,SEEK_CUR);
	//fsetpos(file,&root);
	//fprintf(file, "%4d %s\n", inode_id,str);
	//cout << "bu dosya önceden yok: " << str << endl;
	rewind(file);
	return -1;
}

int update_file_2(char * filename)
{
	//free_inodes--;
	//free_data_block--;
	int go = start_db + 5;
	int root_dir_sonu = start_db + data_block_size;
	fpos_t s;
	fseek(file,go, SEEK_SET);
	char line[5]= "";
	char id[3] = "";
	int temp1 = -1, temp2 = -1;
	do{
		//fscanf(file,"%s",id);
		fscanf(file,"%s",line);
		//cout << " line "<< line << " - filename " << filename << endl;
		if(strlen(line) <= 2)
			sscanf(line,"%d",&temp1);
		//cout << temp1 << " - " << temp2 <<endl;
		if(temp1 == 0)	// root boş
			break;
		else if(temp1 > 0) // bir dosya okuduk. id aldık. son yerini aldık.
		{
			temp2 = temp1;
			//fgetpos(file, &s);
		}
		if (strlen(line) > 1)
		{
			fgetpos(file,&s);
		}
		if (check_file_oper(line,filename) == 1)
		{	// gelen dosya root içinde varmış.
			// inode a yazılacak.
			//cout << "temp2 " << temp2 << endl;
			//cout << "bu dosya önceden var. inode a git yaz. \n";
			return temp2;
		}
		//else
			//cout << "bu dosyalar aynı değil: " << line <<" " << filename << endl;
		if (ftell(file) >= root_dir_sonu)
		{
			break;
		}

	}while(check_file_oper(line,"0") == -1);
	//cout << "temp2 " << temp2 << " t1 " << temp1 << endl;
	int inode_id = uygun_inode_bul();

	if(temp1 == 0 && temp2 == -1){
		fseek(file, go-1, SEEK_SET);
	}
	else{
		fsetpos(file,&s);
	}
	//cout << "bkm sonra: "<< ftell(file) << endl;
	//printf("roota yaz: %4d %s\n",inode_id,filename);
	fprintf(file, "\n%4d %s\n",inode_id,filename );
	return -1;
}

void update_root_block(int i)
{
	int start_root = super_block_size + 5;
	fseek(file,start_root, SEEK_SET);

	fscanf(file,"%6d",&root_dir_counter);

	if (i == 1)
		root_dir_counter++;
	else
		root_dir_counter--;
	fseek(file,start_root,SEEK_SET);

	fprintf(file, "%6d\n", root_dir_counter);
}


int check_inode_id(int t)
{
	int go = super_block_size + root_size + ((t-1) * inode_size) + 46;
	fseek(file,go, SEEK_SET);
	int db_id = -1;
	fscanf(file,"%d",&db_id);

}

void update_file(char * folder,int dosya_sayisi)
{
	fpos_t p;
	fgetpos(file, &p);
	static int onceki_id = -2;

	int ii_id = -9;

	fpos_t save;
	fgetpos(file, &save);
	ii_id = check_root_dir(folder);
	//cout << "checked root: "<< ii_id << endl;
	fsetpos(file, &save);
	if(onceki_id != -2 && ii_id == -1)
	{	// inode a yaz.
		fpos_t save2;
		fgetpos(file, &save2);
		inode_a_yaz(folder, onceki_id);
		fsetpos(file, &save2);
		onceki_id = -2;
	}
	else if(ii_id == -1 && onceki_id == -2)
	{	// -1 ise dosya önceden yok.
		//cout << folder << " önceden yok.\n";
		fsetpos(file, &p);
	//	cout << "şimdi aynı yerde miyiz? " << ftell(file) << endl;
		fpos_t save3;
		fgetpos(file, &save3);
		inode_id = uygun_inode_bul();
		fsetpos(file, &save3);
		rewind(file);
		//cout << "uygun inode " << inode_id << endl;
		fpos_t save4;
		fgetpos(file, &save4);
		update_root_dir(inode_id, folder);
		fsetpos(file, &save4);
		//fseek(file,super_block_size,SEEK_SET);
	}
	else
	{
		onceki_id = ii_id;
		//cout << folder << " daha önceden var. onun altındaki yere yazılmalı " << onceki_id << endl;

	}
	

}



void update_super_block(char * filename)
{
	FILE * fp = fopen(filename, "r+");
	//cout << "update_super_block: " << filename << endl;
	
	fseek(fp, 13, SEEK_SET);
	fprintf(fp, "%3d", free_inodes);
	
	fseek(fp, 40,SEEK_CUR);
	fprintf(fp, "%3d",free_data_block );
	
	fclose(fp);
}



void make_rmdir(char * filename)
{
	cout << "rmdir operation\n";
	int dosya_sayisi = 0;
	for (int i = 0; i < strlen(filename); ++i)
	{
		if (filename[i] == '/')
		{
			dosya_sayisi++;
		}
	}

	char files[dosya_sayisi][strlen(filename)];
	char * ch = strtok(filename, "“”\"/");
	int t = 0;
	while(ch != NULL){
		//cout << "ch: " << ch << endl; 
		strcpy(files[t], ch);
		t++;
		ch = strtok(NULL, "“”\"/");
	}

	//cout << "sil: " << files[dosya_sayisi-1] << endl;

	

	fseek(file, start_db, SEEK_SET);
	char line[30] = " ";
	int id = -1;
	int len = -1;
	while(check_file_oper(line,"0") != 1)
	{
		fscanf(file,"%d",&id);
		fscanf(file,"%s",line);
		//cout << id<<" line: " << line << endl;
		len = strlen(line) +6;
		if(check_file_oper(line,files[dosya_sayisi-1]) == 1){
			//fprintf(file, "%s", );
			//cout << "silinecek char " << len << endl;
			break;
		}
			
	}
	//cout << line <<" id: " << id<< endl;
	//cout <<ftell(file) <<" silinecek id " << id << endl;

	fseek(file, ftell(file)-len+1, SEEK_SET);
	//cout << ftell(file);
	for (int i = 0; i < len; ++i)
	{
		fprintf(file, " ");
	}

	//update_inode_size(id,0);
	int db_id = delete_inode(id);
	delete_data_block(db_id);
	update_root_block(-1);
	
}

int delete_data_block(int id)
{
	int go = start_db + ((id-1) * data_block_size);
	fseek(file, go, SEEK_SET);
	fprintf(file, "%4d\n",0 );
	for (int i = 0; i < data_block_size-6; ++i)
	{
		fprintf(file, " ");
	}
	
	fprintf(file, "\n");
	free_data_block++;
}

int delete_inode(int id)
{
	int go = root_size+ super_block_size +((id-1) * inode_size);
	//cout << "go: " << go << endl;

	fseek(file,go + 8, SEEK_SET);
	fprintf(file, "%c",'T' );

	fseek(file, ftell(file) + 7, SEEK_SET);
	fprintf(file, "%20d",0 );

	fseek(file, ftell(file) + 1, SEEK_SET);
	fprintf(file, "%6d",0 );

	fseek(file, ftell(file) + 1, SEEK_SET);
	int db_id = -1;
	fscanf(file,"%d",&db_id);
	//cout << "silinecek db: " << db_id << endl;
	free_inodes++;
	return db_id;
	
}


void update_root_dir(int i, char * s)
{
	
	fseek(file,start_db,SEEK_SET);
	fpos_t zero;
	int counter = 0, c=0;
	char db[] = "";
	fgets(db,6,file);
	int now = ftell(file);
	fgetpos(file,&zero);
	int root_dir_sonu = start_db + data_block_size;
	char line[] = " ";
	counter = 0;
	//cout << "start: " << ftell(file) << endl;
	int t = 0;
	while(now < root_dir_sonu )//&& check_file_oper(line,"0") == -1
	{
		now = ftell(file);
		//cout << "now " << now << endl;
		
		//fscanf(file,"%d",&t);
		//cout << "t " << t << "  ftel: " <<ftell(file) <<endl;
		if (ftell(file) < root_dir_sonu){
			fscanf(file,"%s",line);
			//cout << "line " << line << " son " << ftell(file)<< endl;
		}
		counter = ftell(file);
	}
	//cout << "counter: "<< counter << endl;
	
	//cout << "counter " << ++counter << endl;
	if(counter < root_dir_sonu)
		fseek(file,counter,SEEK_CUR);
	else
		fsetpos(file,&zero);
	//cout << "bura: "<< ftell(file)<< " counter " << counter<< endl;
	//cout << "roota yaz " << i << "  " << s << endl;

	fprintf(file, "%4d %s\n",i,s );

	update_root_block(1);
	
}

void inode_a_yaz(char * str, int id)
{
	// id gelen inode un idsi
	//free_inodes--;
	//cout << "inode yaz: " << id << "  " << str << endl;
	int get_ind = uygun_inode_bul();
	rewind(file);
	char linedb[6]= " ";
	int inode_start = root_size + super_block_size + ((id-1) * inode_size);
	fseek(file,inode_start+44,SEEK_SET);

	fgets(linedb,5,file);
	//cout << "linedb " << linedb << endl;
	int inode_db = -1;
	sscanf(linedb,"%d",&inode_db);

	int go = ((inode_db-1) * data_block_size) + 5;

	fseek(file, start_db +go, SEEK_SET);

	fprintf(file, "%4d %s\n",get_ind, str );
}



// return -1 dosya daha önceden yoksa
int check_root_dir(char * str)
{
	char temp[30];
	rewind(file);
	// root_size dir e bişeyler yazmak için.
	start_db = root_size + super_block_size + (inode_size * inode_sayisi);
	//cout << "start_db " << start_db << endl;

	fseek(file,start_db, SEEK_SET);
	fpos_t root;
	
	//cout << "db baslangıc: " << ftell(file) << endl;
	char db[] = "";

	fgets(db,5,file);
	int root_dir_sonu = start_db + 4096;
	//cout << "db " << db << endl;
	if (check_file_oper(db, "   1") == 1)
	{
		//cout << "root_size block\n";
		fseek(file,ftell(file)+1,SEEK_SET);
		fgetpos(file,&root);
		//cout << ftell(file) << endl;
		char line[30] = "NULL";
		while(check_file_oper(line,"0") == -1 && ftell(file) != root_dir_sonu)
		{	
			strcpy(temp, line);
			fscanf(file,"%s",line);
			//cout << "line " << line << endl;
			if(check_file_oper(line, str) == 1)	// bu dosya var önceden.
			{
				//cout << temp << " bu dosya var önceden.\n";
				int var_olan_dosyanin_inode_id = -5;
				sscanf(temp,"%d", &var_olan_dosyanin_inode_id);
				//cout << "bu dosya bu id de  var. " << var_olan_dosyanin_inode_id << endl;
				return var_olan_dosyanin_inode_id;
			}

		}
		
		//rewind(file);
	}
	//fseek(file,ftell(file)-data_block_size,SEEK_CUR);
	fsetpos(file,&root);
	//fprintf(file, "%4d %s\n", inode_id,str);
	//cout << "bu dosya önceden yok: " << str << endl;
	return -1;
	
}


int update_inode()
{
	free_inodes--;
	int id=-1;
	int re_id;
	int valid_loc = ftell(file);
	//cout <<"valid_loc: " << valid_loc << endl;
	// T->F
	fseek(file,valid_loc-1,SEEK_SET);
	fprintf(file,"F");
	// get ID
	fseek(file, valid_loc-6, SEEK_SET);
	fscanf(file,"%d",&id);
	//cout << "IDDDDDDD " << id<<endl;
	re_id = id;

	// data block ayarla.
	//cout << "asda: "<<ftell(file)<<endl;

	int inode_start = root_size + super_block_size + ((id-1) * inode_size);

	fseek(file,inode_start,SEEK_SET);
	char inode_str[] = " ";
	fgets(inode_str,8,file);
	//cout << "Uygun olan Inode: " << inode_str << endl;
	//cout << ftell(file) << endl;
    char date[20];
    get_date(date);
    //cout << "Date: " << date << endl;
	fseek(file,ftell(file)+ 9 ,SEEK_SET);
	fprintf(file, "%s", date);
	//cout << "Date dosyaya yazıldı\n";
	fseek(file,ftell(file)+ 1 ,SEEK_SET);
	fprintf(file, "%6d\n", 0);
	//cout << "Size dosyaya yazıldı.\n";
	//cout << ftell(file) << endl;

	// data block oluşturmadan önce imlecin yerini kaydettik.
	fpos_t before_data_block;
	fgetpos(file,&before_data_block);
	int db_id = get_data_block_id();
	//cout << "gelen db_id: "<< db_id<< endl;
	fsetpos(file,&before_data_block);
	fprintf(file, "%3d\n", db_id);

	//cout << "before return " << re_id << endl;
	return re_id;

}

int get_data_block_id()
{
	free_data_block--;
	int bos_db = -1;
	int go = start_db;
	fpos_t p;
	while(bos_db != 0){
		fseek(file,go,SEEK_SET);
		fscanf(file,"%d",&bos_db);
		//cout << "bos_db:"<< bos_db << endl;
		if(bos_db != 0){
			data_block_id = bos_db;
		}
		if(bos_db == 0){
			fgetpos(file, &p);
		}
		go += data_block_size;
	}
	data_block_id++;
	//cout << "data_block_id:   : " << ++data_block_id <<endl;
	fsetpos(file,&p); 
	fseek(file,ftell(file)-4,SEEK_SET);
	fprintf(file, "%4d\n",data_block_id );
	return data_block_id;
}

int uygun_inode_bul()
{
	char valid = 'F';
	rewind(file);
	int go = super_block_size+8+root_size;
	
	while(valid !='T'){
		fseek(file, go,SEEK_SET);
		fscanf(file,"%c",&valid);

		go = go + inode_size;
		
	}

	int id =0;
	if (valid == 'T')
	{
		//cout << "T:\n";
		fpos_t save;
		fgetpos(file, &save);
		id = update_inode();
		fsetpos(file, &save);
	}
	//cout << "return id: " << id << endl;
	
	rewind(file);
	return id;

}

void tokenize(char * target)
{
	int dosya_sayisi = 0;
	for (int i = 0; i < strlen(target); ++i)
	{
		if (target[i] == '/')
		{
			dosya_sayisi++;
		}
	}
	char files[dosya_sayisi][strlen(target)];
	char * ch = strtok(target, "“”\"/");
	int t = 0;
	while(ch != NULL){
		//cout << "ch: " << ch << endl; 
		strcpy(files[t], ch);
		t++;
		ch = strtok(NULL, "“”\"/");
	}
	strcpy(target,files[dosya_sayisi-1]);

}



void get_date(char * d)
{
	char str[20];
	time_t t = time(0);
	auto tm* now = localtime(&t);
	sprintf(str,"%2d/%2d/%2d  %2d:%2d:%2d",now->tm_mday, (now->tm_mon + 1), 
    	(now->tm_year + 1900), now->tm_hour, now->tm_min, now->tm_sec);
	for (int i = 0; i < 20; ++i)
	{
		d[i] = str[i];
	}
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