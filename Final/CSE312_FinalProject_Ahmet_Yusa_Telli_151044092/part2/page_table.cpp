#include "page_table.h"

int pt_access = 0;
int pt_size;
int fifo_i = 0;
int lru_i = 0;
int page_table_print;

void create_page_table(Entry* page_table, int size, int printcounter)
{
	pt_size = size;
	page_table_print = printcounter;
	for (int i = 0; i < pt_size; ++i)
	{
		page_table[i].referance = false;
		page_table[i].modified = false;
		page_table[i].present = false;
		page_table[i].index_fifo = -1;
		page_table[i].index_lru = -1;
		page_table[i].physical_address = -1;
		page_table[i].virtual_address = -1;
	}
}


// eğer gelen virtual address önceden page table içinde varsa onu bul
int checkin_page_table(Entry * page_table, int vir_add)
{
	pt_access++;
	if(pt_access % page_table_print == 0){
		print_page_table(page_table);
	}
	for (int i = 0; i < pt_size; ++i)
	{
		if(page_table[i].virtual_address == vir_add){
			return page_table[i].physical_address;
		}
	}
	return -1;
}


int empty_entry(Entry * page_table)
{
	pt_access++;
	if(pt_access % page_table_print == 0){
		print_page_table(page_table);
	}
	for (int i = 0; i < pt_size; ++i)
	{
		if(page_table[i].present == false)
			return i;
	}
	return -1;
}


int update_page_table(Entry * page_table, int index, int vir_add, int phy_add)
{
	//cout << "update_page_table\n";
//	page_table[index].referance = true;
//	page_table[index].modified = true;
	page_table[index].present = true;
	page_table[index].index_fifo = fifo_i;
	page_table[index].index_lru = lru_i;
	page_table[index].virtual_address = vir_add;
	page_table[index].physical_address = phy_add;

	fifo_i++;
	lru_i++;
	pt_access++;

	if(pt_access % page_table_print == 0){
		print_page_table(page_table);
	}

	if (pt_access % pt_size == 0)
	{
		clear_page_table(page_table);
	}
}

void clear_page_table(Entry * page_table)
{
	for (int i = 0; i < pt_size; ++i)
	{
		page_table[i].referance = false;
		//page_table[i].modified = false;
	}
}


void print_page_table(Entry * page_table)
{
	cout << "Page Table Access: " << pt_access << endl;
	cout << "PAGE TABLE: \n";
	for (int i = 0; i < pt_size; ++i)
	{
		cout << "Referance: " << page_table[i].referance << endl;
		cout << "Modified: " << page_table[i].modified << endl;
		cout << "Present: " << page_table[i].present << endl;
		cout << "Index_fifo: " << page_table[i].index_fifo << endl;
		cout << "Index_lru: " << page_table[i].index_lru << endl;
		cout << "Physical Address: " << page_table[i].physical_address << endl;
		cout << "Virtual Address: " << page_table[i].virtual_address << endl;
		cout << endl;
	}
}

// fifo da bunu güncellemiyoruz. 
void update_lru_index(Entry * page_table, int index)
{
	page_table[index].index_lru = lru_i++;
}