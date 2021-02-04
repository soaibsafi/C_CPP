#include <stdio.h>

const int N = 32 * 8192;
const int threadsPerBlock = 256;
const int n_blocks = 256;

void read_in(float *);
void write_out_vec(float *);
void write_out_val(float);

__global__ void scal_prod(float *a, float *b, float *c)
{
    // maxT
    int part_res = 0.0;                          // local == register
    __shared__ float part_prod[threadsPerBlock]; // local thread

    int tid = threadIdx.x + blockIdx.x * blockDim.x; // global thread

    for (int i = tid; i < N; i += blockDim.x * gridDim.x)
    {
        part_res += a[i] * b[i];
    }
    part_prod[threadIdx.x] = part_res;

    __syncthreads();

    // Parallel reduction

    // Each threads should add up two of the elements
    // This: half of the threads

    int size = blockDim.x / 2;
    while (size != 0)
    {
        if (threadIdx.x < size)
        {
            part_prod[threadIdx.x] = part_prod[threadIdx.x] + part_prod[threadIdx.x + size];
            // Here we are at blocksize/2 elements
        }
        __syncthreads();
        size = size / 2;
        // Repeat untill
    }

    // One elements per block
    //c[0]
    if (threadIdx.x == 0)
        c[blockIdx.x] = part_prod[0];
}

int main()
{
    //int n_threads = threadsPerBlock / n_blocks;
    float a[N], b[N], part_c[n_blocks];
    float *ad, *bd, *part_cd;
    float c = 0.0;

    cudaMalloc((void **)&ad, sizeof(float) * N);
    cudaMalloc((void **)&bd, sizeof(float) * N);
    cudaMalloc((void **)&part_cd, sizeof(float) * n_blocks);

    read_in(a);
    read_in(b);

    cudaMemcpy(ad, a, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(bd, b, N * sizeof(float), cudaMemcpyHostToDevice);

    scal_prod<<<n_blocks, threadsPerBlock>>>(ad, bd, part_cd);

    cudaMemcpy(part_c, part_cd, n_blocks * sizeof(float), cudaMemcpyDeviceToHost);

    for (int i = 0; i < n_blocks; i++) //256 iteration
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
//  GPU activities:   93.59%  342.55us         2  171.28us  171.26us  171.29us  [CUDA memcpy HtoD]
//                     5.98%  21.887us         1  21.887us  21.887us  21.887us  scal_prod(float*, float*, float*)
//                     0.43%  1.5680us         1  1.5680us  1.5680us  1.5680us  [CUDA memcpy DtoH]
//       API calls:   99.41%  279.09ms         3  93.030ms  7.4150us  278.95ms  cudaMalloc
//                     0.27%  753.46us         3  251.15us  162.29us  368.74us  cudaMemcpy
//                     0.11%  310.28us       101  3.0720us     374ns  125.09us  cuDeviceGetAttribute
//                     0.10%  286.03us         1  286.03us  286.03us  286.03us  cuDeviceTotalMem
//                     0.07%  206.96us         3  68.985us  12.608us  105.36us  cudaFree
//                     0.02%  53.174us         1  53.174us  53.174us  53.174us  cuDeviceGetName
//                     0.02%  43.136us         1  43.136us  43.136us  43.136us  cudaLaunchKernel
//                     0.00%  8.9260us         1  8.9260us  8.9260us  8.9260us  cuDeviceGetPCIBusId
//                     0.00%  3.8780us         3  1.2920us     518ns  2.7920us  cuDeviceGetCount
//                     0.00%  1.6580us         2     829ns     425ns  1.2330us  cuDeviceGet
//                     0.00%     615ns         1     615ns     615ns     615ns  cuDeviceGetUuid
