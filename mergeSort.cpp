#include<iostream>
#include<stdlib.h>

#define MAX_SIZE 10000

using namespace std;

int temp_list[MAX_SIZE];

void get_randArr(int (&arr)[MAX_SIZE]);
void merge(int list[], int left, int mid, int right);
void merge_sort(int list[], int left, int right);

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

void merge_sort(int list[], int left, int right){
    int mid;
    if(left<right){
        mid = (left+right)/2;
        merge_sort(list, left, mid);
        merge_sort(list, mid+1, right);
        merge(list, left, mid, right);
    }
}

int main(){
    int i;
    int list[MAX_SIZE];

    get_randArr(list);
    merge_sort(list, 0, MAX_SIZE-1);
    
    for(i=0; i<MAX_SIZE; i++){
        cout << list[i] << " ";
    }
    cout << endl;
    return 0;
}