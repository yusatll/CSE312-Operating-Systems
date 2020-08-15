#include "sort.h"

void swap(int* x, int* y)  
{  
    int k = *x;  
    *x = *y;  
    *y = k;  
}  





void merge_sort(int arr[], int frame_index, int frameSize)
{
	//cout << "ne lan bu nerde bu.MERGE SORT\n";
	int size, left_in;
	for(size = 1; size <= frameSize - 1; size = 2 * size)
	{
		for(left_in = 0; left_in <=  frameSize-1; left_in += 2 * size)
		{
			int middle = left_in + size -1;
			int right_in;

			if(left_in + 2 * size - 1 > frameSize - 1)
				right_in = frameSize-1;
			else
				right_in = left_in + 2 * size - 1;

			merge(arr, left_in, middle, right_in, frame_index, frameSize);
		}
	}
}

void array_doldur(int size1, int arr1[], int size2, int arr2[])
{
	for (int i = 0; i < size1; ++i)
	{
		arr1[i] = arr2[size2 + i];
	}	
}

void kalan_tamamlama(int in1, int in2, int in3, int arr1[], int arr2[])
{
	while(in1 < in3)
	{
		arr2[in2] = arr1[in1];
		in1++;
		in2++;
	}
}

	

void merge(int arr[], int left_in, int mid_in, int right_in,int frame_index, int frameSize)
{
	char m_ptr[6] = "Merge";
	int i,j;

	int side1 = mid_in - left_in + 1;
	int side2 = right_in - mid_in;
	int left_arr[side1], right_arr[side2];


	for (i = 0; i < side1; ++i)
		left_arr[i] = get(left_in + i + (frameSize*frame_index), m_ptr);
		//left_arr[i] = arr[left_in + i];

	for (j = 0; j < side2; ++j)
		right_arr[j] = get(mid_in + 1 + j + (frameSize*frame_index), m_ptr);
		//right_arr[i] = arr[mid_in + 1 + i];
	


//	array_doldur(side1, left_arr, left_in, arr);
//	array_doldur(side2, right_arr, mid_in+1, arr);

	i=0,j=0;
	int k=left_in;

	while(i < side1 && j < side2)
	{
		if (left_arr[i] <= right_arr[j])
		{
			set(k + (frameSize*frame_index), left_arr[i], m_ptr);
			//arr[k] = left_arr[i];
			k++;
			i++;
		}
		else
		{
			set(k + (frameSize*frame_index), right_arr[j], m_ptr);
			//arr[k] = right_arr[j];
			k++;
			j++;
		}
	}

	// in1, in2, in3, arr1, arr2
	while(i < side1)
	{
		set(k + (frameSize*frame_index), left_arr[i], m_ptr);
		//arr[k] = left_arr[i];
		i++;
		k++;
	}

	while(j < side2)
	{
		set(k + (frameSize*frame_index), right_arr[j], m_ptr);
		//arr[k] = right_arr[j];
		j++;
		k++;
	}

//	kalan_tamamlama(i, k, side1, left_arr, arr);
//	kalan_tamamlama(j, k, side2, right_arr, arr);

}




void quick_sort(int* arr, int left_in, int right_in, int frame_index, int frameSize)
{
	//cout << "QUICK SORT\n";
	char q_ptr[6] = "Quick";
	int pivot = 0, i, j, k;
	int int2;
	int carpim = frame_index*frameSize;
	//cout << "Çarpım: "<< carpim << endl;
	
	if(left_in < right_in)
	{
		pivot = left_in;
		i = left_in;
		j = right_in;

		while(i < j)
		{
			int2 = get(pivot + carpim, q_ptr);
			int int4 = get(i + carpim, q_ptr);
			while(int4 <= int2 && i < right_in) 
			//while(arr[i] <= arr[pivot] && i < right_in)
			{
				i++;
				int4 = get(i + carpim, q_ptr);
			}
			//cout << "1carim + i: " << carpim + i  << endl;
			//cout << "2carim + j: " << carpim + j  << endl;
			int int5 = get(j+carpim, q_ptr);
			while(int5 > int2)
			//while(arr[j] > arr[pivot])
			{
				j--;
				int5 = get(j + carpim, q_ptr);
			}
			//cout << "3carim + j: " << carpim + j  << endl;
			if(i < j)
			{
				//swap(arr[i], arr[j]);
				int int1 = get(i + carpim, q_ptr);
				int int3 = get(j + carpim, q_ptr);
				set(i + carpim, int3, q_ptr);
				set(j + carpim, int1, q_ptr);
			}
//			cout << "4carim + i: " << carpim + i  << endl;
//			cout << "5carim + j: " << carpim + j  << endl;
		}

		//swap(arr[j], arr[pivot]);

		int a = get(j + carpim, q_ptr);
		int b = get(pivot + carpim, q_ptr);

		set(j + carpim, b, q_ptr);
		set(pivot + carpim, a, q_ptr);

		//cout << "6carim + pivot: " << carpim + pivot  << endl;
		//cout << "7carim + j: " << carpim + j  << endl;

		quick_sort(arr, left_in, j-1, frame_index,frameSize);
		quick_sort(arr, j+1, right_in,frame_index,frameSize);
	}

}




// index_size = frameSize
int * index_sort(int *arr, int index_size,int frame_index)
{
	char i_ptr[6] = "Index";
	int *temp_frame = (int*)malloc(sizeof(int) * index_size);
	int* index = (int*)malloc(sizeof(int) * index_size);
	for (int i = 0; i < index_size; ++i)
	{
		index[i] = i;
	}

	for (int i = 0; i < index_size-1; ++i)
	{
		for (int j = i+1; j < index_size; ++j)
		{
			int int1 = get(index[i]+(frame_index*index_size), i_ptr);
			int int2 = get(index[j]+(frame_index*index_size), i_ptr);
			//if (arr[index[i]] > arr[index[j]])
			if(int1 > int2)
			{
				//swap(index[i], index[j]);
				set(index[i]+(frame_index*index_size), int2, i_ptr);
				set(index[j]+(frame_index*index_size), int1, i_ptr);
			}
		}
	}

	for (int i = 0; i < index_size; ++i)
	{
		int t = get(index[i]+(frame_index*index_size), i_ptr);
		temp_frame[i] = t;
	}
	return temp_frame;
	
}



void bubble_sort(int * arr, int size,int frame_index)
{
	char b_ptr[7] = "Bubble";
	for (int i = 1; i < size; ++i)
	{
		for(int j = 0; j < size - i; j++)
		{
			//if (arr[j] > arr[j+1])
			int t1 = get(j+(size*frame_index)+1,b_ptr);
			int t2 = get(j+(size*frame_index),b_ptr);
			if(t2 > t1)
			{
				//swap(arr[j], arr[j+1]);
				set(j+(size*frame_index), t1,b_ptr);
				set(j+(size*frame_index)+1,t2,b_ptr);
			}
		}
	}

}
