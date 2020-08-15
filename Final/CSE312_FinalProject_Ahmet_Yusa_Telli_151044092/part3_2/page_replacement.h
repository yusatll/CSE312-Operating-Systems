#include <iostream>
#include "structs.h"


using namespace std;

int LRU(Entry * page_table, int pt_size);
int NRU(Entry * page_table, int pt_size);
int FIFO(Entry * page_table, int pt_size);
int WSClock(Entry * page_table, int pt_size);
int SC(Entry * page_table, int pt_size);


// helper for fifo
int find_min_fifo_index(Entry * page_table, int pt_size);

// physical memory page replacement
int LRU_physical(Page * physical_memory, int numPhysical);
int NRU_physical(Page * physical_memory, int numPhysical);
int FIFO_physical(Page * physical_memory, int numPhysical);
int WSClock_physical(Page * physical_memory, int numPhysical);
int SC_physical(Page * physical_memory, int numPhysical);
void update_lru_index_phy(Page * page_table, int index);