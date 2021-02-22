#include <stdio.h>

void init(int *a, int N)
{
  int i;
  for (i = 0; i < N; ++i)
  {
    a[i] = i;
  }
}

__global__ //Define this function is computed by gpu(eg, __global__ void function())
void doubleElements(int *a, int N)
{
  int i;
  i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < N)
  {
    a[i] *= 2;
  }
}

bool checkElementsAreDoubled(int *a, int N)
{
  int i;
  for (i = 0; i < N; ++i)
  {
    if (a[i] != i*2) return false;
  }
  return true;
}

int main()
{
  int N = 100;
  int *a;

  size_t size = N * sizeof(int);

  cudaMallocManaged(&a, size);  //Array allocate for GPU
  init(a, N);                   //Array allocate for CPU

  size_t threads_per_block = 10;
  size_t number_of_blocks = 10;


  doubleElements<<<number_of_blocks, threads_per_block>>>(a, N); //GPU compute
  cudaDeviceSynchronize(); //synchronize GPU with CPU 

  bool areDoubled = checkElementsAreDoubled(a, N);
  printf("All elements were doubled? %s\n", areDoubled ? "TRUE" : "FALSE");

  cudaFree(a);
}

/*
kernel: GPU function을 부르는 용어. kernel은 execution configuration에 따라 실행된다.
thread: GPU 작업의 기본 단위. 여러 thread가 병렬적으로 작동한다.
block:  thread의 모임을 block이라 한다.
grid:   주어진 kernel의 execution configuration에서 block들의 모임, 그러니까 전체를 grid라 부른다.
*/