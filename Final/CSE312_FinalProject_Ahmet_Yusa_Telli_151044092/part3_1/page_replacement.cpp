#include "page_replacement.h"


int LRU(Entry * page_table, int pt_size)
{
	int min_index = 99999999;
	int remove_index = -1;
	for (int i = 0; i < pt_size; ++i)
	{
		if (page_table[i].index_lru < min_index)
		{
			min_index = page_table[i].index_lru;
			remove_index = i;
		}
	}
	return remove_index;
}

// class 0 = 0 0
// class 1 = 1 0
// class 2 = 0 1
// class 3 = 1 1
int NRU(Entry * page_table, int pt_size)
{
	// check class 0
	for (int i = 0; i < pt_size; ++i)
	{
		if(page_table[i].referance == false && page_table[i].modified == false)		// class 0
			return i;
	}

	// check class 1
	for (int i = 0; i < pt_size; ++i)
	{
		if(page_table[i].referance == false && page_table[i].modified == true)		// class 1
			return i;
	}

	// check class 2
	for (int i = 0; i < pt_size; ++i)
	{
		if(page_table[i].referance == true && page_table[i].modified == false)		// class 2
			return i;
	}

	// check class 3
	for (int i = 0; i < pt_size; ++i)
	{
		if(page_table[i].referance == true && page_table[i].modified == true)		// class 3
			return i;
	}
}




int FIFO(Entry * page_table, int pt_size)
{
	int min_index = 99999999;
	int remove_this_page = -1;
	for (int i = 0; i < pt_size; ++i)
	{
		if (page_table[i].index_fifo < min_index)
		{
			min_index = page_table[i].index_fifo;
			remove_this_page = i;
		}
	}
	return remove_this_page;

}

int find_min_fifo_index(Entry * page_table, int pt_size)
{
	int min_index = 99999999;
	int j = -1;
	for (int i = 0; i < pt_size; ++i)
	{
		if (page_table[i].index_fifo < min_index)
		{
			min_index = page_table[i].index_fifo;
			j = i;
		}
	}
	return j;
}


int SC(Entry * page_table, int pt_size)
{
	int min_index = 99999999;
	int j = -1;
	for (int i = 0; i < pt_size; ++i)
	{
		if (page_table[i].index_fifo < min_index)
		{
			if(page_table[i].referance == false){
				min_index = page_table[i].index_fifo;
				j = i;
			}
			else
				page_table[i].referance = false;
		}
		
	}
	if(j == -1)
		return 0;
	return j;
}

int victim = 0;

int WSClock(Entry * page_table, int pt_size)
{
//	cout << "Second Chance Algo " << victim << endl;
	if(victim >= pt_size)
		victim = 0;
	while(victim < pt_size)
	{
//		cout << "victim: ++ " << victim <<" - ref: " << page_table[victim].referance <<  endl;
		if(page_table[victim].referance == false){
			
			return victim++;
		}
		else if(page_table[victim].referance == true){
			page_table[victim].referance = false;
		}
		
		victim++;

		if(victim >= pt_size)
			victim = 0;
	}
}





int LRU_physical(Page * physical_memory, int pt_size)
{
	int min_index = 99999999;
	int remove_index = -1;
	for (int i = 0; i < pt_size; ++i)
	{
		if (physical_memory[i].index_lru < min_index)
		{
			min_index = physical_memory[i].index_lru;
			remove_index = i;
		}
	}
	return remove_index;
}

// class 0 = 0 0
// class 1 = 1 0
// class 2 = 0 1
// class 3 = 1 1
int NRU_physical(Page * physical_memory, int pt_size)
{
	// check class 0
	for (int i = 0; i < pt_size; ++i)
	{
		if(physical_memory[i].referance == false && physical_memory[i].modified == false)		// class 0
			return i;
	}

	// check class 1
	for (int i = 0; i < pt_size; ++i)
	{
		if(physical_memory[i].referance == false && physical_memory[i].modified == true)		// class 1
			return i;
	}

	// check class 2
	for (int i = 0; i < pt_size; ++i)
	{
		if(physical_memory[i].referance == true && physical_memory[i].modified == false)		// class 2
			return i;
	}

	// check class 3
	for (int i = 0; i < pt_size; ++i)
	{
		if(physical_memory[i].referance == true && physical_memory[i].modified == true)		// class 3
			return i;
	}
}




int FIFO_physical(Page * physical_memory, int pt_size)
{
	int min_index = 99999999;
	int remove_this_page = -1;
	for (int i = 0; i < pt_size; ++i)
	{
		if (physical_memory[i].index_fifo < min_index)
		{
			min_index = physical_memory[i].index_fifo;
			remove_this_page = i;
		}
	}
	return remove_this_page;

}




int WSClock_physical(Page * physical_memory, int pt_size)
{


}

int victim2 = 0;

int SC_physical(Page * physical_memory, int pt_size)
{
//	cout << "Second Chance Algo " << victim2 << endl;
	if(victim2 >= pt_size)
		victim2 = 0;
	while(victim2 < pt_size)
	{
//		cout << "victim2: ++ " << victim2 <<" - ref: " << physical_memory[victim2].referance <<  endl;
		if(physical_memory[victim2].referance == false){
			
			return victim2++;
		}
		else if(physical_memory[victim2].referance == true){
			physical_memory[victim2].referance = false;
		}
		
		victim2++;

		if(victim2 >= pt_size)
			victim2 = 0;
	}
}

void update_lru_index_phy(Page * physical_memory, int index)
{
	physical_memory[index].index_lru++;
}