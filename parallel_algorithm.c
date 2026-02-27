#include <stdio.h>
#include <omp.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define N 15 // размерность рюкзака

// номер вектора в бинарный вектор укладки 
void index_to_vector(unsigned int idx, int* p) 
{
    for (int i = 0; i < N; i++)
    {
        p[i] = (idx >> i) & 1;  
    }
}

// IF-ADD 
uint64_t if_add(const int* p, const uint64_t* a) 
{
    uint64_t w = 0;
    for (int i = 0; i < N; i++) 
    {
        if (p[i] == 1) 
        {
            w += a[i];
        }
    }
    return w;
}

// задача рюкзака
uint64_t knapsack(const uint64_t* a)
{
    unsigned int total = 1u << N;           // 2^N = 32768
    uint64_t summ = 0;
    int i = 0;

    // распределение итераций цикла между потоками
    double start = omp_get_wtime(); 

    #pragma omp parallel for num_threads(6) reduction(+:summ) schedule(dynamic, 64)
    for (i = 0; i < total; i++)
    {
        int p[N];
        index_to_vector(i, p);
        summ += if_add(p, a);
        
    }
    double end = omp_get_wtime();
    double time = end - start;
    printf("Время (с): %.6f с\n", time);

    return summ;
}

// генерация весов
void weights(uint64_t* a, int n) 
{
    srand(time(NULL));

    for (int i = 0; i < n; i++) 
    {
        a[i] = ((uint64_t)rand() << 32) | (uint64_t)rand();
    }
}


int main()
{
    // рюкзачный вектор со случайными 64-битными весами 
    uint64_t a[N];
    weights(a, N);
    unsigned int total = 1u << N;

    printf("Первые 5 весов:\n");
    for (int i = 0; i < 5 && i < N; i++) 
    {
        printf("  a[%d] = %llu (0x%016llx)\n", i,
            (unsigned long long)a[i],
            (unsigned long long)a[i]);
    }
    printf("\n");

    // алгоритм
    uint64_t result = knapsack(a);
    printf("Сумма всех весов укладок: %llu\n", (unsigned long long)result);
    printf("Размерность N = %d\n", N);
    printf("Всего векторов = %u (2^%d)\n", total, N);

    return 0;
}