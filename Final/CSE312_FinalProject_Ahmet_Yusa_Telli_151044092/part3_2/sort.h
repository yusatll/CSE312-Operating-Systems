#include <iostream>
//#include <fstream>
#include <stdlib.h>
using namespace std;


extern void set(unsigned int index, int value, char * tName);
extern int get(unsigned int index, char * tName);


void fill_virtual_mem();
void merge_sort(int arr[], int left_in, int right_in);
void merge(int arr[], int left_in, int mid_in, int right_in, int frame_index, int frameSize);
void quick_sort(int arr[], int left_in, int right_in,int frame_index, int frameSize);
int* index_sort(int *arr, int index_size,int frame_index);
void bubble_sort(int * arr, int size,int frame_index);


void array_doldur(int size1, int arr1[], int size2, int arr2[]);
void kalan_tamamlama(int in1, int in2, int in3, int arr1, int arr2);
void swap(int* x, int* y);
