#include <stdio.h>

const int N = 32 * 8192;
const int threadsPerBlock = 256;
const int n_blocks = N / threadsPerBlock;

void read_in(float *);
void write_out_vec(float *);
void write_out_val(float);

__global__ void scal_prod(float *a, float *b, float *c)
{
    int tid = threadIdx.x + blockIdx.x * blockDim.x;

    c[tid] = a[tid] * b[tid];
}

int main()
{
    float a[N], b[N], part_c[N];
    float *ad, *bd, *part_cd;
    float c = 0.0;

    cudaMalloc((void **)&ad, sizeof(float) * N);
    cudaMalloc((void **)&bd, sizeof(float) * N);
    cudaMalloc((void **)&part_cd, sizeof(float) * N);

    read_in(a);
    read_in(b);

    cudaMemcpy(ad, a, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(bd, b, N * sizeof(float), cudaMemcpyHostToDevice);

    scal_prod<<<n_blocks, threadsPerBlock>>>(ad, bd, part_cd);

    cudaMemcpy(part_c, part_cd, N * sizeof(float), cudaMemcpyDeviceToHost);

    for (int i = 0; i < N; i++)
    {
        c += part_c[i];
    }

    write_out_val(c);

    cudaFree(ad);
    cudaFree(bd);
    cudaFree(part_cd);
}

void read_in(float *arr)
{
    for (int i = 0; i < N; i++)
    {
        arr[i] = 1;
    }
}

void write_out_val(float val)
{
    printf("Result is %.2f\n\n", val);
}

void write_out_vec(float *vec)
{
    printf("Resut\n");
    for (int i = 0; i < N; i++)
    {
        printf("%.2f", vec[i]);
    }
}

// STDERR
// ==6== NVPROF is profiling process 6, command: ./a.out
// ==6== Profiling application: ./a.out
// ==6== Profiling result:
//             Type  Time(%)      Time     Calls       Avg       Min       Max  Name
//  GPU activities:   64.96%  342.26us         2  171.13us  171.13us  171.13us  [CUDA memcpy HtoD]
//                    29.90%  157.53us         1  157.53us  157.53us  157.53us  [CUDA memcpy DtoH]
//                     5.14%  27.104us         1  27.104us  27.104us  27.104us  scal_prod(float*, float*, float*)
//       API calls:   99.11%  272.24ms         3  90.746ms  7.6890us  272.10ms  cudaMalloc
//                     0.57%  1.5711ms         3  523.71us  226.07us  977.65us  cudaMemcpy
//                     0.11%  299.16us       101  2.9610us     377ns  117.61us  cuDeviceGetAttribute
//                     0.10%  279.76us         1  279.76us  279.76us  279.76us  cuDeviceTotalMem
//                     0.08%  210.52us         3  70.173us  14.868us  108.48us  cudaFree
//                     0.02%  42.620us         1  42.620us  42.620us  42.620us  cudaLaunchKernel
//                     0.02%  41.219us         1  41.219us  41.219us  41.219us  cuDeviceGetName
//                     0.00%  3.5540us         1  3.5540us  3.5540us  3.5540us  cuDeviceGetPCIBusId
//                     0.00%  2.7450us         3     915ns     488ns  1.7210us  cuDeviceGetCount
//                     0.00%  1.6800us         2     840ns     423ns  1.2570us  cuDeviceGet
//                     0.00%     635ns         1     635ns     635ns     635ns  cuDeviceGetUuid
