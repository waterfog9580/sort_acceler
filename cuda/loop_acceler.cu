#include <stdio.h>

/*
 * Refactor `loop` to be a CUDA Kernel. The new kernel should
 * only do the work of 1 iteration of the original loop.
 */

__global__ void loop()
{
  
  printf("This is iteration number %d\n", threadIdx.x);
  
}

int main()
{
  /*
   * When refactoring `loop` to launch as a kernel, be sure
   * to use the execution configuration to control how many
   * "iterations" to perform.
   *
   * For this exercise, only use 1 block of threads.
   */

  int N = 10;
  loop<<<1,N>>>();
  cudaDeviceSynchronize();
}

/*
kernel은 해당 반복문에서 딱 한 번의 반복 작업만 하도록 설계되어야 한다.
kernel이 다른 kernel에 대해서 알지 못하기 때문에, execution configuration이 해당 반복문에서 반복되는 작업의 수에 맞춰 선언되어야 한다.
*/