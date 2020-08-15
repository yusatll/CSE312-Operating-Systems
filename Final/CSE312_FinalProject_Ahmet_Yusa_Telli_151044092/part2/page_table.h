#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H value

#include <iostream>
#include "structs.h"

using namespace std;


int checkin_page_table(Entry* page_table, int vir_add);
int empty_entry(Entry * page_table);
int update_table(Entry * page_table);
void create_page_table(Entry* page_table, int size,int counter);
int update_page_table(Entry * page_table, int index, int vir_add, int phy_add);
void print_page_table(Entry * page_table);
void clear_page_table(Entry* page_table);
void update_lru_index(Entry * page_table, int index);


#endif