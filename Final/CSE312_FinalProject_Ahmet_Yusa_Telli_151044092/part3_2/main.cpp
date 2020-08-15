#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <string.h>
#include "sort.h"
#include "page_table.h"
#include "page_replacement.h"

using namespace std;
using namespace std::chrono;

mutex mtx;

struct Page * virtual_memory;
struct Page * physical_memory;
struct Entry * page_table;


char programName[10];
int frameSize;			// 2^12
int numPhysical;		// 2^5
int numVirtual;			// 2^10
int pageTablePrintInt; 	// 10000
char algorithm[8];
char allocPolicy[10];
char diskFileName[20];


int page_table_size;
int fm_index = 0;
int page_fault_counter = 0;
int hit_counter = 0;
int number_of_read;
int number_of_write;
int number_of_page_replacements;
int number_of_disk_page_writes;
int number_of_disk_page_reads;


int bubble_page_repl;
int quick_page_repl;
int merge_page_repl;
int index_page_repl;


// thread run areas
int merge_start;
int merge_end;
int quick_start;
int quick_end;
int index_start;
int index_end;
int bubble_start;
int bubble_end;

int bubble_best = 0;
int quick_best = 0;
int index_best = 0;
int merge_best = 0;

char bubble_algo[7];
char quick_algo [7];
char index_algo [7];
char merge_algo [7];

int bubble_frame;
int quick_frame;
int merge_frame;
int index_frame;



void set(unsigned int index, int value, char * tName);
int get(unsigned int index, char * tName);

void fill_virtual_memory(int frameSize, int page_sayisi);
void fill_physical_memory(int frameSize, int page_sayisi);
void create_memories();
void thread1_sort(int frameSize, int pageSize);
void thread2_sort(int frameSize, int pageSize);
void thread3_sort(int frameSize, int pageSize);
void thread4_sort(int frameSize, int pageSize);
int string_compare(const char* str1, const char* str2);
int check_area(char * tName, int index);
int check_physical_memory();
void print_physical_memory(int ix);
void update_physical_memory(int virtual_add, int physical_add);
int find_page_algorithm();
void check_virtual_sorted();
int discharge_physical_memory();
void program_statistics();
void go_to_disk(int fm_address);
void clear_memories();
void program_run();
void optimal();
// 32/4 = page table uzunluğu


int two_square(int a){
	int r = 2;
	for (int i = 1; i < a; ++i)
		r = r * 2;	
	return r;
}





int main(int argc, char const *argv[])
{
	// sortArrays 12 5 10 LRU local 10000 diskFileName.dat
	// sortArrays 7 4 6 LRU local 10000 diskFileName.dat
	// sortArrays 10 6 10 LRU local 10000 diskFileName.dat


	if(argc != 5)
	{
		cout << "Eksik Parametre girdiniz. Çıkılıyor.\n";
		cout << "./sortArrays LRU local 10000 diskFileName.dat   --> şeklinde giriniz.\n";
		exit(1);
	}

	sscanf(argv[1],"%s", algorithm);
	sscanf(argv[2],"%s", allocPolicy);
	sscanf(argv[3],"%d", &pageTablePrintInt);
	sscanf(argv[4],"%s", diskFileName);




//	frameSize = 100;
//	numVirtual = 12;

/*
	cout << "frameSize: " << frameSize << endl;
	cout << "numPhysical: " << numPhysical << endl;
	cout << "numVirtual: " << numVirtual << endl<<endl;

	cout << "algorithm: " << algorithm << endl;
	cout << "allocPolicy: " << allocPolicy << endl;
	cout << "pageTablePrintInt: " << pageTablePrintInt << endl;
	cout << "diskFileName: " << diskFileName << endl;
*/
	/*
	char * algo1 = "LRU";
	char * algo2 = "NRU";
	char * algo3 = "FIFO";
	char * algo4 = "WSClock";
	char * algo5 = "SC";
	*/
	char algo[5][8] = {"LRU", "NRU", "FIFO", "WSClock", "SC"};

	int phyiscal_size = 16384;
	int virtual_size = phyiscal_size*8;
	frameSize = 512;
	numPhysical = phyiscal_size / frameSize;
	numVirtual = virtual_size  / frameSize ;
	page_table_size = numPhysical; 

	int total_bubble_repl[5];
	int total_quick_repl[5];
	int total_merge_repl[5];
	int total_index_repl[5];


	for (int j = 0; j < 5; ++j)
	{
		strcpy(algorithm, algo[j]);
		for (int i = 0; i < 3; i++){
			program_run();
		}
		total_bubble_repl[j] = bubble_best;
		total_quick_repl[j] = quick_best;
		total_merge_repl[j] = merge_best;
		total_index_repl[j] = index_best;
		bubble_best=0;
		quick_best=0;
		merge_best=0;
		index_best=0;
	}


	int bubble_avg_lru, bubble_avg_nru, bubble_avg_sc, bubble_avg_fifo, bubble_avg_wsc;
	int quick_avg_lru, quick_avg_nru, quick_avg_sc, quick_avg_fifo, quick_avg_wsc;
	int merge_avg_lru, merge_avg_nru, merge_avg_sc, merge_avg_fifo, merge_avg_wsc;
	int index_avg_lru, index_avg_nru, index_avg_sc, index_avg_fifo, index_avg_wsc;

	// LRU AVG
	bubble_avg_lru = total_bubble_repl[0] / 3;
	quick_avg_lru = total_quick_repl[0] / 3;
	merge_avg_lru = total_merge_repl[0] / 3;
	index_avg_lru = total_index_repl[0] / 3;

	// NRU AVG
	bubble_avg_nru = total_bubble_repl[1] / 3;
	quick_avg_nru = total_quick_repl[1] / 3;
	merge_avg_nru = total_merge_repl[1] / 3;
	index_avg_nru = total_index_repl[1] / 3;

	// FIFO AVG
	bubble_avg_fifo = total_bubble_repl[2] / 3;
	quick_avg_fifo = total_quick_repl[2] / 3;
	merge_avg_fifo = total_merge_repl[2] / 3;
	index_avg_fifo = total_index_repl[2] / 3;

	// WSC AVG
	bubble_avg_wsc = total_bubble_repl[3] / 3;
	quick_avg_wsc = total_quick_repl[3] / 3;
	merge_avg_wsc = total_merge_repl[3] / 3;
	index_avg_wsc = total_index_repl[3] / 3;

	// SC AVG
	bubble_avg_sc = total_bubble_repl[4] / 3;
	quick_avg_sc = total_quick_repl[4] / 3;
	merge_avg_sc = total_merge_repl[4] / 3;
	index_avg_sc = total_index_repl[4] / 3;


	if(bubble_avg_lru <= bubble_avg_nru && bubble_avg_lru <= bubble_avg_fifo &&
		bubble_avg_lru <=  bubble_avg_wsc && bubble_avg_lru <= bubble_avg_sc){
		cout << "Bubble sort Average " << bubble_avg_lru << " for LRU\n";
	} 

	if(bubble_avg_nru <= bubble_avg_lru && bubble_avg_nru <= bubble_avg_fifo &&
		bubble_avg_nru <=  bubble_avg_wsc && bubble_avg_nru <= bubble_avg_sc){
		cout << "Bubble sort Average " << bubble_avg_nru << " for NRU\n";
	} 

	if(bubble_avg_wsc <= bubble_avg_nru && bubble_avg_wsc <= bubble_avg_fifo &&
		bubble_avg_wsc <=  bubble_avg_lru && bubble_avg_wsc <= bubble_avg_sc){
		cout << "Bubble sort Average " << bubble_avg_wsc << " for WSClock\n";
	} 
	if(bubble_avg_sc <= bubble_avg_nru && bubble_avg_sc <= bubble_avg_fifo &&
		bubble_avg_sc <=  bubble_avg_wsc && bubble_avg_sc <= bubble_avg_lru){
		cout << "Bubble sort Average " << bubble_avg_sc << " for SC\n";
	} 
	if(bubble_avg_fifo <= bubble_avg_nru && bubble_avg_fifo <= bubble_avg_lru &&
		bubble_avg_fifo <=  bubble_avg_wsc && bubble_avg_fifo <= bubble_avg_sc){
		cout << "Bubble sort Average " << bubble_avg_fifo << " for FIFo\n";
	} 

	/////////////////////////////////////////////////////

	if(quick_avg_lru <= quick_avg_nru && quick_avg_lru <= quick_avg_fifo &&
		quick_avg_lru <=  quick_avg_wsc && quick_avg_lru <= quick_avg_sc){
		cout << "Quick sort Average " << quick_avg_lru << " for LRU\n";
	} 

	if(quick_avg_nru <= quick_avg_lru && quick_avg_nru <= quick_avg_fifo &&
		quick_avg_nru <=  quick_avg_wsc && quick_avg_nru <= quick_avg_sc){
		cout << "Quick sort Average " << quick_avg_nru << " for NRU\n";
	} 

	if(quick_avg_wsc <= quick_avg_nru && quick_avg_wsc <= quick_avg_fifo &&
		quick_avg_wsc <=  quick_avg_lru && quick_avg_wsc <= quick_avg_sc){
		cout << "Quick sort Average " << quick_avg_wsc << " for WSClock\n";
	} 
	if(quick_avg_sc <= quick_avg_nru && quick_avg_sc <= quick_avg_fifo &&
		quick_avg_sc <=  quick_avg_wsc && quick_avg_sc <= quick_avg_lru){
		cout << "Quick sort Average " << quick_avg_sc << " for SC\n";
	} 
	if(quick_avg_fifo <= quick_avg_nru && quick_avg_fifo <= quick_avg_lru &&
		quick_avg_fifo <=  quick_avg_wsc && quick_avg_fifo <= quick_avg_sc){
		cout << "Quick sort Average " << quick_avg_fifo << " for FIFo\n";
	} 

	////////////////////////////////////////////////////////////////

	if(merge_avg_lru <= merge_avg_nru && merge_avg_lru <= merge_avg_fifo &&
		merge_avg_lru <=  merge_avg_wsc && merge_avg_lru <= merge_avg_sc){
		cout << "Merge sort Average " << merge_avg_lru << " for LRU\n";
	} 

	if(merge_avg_nru <= merge_avg_lru && merge_avg_nru <= merge_avg_fifo &&
		merge_avg_nru <=  merge_avg_wsc && merge_avg_nru <= merge_avg_sc){
		cout << "Merge sort Average " << merge_avg_nru << " for NRU\n";
	} 

	if(merge_avg_wsc <= merge_avg_nru && merge_avg_wsc <= merge_avg_fifo &&
		merge_avg_wsc <=  merge_avg_lru && merge_avg_wsc <= merge_avg_sc){
		cout << "Merge sort Average " << merge_avg_wsc << " for WSClock\n";
	} 
	if(merge_avg_sc <= merge_avg_nru && merge_avg_sc <= merge_avg_fifo &&
		merge_avg_sc <=  merge_avg_wsc && merge_avg_sc <= merge_avg_lru){
		cout << "Merge sort Average " << merge_avg_sc << " for SC\n";
	} 
	if(merge_avg_fifo <= merge_avg_nru && merge_avg_fifo <= merge_avg_lru &&
		merge_avg_fifo <=  merge_avg_wsc && merge_avg_fifo <= merge_avg_sc){
		cout << "Merge sort Average " << merge_avg_fifo << " for FIFo\n";
	} 

	//****************************************************************

	if(index_avg_lru <= index_avg_nru && index_avg_lru <= index_avg_fifo &&
		index_avg_lru <=  index_avg_wsc && index_avg_lru <= index_avg_sc){
		cout << "Index sort Average " << index_avg_lru << " for LRU\n";
	} 

	if(index_avg_nru <= index_avg_lru && index_avg_nru <= index_avg_fifo &&
		index_avg_nru <=  index_avg_wsc && index_avg_nru <= index_avg_sc){
		cout << "Index sort Average " << index_avg_nru << " for NRU\n";
	} 

	if(index_avg_wsc <= index_avg_nru && index_avg_wsc <= index_avg_fifo &&
		index_avg_wsc <=  index_avg_lru && index_avg_wsc <= index_avg_sc){
		cout << "Index sort Average " << index_avg_wsc << " for WSClock\n";
	} 
	if(index_avg_sc <= index_avg_nru && index_avg_sc <= index_avg_fifo &&
		index_avg_sc <=  index_avg_wsc && index_avg_sc <= index_avg_lru){
		cout << "Index sort Average " << index_avg_sc << " for SC\n";
	} 
	if(index_avg_fifo <= index_avg_nru && index_avg_fifo <= index_avg_lru &&
		index_avg_fifo <=  index_avg_wsc && index_avg_fifo <= index_avg_sc){
		cout << "Index sort Average " << index_avg_fifo << " for FIFo\n";
	} 

	
	return 0;
}

void program_run()
{
	create_memories();

	// Bubble sort, quick sort, merge sort,index sort

	thread bubble(thread1_sort, frameSize, numVirtual);
	thread quick(thread2_sort, frameSize, numVirtual);
	thread merge(thread3_sort, frameSize, numVirtual);
	thread index(thread4_sort, frameSize, numVirtual);

	bubble.join();
	quick.join();
	merge.join();
	index.join();



//	check_virtual_sorted();

	program_statistics();

	

	clear_memories();
}

void optimal()
{



	cout << "Bubble Average Page Replacement: "<< bubble_best <<" Algorithm: " << algorithm << endl;
	cout << "Quick Average  Page Replacement: "<< quick_best << " Algorithm: " << algorithm << endl;
	cout << "Merge Average  Page Replacement: "<< merge_best << " Algorithm: " << algorithm << endl;
	cout << "Index Average  Page Replacement: "<< index_best << " Algorithm: " << algorithm << endl;
}

void program_statistics()
{
	cout << "Bubble Sort Page Replacement: " << bubble_page_repl << " Algorithm: " << algorithm << endl;
	cout << "Qucik Sort Page Replacement : " << quick_page_repl << " Algorithm: " << algorithm << endl;
	cout << "Merge Sort Page Replacement : " << merge_page_repl << " Algorithm: " << algorithm << endl;
	cout << "Index Sort Page Replacement : " << index_page_repl << " Algorithm: " << algorithm << endl;
	cout << endl;

	bubble_best += bubble_page_repl;
	quick_best += quick_page_repl;
	merge_best += merge_page_repl;
	index_best += index_page_repl;

	bubble_page_repl = 0;
	quick_page_repl = 0;
	merge_page_repl = 0;
	index_page_repl = 0;
}




void set(unsigned int index, int value, char * tName)
{
	mtx.lock();
	number_of_write++;
	int c = -2;

	if (string_compare(allocPolicy,"local") == 1)
	{
		c = check_area(tName, index);
		if(c != 1)
		{
			cout << "Bu index \"" << index << "\", bu thread \""<< tName <<"\" tarafında set fonksiyonuna erişilemez.\n";
			mtx.unlock();
			return ;
		}
	}

	
	if(c == 1)
	{

		int page = int(index / frameSize);
		int page_index = int(index % frameSize);
//		cout << index <<" in page i  >" << page << endl;

		// PAGE TABLE DA BOS YER VAR MI?
		int pt_free = empty_entry(page_table);	// page table index
//		cout << "int set page tabledaki boş index : " << pt_free << endl;
		// -1 gelirse page table dolu. 

		// GELEN PAGE DAHA ONCEDEN PAGE TABLE DA VAR MI?
		int in_pt = checkin_page_table(page_table, page);	// physical address bulur
		// page fault ve HIT kontrolü
		// in_pt == -1 ise page table da yok
		// in_pt == fiziksel memory adresi
//		cout << "in set Gelen page önceden table da varmı :" << in_pt << endl;


		// GELEN PAGE ONCEDEN VARSA HIT OLUR. SADECE GUNCELLEME YAP
		if(in_pt != -1)	// HIT
		{	// bu page önceden page table da var. sadece gerekli satırı güncelledik.
//			cout << "HIIIT OLDU\n";

			hit_counter++;
			virtual_memory[page].frame[page_index] = value;
			physical_memory[in_pt].frame[page_index] = value;
			page_table[in_pt].referance = true;
			page_table[in_pt].modified = true;
			//physical_memory[in_pt].modified = true;
			update_lru_index(page_table, in_pt);
			update_lru_index_phy(physical_memory, in_pt);
		}
		else if(in_pt == -1 && pt_free != -1) // page table da daha önceden yok. VE page table boş
		{
//			cout << "Ppage table da daha önceden yok.\n";
			////mtx.lock();
			page_fault_counter++;
			//print_page_table(page_table);

			// PAGE FAULT - page e ilk kez geliyo
		
//			cout << "PAGE FAULT in set\n";

			// find free space in physical memory
			int fm_free = check_physical_memory();
//			cout << "physical_memory deki boş index: " << fm_free << endl;
//			cout << "Fizikseldeki boş yer: set 1"<< fm_free << endl;
			if(fm_free == -1)
			{
				fm_free = discharge_physical_memory();
//				cout << fm_free << " Fiziksel Memory DOLU in set 1\n";
				go_to_disk(fm_free);
			}
			

			//cout << "Before set : " <<  virtual_memory[page].frame[page_index] << endl;			
			virtual_memory[page].frame[page_index] = value;
			//cout << "After set : " <<  virtual_memory[page].frame[page_index] << endl;
//			cout << "update_page_table 1\n";
			update_page_table(page_table,pt_free, page, fm_free);
			page_table[pt_free].referance = true;
//			page_table[pt_free].modified = true;
			//physical_memory[in_pt].modified = true;
//			cout << "update_physical_memory 1\n";
			update_physical_memory(page, fm_free);
			physical_memory[fm_free].modified = true;
			////mtx.unlock();
			//print_physical_memory(fm_free);
			//print_page_table(page_table);
		}	
		else if(in_pt == -1 && pt_free == -1){	// PAGE TABLE DA ONCEDEN YOK. VE PAGE TABLE DOLU
//			cout << "in set page replacement algo : "<< algorithm << endl;
			////mtx.lock();
			page_fault_counter++;
			number_of_page_replacements++;

			if(string_compare(tName, "Bubble") == 1)
				bubble_page_repl++;
			else if(string_compare(tName, "Quick") == 1)
				quick_page_repl++;
			else if(string_compare(tName, "Merge") == 1)
				merge_page_repl++;
			else if(string_compare(tName, "Index") == 1)
				index_page_repl++;

			int remove = find_page_algorithm();
			virtual_memory[page].frame[page_index] = value;		
			int fm_free = check_physical_memory();

//			cout << "Fizikseldeki boş yer: set 2 "<< fm_free << endl;
			if(fm_free == -1)
			{
				fm_free = discharge_physical_memory();
//				cout << fm_free << " Fiziksel Memory DOLU in set 2\n";
				go_to_disk(fm_free);
			}
//			cout << "update_page_table 2\n";
			update_page_table(page_table, remove, page, fm_free);
			//page_table[remove].modified = true;
			page_table[remove].referance = true;
//			cout << "update_physical_memory 2\n";
			update_physical_memory(page, fm_free);
			physical_memory[fm_free].modified = true;
		}
	}
	mtx.unlock();
	//print_page_table(page_table);
}


int discharge_physical_memory()
{
	/*
	int remove_page = -1;
	if (string_compare(algorithm,"LRU") == 1)
	{
		remove_page = LRU_physical(physical_memory, numPhysical);
	}
	else if (string_compare(algorithm,"NRU") == 1)
	{
		remove_page = NRU_physical(physical_memory, numPhysical);
	}
	else if (string_compare(algorithm,"FIFO") == 1)
	{
		remove_page = FIFO_physical(physical_memory, numPhysical);
	}
	else if (string_compare(algorithm,"WSClock") == 1)
	{
		remove_page = WSClock_physical(physical_memory, numPhysical);
	}
	else if (string_compare(algorithm,"SC") == 1)
	{
		remove_page = SC_physical(physical_memory, numPhysical);
	}
	physical_memory[remove_page].empty = false;
	return remove_page;*/

	for (int i = 0; i < page_table_size; ++i)
	{
		if(page_table[i].present == false){
			return page_table[i].physical_address;
		}
	}
}

void go_to_disk(int fm_address)
{
//	cout << "Diske yaz: "<< fm_address << endl;
	number_of_disk_page_writes++;
	fstream disk;
	disk.open(diskFileName, fstream::out | fstream::app);
	disk << "Index in Physical Memory: " << fm_address << endl;
	for (int i = 0; i < numPhysical; ++i)
	{
		disk << physical_memory[fm_address].frame[i] << endl;
	}
	//disk.close();
}

int find_page_algorithm()
{
	int remove_page = -1;
	if (string_compare(algorithm,"LRU") == 1)
	{
		remove_page = LRU(page_table, page_table_size);
	}
	else if (string_compare(algorithm,"NRU") == 1)
	{
		remove_page = NRU(page_table, page_table_size);
	}
	else if (string_compare(algorithm,"FIFO") == 1)
	{
		remove_page = FIFO(page_table, page_table_size);
	}
	else if (string_compare(algorithm,"WSClock") == 1)
	{
		remove_page = WSClock(page_table, page_table_size);
	}
	else if (string_compare(algorithm,"SC") == 1)
	{
		remove_page = SC(page_table, page_table_size);
	}


//	cout << endl << algorithm <<" ile çıkartılacak Page: " << remove_page << endl;
/*
	cout << "Referance: " << page_table[remove_page].referance << endl;
	cout << "Modified: " << page_table[remove_page].modified << endl;
	cout << "Present: " << page_table[remove_page].present << endl;
	cout << "Index_fifo: " << page_table[remove_page].index_fifo << endl;
	cout << "Index_lru: " << page_table[remove_page].index_lru << endl;
	cout << "Physical Address: " << page_table[remove_page].physical_address << endl;
	cout << "Virtual Address: " << page_table[remove_page].virtual_address << endl;
*/
	page_table[remove_page].present = false;
	//physical_memory[remove_page].empty = false;



	return remove_page;
	
}

int get(unsigned int index, char * tName)
{
	mtx.lock();
	number_of_read++;

	if (string_compare(allocPolicy,"local") == 1)
	{
		int c = check_area(tName, index);
		if(c != 1)
		{
			cout << "Bu index \"" << index << "\", bu thread \""<< tName <<"\" tarafında get fonksiyonuna erişilemez.\n";
			mtx.unlock();
			return -1;
		}
	}
//	cout << "index " << index << "fram" << frameSize<< endl;
	////mtx.lock();
	int page = int(index / frameSize);
//	cout << index <<" : Kaçıncı page de? = " << page << endl;
	int page_index = int(index % frameSize);
//	cout << "Pagedeki indexi : " << page_index << endl;
//	cout << "Kim erişecek: " << tName << endl;
	
	
	// PAGE TABLE DA BOS YER VAR MI?
	int pt_free = empty_entry(page_table);	// page table index
//	cout << "in get page tabledaki boş index : " << pt_free << endl;
	// -1 gelirse page table dolu. 

	// GELEN PAGE DAHA ONCEDEN PAGE TABLE DA VAR MI?
	int in_pt = checkin_page_table(page_table, page);	// physical address bulur
	// page fault ve HIT kontrolü
	// in_pt == -1 ise page table da yok
	// in_pt == fiziksel memory adresi
//	cout << "in get Gelen page önceden table da varmı :" << in_pt << endl;
	////mtx.unlock();

	// GELEN PAGE ONCEDEN VARSA HIT OLUR. SADECE GUNCELLEME YAP
	if(in_pt != -1)	// HIT
	{	// bu page önceden page table da var. sadece gerekli satırı güncelledik.
//		cout << "HIIIT OLDU in get\n";
		////mtx.lock();
		hit_counter++;
		//virtual_memory[page].frame[page_index] = value;
		//physical_memory[in_pt].frame[page_index] = value;
		//update_lru_index(page_table, in_pt);
		page_table[in_pt].referance = true;
		mtx.unlock();
		//cout << "Get return : " << physical_memory[in_pt].frame[page_index] << endl;
		return physical_memory[in_pt].frame[page_index];
	}
	else if(in_pt == -1 && pt_free != -1) // page table da daha önceden yok. VE page table da yer var
	{
	//	cout << "in get page table da daha önceden yok.\n";
		////mtx.lock();
		page_fault_counter++;
		//print_page_table(page_table);


		// PAGE FAULT - page e ilk kez geliyo
	
//		cout << "PAGE FAULT in get\n";


		// find free space in physical memory
		int fm_free = check_physical_memory();
//		cout << "in get physical_memory deki boş index: " << fm_free << endl;

//		cout << "Fizikseldeki boş yer: get 1 "<< fm_free << endl;
		if(fm_free == -1)
		{
			fm_free = discharge_physical_memory();
//			cout << fm_free << " Fiziksel Memory DOLU int get 1\n";
			go_to_disk(fm_free);
		}

		// page table daki boş yere yaz.
		
		update_page_table(page_table,pt_free, page, fm_free);
		page_table[pt_free].referance = true;
		// virtual deki frame i physical a yaz komple. 4096int
		update_physical_memory(page, fm_free);
		physical_memory[fm_free].referance = true;
		mtx.unlock();
		//cout << "Get return : " << virtual_memory[page].frame[page_index] << endl;
		return physical_memory[fm_free].frame[page_index];
	}
	else if(in_pt == -1 && pt_free == -1) // PAGE TABLE DA ONCEDEN YOK. VE PAGE TABLE DOLU
	{	
		////mtx.lock();
//		cout << "page replacement algo : "<< algorithm << endl;
		page_fault_counter++;
		number_of_page_replacements++;

		if(string_compare(tName, "Bubble") == 1)
			bubble_page_repl++;
		else if(string_compare(tName, "Quick") == 1)
			quick_page_repl++;
		else if(string_compare(tName, "Merge") == 1)
			merge_page_repl++;
		else if(string_compare(tName, "Index") == 1)
			index_page_repl++;

		int remove = find_page_algorithm();
		int fm_free = check_physical_memory();

//		cout << "Fizikseldeki boş yer: get 2 "<< fm_free << endl;
		if(fm_free == -1)
		{
			fm_free = discharge_physical_memory();
//			cout << fm_free <<" Fiziksel Memory DOLU int get 2\n";
			go_to_disk(fm_free);
		}

		update_page_table(page_table,remove, page, fm_free);
		page_table[remove].referance = true;
//		cout << "remove: "<< remove << endl;
		update_physical_memory(page, fm_free);
		physical_memory[fm_free].referance = true;
//		cout << "asdsa \n";
		mtx.unlock();
		return physical_memory[fm_free].frame[page_index];
	}
	mtx.unlock();
}


int check_area(char * tName, int index)
{
	if(string_compare(tName, "Merge") == 1)
	{
		if(index >= merge_start*frameSize && index < (merge_end+1)*frameSize)
			return 1;
	}
	else if(string_compare(tName, "Quick") == 1)
	{
		if(index >= quick_start*frameSize && index < (quick_end+1)*frameSize)
			return 1;
	}
	else if(string_compare(tName, "Index") == 1)
	{
		if(index >= index_start*frameSize && index < (index_end+1)*frameSize)
			return 1;
	}
	else if(string_compare(tName, "Bubble") == 1)
	{
		if(index >= bubble_start*frameSize && index < (bubble_end+1)*frameSize)
			return 1;
	}
	return -1;
}

int string_compare(const char* str1, const char* str2)
{
	for (int i = 0; i < strlen(str1); ++i)
	{
		if(str1[i] != str2[i])
			return -1;
	}
	return 1;
}


// komple page i virtual mem den physical mem e at
void update_physical_memory(int virtual_add, int physical_add)
{
	//int page = virtual_add / frameSize;
	//int phy_page = physical_add / frameSize;
	physical_memory[physical_add].empty = true;
	physical_memory[physical_add].index_lru++;
	physical_memory[physical_add].index_fifo++;

	for (int i = 0; i < frameSize; ++i)
	{
//		cout << "phy: " <<  virtual_memory[virtual_add].frame[i] << endl;
		physical_memory[physical_add].frame[i] = virtual_memory[virtual_add].frame[i];
		//cout << i <<" virtual  mem : " << virtual_memory[page].frame[i] << endl;
	}
	//cout << "update_physical_memory\n";
}



int check_physical_memory()
{
	for(int i = 0; i < numPhysical; i++)
	{		
		if(physical_memory[i].empty == false){
			//go_to_disk(i);
			return i;
		}
	}
	return -1;
}

void print_physical_memory(int ix)
{
	//for(int i = 0; i < numPhysical; i++)
	{
		for(int j = 0; j < frameSize; j++)
		{
			cout << j <<" fiziksel mem : " << physical_memory[ix].frame[j] << endl;

		}
	}
}


void check_virtual_sorted()
{
	for (int i = 0; i < numVirtual; ++i)
	{
		for (int j = 0; j < frameSize-1; ++j)
		{
			if(virtual_memory[i].frame[j] > virtual_memory[i].frame[j+1])
			{
				cout << virtual_memory[i].frame[j] << " > " << virtual_memory[i].frame[j+1] << endl;
				cout << "Sort edilmemiş.\n";
				cout << "i : " <<  i << " j : " <<  j << endl;
				return;
			}
		}
		if(i == bubble_end)
			cout << "Bubble sort Doğru Sıralamış\n";
		else if(i == quick_end)
			cout << "Quick sort Doğru Sıralamış\n";
		else if(i == merge_end)
			cout << "Merge sort Doğru Sıralamış\n";
		else if(i == index_end)
			cout << "Index sort Doğru Sıralamış\n";
	}
	cout << "KOMPLE SORT EDİLMİŞ\n";
}



//Bubble sort
// Bubble sort for thread1
// 0 - 255
void thread1_sort(int frameSize, int pageSize)
{
	//mtx.lock();
	thread::id this_id = std::this_thread::get_id();
//	cout << "bubble Thread1 ID: " << this_id << endl;
	bubble_start = 0;
	bubble_end = pageSize/4 -1;
//	cout << "bubble sort işlem aralığı: " << bubble_start << " - " << bubble_end << endl;
	// start time
	auto start = high_resolution_clock::now();
	for (int i = bubble_start; i <= bubble_end; ++i)
	{
//		cout << "bubble index" << i << endl;
		bubble_sort(virtual_memory[i].frame, frameSize, i);
	}

//	bubble_sort(virtual_memory[60].frame, 60, frameSize);
	// end time
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	//cout << "bubble Sort Run Time: " << duration.count() << " milliseconds\n";
	//mtx.unlock();
}

// Quick
// 256 - 511
void thread2_sort(int frameSize, int pageSize)
{
	//mtx.lock();
	thread::id this_id = std::this_thread::get_id();
//	cout << "Quick Thread2 ID: " << this_id << endl;
	quick_start = pageSize/4;
	quick_end = pageSize/4 + pageSize/4 -1;
//	cout << "Quick Sort işlem aralığı: " << quick_start << " - " << quick_end << endl;
	// start time
	auto start = high_resolution_clock::now();
	
	for (int i = quick_start; i <= quick_end; ++i)
	{
		quick_sort(virtual_memory[i].frame, 0, frameSize-1, i, frameSize);
	}
	
	// end time
//	quick_sort(virtual_memory[60].frame, 0, frameSize, 60, frameSize);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	//cout << "Quick Sort Run Time: " << duration.count() << " milliseconds\n";
	//mtx.unlock();
}

// merge sort,
// 512 - 767
void thread3_sort(int frameSize, int pageSize)
{
	//mtx.lock();
	thread::id this_id = std::this_thread::get_id();
//	cout << "merge Thread3 ID: " << this_id << endl;
	merge_start = pageSize/2;
	merge_end = (pageSize / 2)+ (pageSize / 4) - 1;
//	cout << "merge sort işlem  aralığı: " << merge_start << " - " << merge_end << endl;
	// start time
	auto start = high_resolution_clock::now();
	
	for (int i = merge_start; i <= merge_end; ++i)
	{
		merge_sort(virtual_memory[i].frame, i, frameSize);
	}
	
	// end time
	//virtual_memory[60].frame = merge_sort(virtual_memory[60].frame, frameSize, 60);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	//cout << "merge Sort Run Time: " << duration.count() << " milliseconds\n";
	//mtx.unlock();
}

// index sort
// 768 - 1023
void thread4_sort(int frameSize, int pageSize)
{
	//mtx.lock();
	thread::id this_id = std::this_thread::get_id();
//	cout << "index Thread4 ID: " << this_id << endl;
	index_start = 3 * (pageSize/4);
	index_end = 3 * (pageSize/4) + pageSize/4 - 1;
//	cout << "Index Sort işlem aralığı: " << index_start << " - " << index_end << endl;
	// start time
	auto start = high_resolution_clock::now();
	
	for (int i = index_start; i <= index_end; ++i)
	{
		virtual_memory[i].frame = index_sort(virtual_memory[i].frame, frameSize, i);
	}
	
	//index_sort(virtual_memory[60].frame, frameSize,60);
	// end time
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	//cout << "Index Sort Run Time: " << duration.count() << " milliseconds\n";
	//mtx.unlock();
}



void fill_virtual_memory(int frameSize, int page_sayisi)
{
	
	for(int i = 0; i < page_sayisi; i++)
	{
		virtual_memory[i].frame = (int*)malloc(frameSize * sizeof(int));
		for(int j = 0; j < frameSize; j++)
		{
			virtual_memory[i].frame[j] = rand();
		}
	}
}

void fill_physical_memory(int frameSize, int page_sayisi)
{
	for(int i = 0; i < page_sayisi; i++)
	{
		physical_memory[i].frame = (int*)malloc(frameSize * sizeof(int));
		physical_memory[i].empty = false;
		physical_memory[i].referance = false;	// nru - 2nd
		physical_memory[i].modified = false;	// nru
		physical_memory[i].index_fifo = -1;
		physical_memory[i].index_lru = -1;

		for(int j = 0; j < frameSize; j++)
		{
			physical_memory[i].frame[j] = -1;
		}
	}
}



void create_memories()
{
	virtual_memory  = (Page *) malloc(sizeof(struct Page) * numVirtual);	// 1024 * 4096
	physical_memory = (Page *) malloc(sizeof(struct Page) * numPhysical);	// 32	* 4096
	page_table = (Entry *) malloc(sizeof(struct Entry) * page_table_size);	// 8
	//page_table = new Entry[page_table_size];
	srand(1000);
	fill_virtual_memory(frameSize, numVirtual);
	fill_physical_memory(frameSize, numVirtual);
	create_page_table(page_table, page_table_size, pageTablePrintInt);
}

void clear_memories()
{
	//free(page_table);
/*
	for (int i = 0; i < numVirtual; ++i){
		free(virtual_memory[i].frame);
	}
	free(virtual_memory);
	
	for (int i = 0; i < numPhysical; ++i){
		free(physical_memory[i].frame);
	}
	
	free(physical_memory);
*/
}

