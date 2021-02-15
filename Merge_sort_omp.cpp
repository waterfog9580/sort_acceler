#include<iostream>
#include<sys/time.h>
#include<omp.h>
using namespace std;

#define SIZE 100000
#define THREADS 4

int arr[SIZE];

void get_randArr();
void merge_sort(int a[], int numThreads);
void merge_sort(int a[], int low, int high);
void merge(int a[], int left_low, int left_high, int right_low, int right_high);

int main(){
    float sum = 0;
	for (int i = 0; i < 10000; i++)
	{
		struct timeval stime,etime,gap;
		get_randArr();
		
		gettimeofday(&stime,NULL);
    	merge_sort(arr,SIZE,THREADS);
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

void get_randArr(){
	srand((unsigned int)time(NULL));

	for (int i = 0; i < SIZE; i++)
	{
		arr[i] = rand() % SIZE;
	}
}

void merge_sort(int a[], int numThreads) {
    #pragma omp parallel num_threads(numThreads)
    {
        #pragma omp single nowait
        merge_sort(a, 0, SIZE-1);
    }
}

void merge_sort(int a[], int low, int high) {
    if (low >= high)                  //Base case: 1 value to sort->sorted
      return;                         //(0 possible only on initial call)
    else {
      int mid = (low + high)/2;       //Approximate midpoint*
      #pragma omp task
      merge_sort(a, low, mid);        //Sort low to mid part of array
      #pragma omp task
      merge_sort(a, mid+1, high);     //Sort mid+1 to high part of array
      merge(a, low, mid, mid+1,high); //Merge sorted subparts of array
    }
}

void merge(int a[], int left_low, int left_high, int right_low, int right_high) 
{ 
    int length = right_high-left_low+1;
    int temp[length];
    int left = left_low;
    int right = right_low;
    for (int i = 0; i < length; ++i) { 
        if (left > left_high)
            temp[i] = a[right++];
        else if (right > right_high)
            temp[i] = a[left++];
        else if (a[left] <= a[right])
            temp[i] = a[left++];
        else
            temp[i] = a[right++]; 
    }
    
    for (int i=0; i< length; ++i) 
        a[left_low++] = temp[i];
}