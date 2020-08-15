#ifndef STRUCTS_H
#define STRUCTS_H



struct Page
{
	bool referance;	// nru - 2nd
	bool modified;	// nru
	int index_fifo;
	int index_lru;

	int *frame;
	bool empty;
};


struct Entry
{
	bool referance;	// nru - 2nd
	bool modified;	// nru
	bool present;
	int index_fifo;
	int index_lru;
	int physical_address;
	int virtual_address;
};

#endif