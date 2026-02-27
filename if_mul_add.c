#include <stdio.h>
#include <omp.h>
#include <stdint.h>

#define TASK_SIZE 15
#define VECTORS_PER_POINT 1000000

// IF-ADD с 64-битными числами
uint64_t weight_if_add_64(const int* p, const uint64_t* a) {
    uint64_t weight = 0;
    for (int i = 0; i < TASK_SIZE; i++) {
        if (p[i]) {
            weight += a[i];
        }
    }
    return weight;
}

// MUL-ADD с 64-битными числами  
uint64_t weight_mul_add_64(const int* p, const uint64_t* a) {
    uint64_t weight = 0;
    for (int i = 0; i < TASK_SIZE; i++) {
        weight += (uint64_t)p[i] * a[i];
    }
    return weight;
}

// Генерация следующего сочетания (лексикографический порядок)
int next_combination(int* comb, int k, int n) {
    for (int i = k - 1; i >= 0; i--) {
        if (comb[i] < n - k + i) {
            comb[i]++;
            for (int j = i + 1; j < k; j++) {
                comb[j] = comb[j - 1] + 1;
            }
            return 1;
        }
    }
    return 0;
}

// Генерация всех сочетаний с повторами (циклически)
int generate_vectors_cyclic(int k, int vectors[][TASK_SIZE], int max_count) {
    if (k < 0 || k > TASK_SIZE) return 0;

    // Особые случаи
    if (k == 0) {
        for (int i = 0; i < max_count; i++) {
            for (int j = 0; j < TASK_SIZE; j++)
                vectors[i][j] = 0;
        }
        return max_count;
    }

    if (k == TASK_SIZE) {
        for (int i = 0; i < max_count; i++) {
            for (int j = 0; j < TASK_SIZE; j++)
                vectors[i][j] = 1;
        }
        return max_count;
    }

    // Вычисляем общее количество сочетаний C(TASK_SIZE, k)
    int total_combinations = 1;
    for (int i = 1; i <= k; i++) {
        total_combinations = total_combinations * (TASK_SIZE - k + i) / i;
    }

    // Инициализация первой комбинации
    int comb[TASK_SIZE];
    for (int i = 0; i < k; i++)
        comb[i] = i;

    int count = 0;
    int comb_index = 0;

    // Генерируем max_count векторов с повторами
    while (count < max_count) {
        // Создаем вектор из текущей комбинации
        for (int i = 0; i < TASK_SIZE; i++)
            vectors[count][i] = 0;

        for (int i = 0; i < k; i++)
            vectors[count][comb[i]] = 1;

        count++;
        comb_index++;

        // Если прошли все комбинации, начинаем сначала
        if (comb_index >= total_combinations) {
            // Сброс на первую комбинацию
            for (int i = 0; i < k; i++)
                comb[i] = i;
            comb_index = 0;
        }
        else {
            // Иначе генерируем следующую комбинацию
            next_combination(comb, k, TASK_SIZE);
        }
    }

    return count;
}

int main() {
    omp_set_num_threads(1);

    // 64-битные веса
    uint64_t a[TASK_SIZE];
    for (int i = 0; i < TASK_SIZE; i++)
        a[i] = (uint64_t)(i + 1) * 1000000000ULL;

    static int vectors[VECTORS_PER_POINT][TASK_SIZE];

    printf("k,if_ns,mul_ns,count,total_combinations\n");

    // k от 0 до TASK_SIZE
    for (int k = 0; k <= TASK_SIZE; k++) {
        // Генерируем ровно VECTORS_PER_POINT векторов с повторами
        int count = generate_vectors_cyclic(k, vectors, VECTORS_PER_POINT);

        uint64_t dummy = 0;

        // IF-ADD
        double start_if = omp_get_wtime();
        for (int i = 0; i < count; i++) {
            dummy += weight_if_add_64(vectors[i], a);
        }
        double end_if = omp_get_wtime();

        // MUL-ADD
        double start_mul = omp_get_wtime();
        for (int i = 0; i < count; i++) {
            dummy += weight_mul_add_64(vectors[i], a);
        }
        double end_mul = omp_get_wtime();

        double if_ns = (end_if - start_if) * 1e9 / count;
        double mul_ns = (end_mul - start_mul) * 1e9 / count;

        // Вычисляем общее количество уникальных комбинаций
        int total_comb = 1;
        for (int i = 1; i <= k && i <= TASK_SIZE - k; i++) {
            total_comb = total_comb * (TASK_SIZE - k + i) / i;
        }

        printf("%d,%.2f,%.2f,%d,%d\n", k, if_ns, mul_ns, count, total_comb);
    }

    return 0;
}