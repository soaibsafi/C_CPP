/*******************************
*** *** TASK-3
*** NAME: - SOAIBUZZAMAN
*** Matrikel Number: 613488
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

const int N = 200;
const int block_size = 32;
const int num_blocks = N / block_size + (N % block_size == 0 ? 0 : 1);

// Device Function
__global__ void calc_max_device(int *vec, int N, int *grid_results)
{
    // Each thread determines the local maximum maxT of its assigned vector elements.
    int max_t = 0;
    __shared__ int part_prod[block_size];

    int tid = threadIdx.x + blockIdx.x * blockDim.x;

    for (int i = tid; i < N; i += blockDim.x * gridDim.x)
    {
        // Max Logic
        if (vec[i] < vec[tid * block_size + i])
        {
            max_t = vec[tid * block_size + i];
        }
    }
    part_prod[threadIdx.x] = max_t;
    __syncthreads();

    // The threads of a blockblockIdx.x determine one local maximum maxB
    // for this block inparallel. This value is stored at the position
    // grid_results[blockIdx.x].
    int size = blockDim.x / 2;
    while (size > 0)
    {
        if (threadIdx.x < size)
        {
            // Finding max for blocksize/2 elements
            if (part_prod[threadIdx.x] < part_prod[threadIdx.x + 1])
                part_prod[threadIdx.x] = part_prod[threadIdx.x + 1];
        }

        __syncthreads();
        size = size / 2;
    }
    // One elements per block
    if (threadIdx.x == 0)
    {
        grid_results[blockIdx.x] = part_prod[0];
    }
}

// Host function
int calc_max(int *vec, int N, int block_size, int num_blocks)
{
    int *max_v;
    int *vec_d, *max_val_d;

    max_v = (int *)malloc(sizeof(int));

    cudaMalloc((void **)&vec_d, sizeof(int) * N);
    cudaMalloc((void **)&max_val_d, sizeof(int));

    // initialize the vector for testing.
    for (int i = 0; i < N; i++)
    {
        vec[i] = rand() % 100;
    }

    cudaMemcpy(vec_d, vec, sizeof(int) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(max_val_d, max_v, sizeof(int) * N, cudaMemcpyHostToDevice);

    // calling the device funtion
    calc_max_device<<<num_blocks, block_size>>>(vec_d, N, max_val_d);

    cudaMemcpy(max_v, max_val_d, sizeof(int), cudaMemcpyDeviceToHost);

    free(vec);
    // Couldn't free max_v because it need to be returned.
    cudaFree(vec_d);
    cudaFree(max_val_d);

    return *max_v;
}

int main(void)
{
    // Main function for testing
    int *vec_h, *max_val_h;

    max_val_h = (int *)malloc(sizeof(int));
    vec_h = (int *)malloc(sizeof(int) * N);

    *max_val_h = calc_max(vec_h, N, block_size, num_blocks);

    printf("%d\n", *max_val_h);

    free(max_val_h);

    return 0;
}
