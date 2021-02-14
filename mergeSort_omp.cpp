#include<iostream>
#include<stdlib.h>
#include<sys/time.h>
#include<omp.h>

#define MAX_SIZE 10000

using namespace std;

int temp_list[MAX_SIZE];

void get_randArr(int (&arr)[MAX_SIZE]);
void merge(int list[], int left, int mid, int right);
void mergeSort_parallel(int list[], int left, int right,int threads);
void mergeSort_parallel_internal(int list[], int left, int right);

void get_randArr(int (&arr)[MAX_SIZE]){
	srand((unsigned int)time(NULL));
	for (int i = 0; i < MAX_SIZE; i++)
	{
		arr[i] = rand() % MAX_SIZE;
	}
}

void merge(int list[], int left, int mid, int right){
    int i, j, k, l;
    i = left;
    j = mid +1;
    k = left;

    /*merge divided list*/
    while(i<=mid && j<=right){
        if(list[i]<= list[j])
            temp_list[k++] = list[i++];
        else
            temp_list[k++] = list[j++];
    }

    /*Copy the remaining values*/
    if(i>mid){
        for(l=j; l<=right; l++)
            temp_list[k++] = list[l];
    }
    else{
        for(l=i; l<=mid; l++)
            temp_list[k++] = list[l];
    }

    /*copy temp list to main list*/
    for(l=left; l<=right; l++){
        list[l] = temp_list[l];
    }
}

void mergeSort_parallel(int list[], int left, int right,int threads){
    #pragma omp parallel num_threads(threads)
    {
        #pragma omp single
        mergeSort_parallel_internal(list,left,right);
    }
}

void mergeSort_parallel_internal(int list[], int left, int right){
    int mid;
    if(left<right){
        mid = (left+right)/2;
        #pragma omp task
        mergeSort_parallel_internal(list, left, mid);
        #pragma omp task
        mergeSort_parallel_internal(list, mid+1, right);

        merge(list, left, mid, right);
    }
}

int main(){
    float sum = 0;
    int list[MAX_SIZE];
	for (int i = 0; i < 10000; i++)
	{
		struct timeval stime,etime,gap;
		get_randArr(list);
		
		gettimeofday(&stime,NULL);
    	mergeSort_parallel(list,0, MAX_SIZE-1, 1);
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