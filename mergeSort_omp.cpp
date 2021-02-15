#include <iostream>
#include <unistd.h>
#include <string.h>
#include <omp.h>
#include <sys/time.h>

#define SIZE 10000
#define THREAD 1
#define SWAP(a,b) {int temp = a; a = b; b = temp;}

using namespace std;

void init_List(int arr[]);
void merge(int arr[],int size, int temp[]);
void mergeSort(int arr[],int size, int temp[]);
void mergeSort_omp(int arr[],int size, int temp[], int threads);

int main(int argc,char* argv[]){
    float sum = 0;
    int list[SIZE];
    int temp[SIZE];

	for (int i = 0; i < 10000; i++)
	{
		struct timeval stime,etime,gap;
		init_List(list);
		
		gettimeofday(&stime,NULL);
    	mergeSort_omp(list,SIZE,temp, THREAD);
       	gettimeofday(&etime,NULL);

       	gap.tv_sec = etime.tv_sec - stime.tv_sec;
	   	gap.tv_usec = etime.tv_usec - stime.tv_usec;

       	if(gap.tv_usec < 0){
           	gap.tv_sec = gap.tv_sec-1; 
           	gap.tv_usec = gap.tv_usec + 1000000;
       	}
       	sum += gap.tv_usec;
    }
    cout << sum/10000 << endl;
}

void init_List(int arr[]){
    srand((unsigned int)time(NULL));
	for (int i = 0; i < SIZE; i++)
        arr[i] = rand() % SIZE;
}

void merge(int arr[], int size, int temp[]) {
	int i1 = 0;
	int i2 = size / 2;
	int it = 0;

	while(i1 < size/2 && i2 < size) {
		if (arr[i1] <= arr[i2])
			temp[it] = arr[i1++];
		else
			temp[it] = arr[i2++];
		it++;
	}

	while (i1 < size/2)
	    temp[it++] = arr[i1++];
	while (i2 < size)
	    temp[it++] = arr[i2++];

	memcpy(arr, temp, size*sizeof(int));
}

void mergeSort(int arr[],int size, int temp[]){
    int i;

	if (size == 2) { 
		if (arr[0] <= arr[1])
			return;
		else {
			SWAP(arr[0], arr[1]);
			return;
		}
	}

	mergeSort(arr, size/2, temp);
	mergeSort(arr + size/2, size - size/2, temp);
	merge(arr, size, temp);
}

void mergeSort_omp(int arr[],int size, int temp[], int threads){
    if(threads = 1)
        mergeSort(arr,size,temp);
    else if(threads > 1){
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSort_omp(arr, size/2, temp, threads/2);
            #pragma omp section
            mergeSort_omp(arr+size/2, size-size/2,temp+size/2, threads-threads/2);
        }
    }
    merge(arr, size, temp);
}