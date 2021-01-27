#include <stdio.h>

__global__ void vec_mul(float *A, float *x, float *c, int N)
{

    int i;
    int global_tidx = blockIdx.x * blockDim.x + threadIdx.x;

    if (global_tidx < N)
    {
        c[global_tidx] = 0.0;
        for (i = 0; i < N; i++)
        {
            c[global_tidx] += A[global_tidx * N + i] * x[i];
        }
    }

    printf("Hello World from GPU thread %d | block %d | thread in block %d !\n", i, blockIdx.x, threadIdx.x);
}

int main(void)
{

    const int N = 200;

    float *A_h, *x_h, *c_h;
    float *A_d, *x_d, *c_d;

    A_h = (float *)malloc(sizeof(float) * N * N);
    x_h = (float *)malloc(sizeof(float) * N);
    c_h = (float *)malloc(sizeof(float) * N);

    cudaMalloc((void **)&A_d, sizeof(float) * N * N);
    cudaMalloc((void **)&x_d, sizeof(float) * N);
    cudaMalloc((void **)&c_d, sizeof(float) * N);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A_h[i * N + j] = (float)1.0;
        }
    }

    for (int i = 0; i < N; i++)
    {
        x_h[i] = (float)1.0;
    }

    cudaMemcpy(A_d, A_h, sizeof(float) * N * N, cudaMemcpyHostToDevice);
    cudaMemcpy(x_d, x_h, sizeof(float) * N, cudaMemcpyHostToDevice);

    int block_size = 32;
    int n_block = N / block_size + (N % block_size == 0 ? 0 : 1);

    vec_mul<<<n_block, block_size>>>(A_d, x_d, c_d, N);
    //cudaDeviceSynchronize(); // Otherwise print may not work.

    for (int i = 0; i < N; i++)
    {
        printf("%d %f\n", i, c_h[i]);
    }

    free(A_h);
    free(x_h);
    free(c_h);
    cudaFree(A_d);
    cudaFree(x_d);
    cudaFree(c_d);

    return 0;
}
