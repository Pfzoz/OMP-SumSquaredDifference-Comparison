#include <vector>
#include <random>
#include <iostream>
#include "omp.h"

// Random Double Gambiarra
double lower_bound = 0, higher_bound = 10000;
std::uniform_real_distribution<double> unif(lower_bound, higher_bound);
std::default_random_engine re;

std::vector<std::vector<double>> GenDoubleMatrix(int rows, int columns)
{
    std::vector<std::vector<double>> matrix;
    for (int i = 0; i < rows; i++)
    {
        std::vector<double> row;
        for (int j = 0; j < columns; j++)
        {
            row.push_back(unif(re));
        }
        matrix.push_back(row);
    }
    return matrix;
}

double SumSquaredDifference(std::vector<std::vector<double>> A, std::vector<std::vector<double>> B)
{
    int rows = A.size(), columns = A[0].size();
    std::vector<std::vector<double>> D = GenDoubleMatrix(rows, columns);
    double accum = 0;
    int i, j;
    auto begin = omp_get_wtime();
    // Step 1
    {
#pragma omp parallel for shared(A, i) private(j)
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                A[i][j] *= A[i][j];
            }
        }
#pragma omp parallel for shared(B, i) private(j)
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B[i][j] *= B[i][j];
            }
        }

#pragma omp parallel for shared(D, i) private(j)
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                D[i][j] = A[i][j] - B[i][j];
            }
        }
#pragma omp parallel for shared(D, i) private(j) reduction(+:accum)
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                accum = accum + D[i][j];
            }
        }
    }
    auto end = omp_get_wtime();
    std::cout << "Time: " << end - begin << "s\n";
    std::cout << "SSD: " << accum << '\n';
    return accum;
}

void DMRead(std::vector<std::vector<double>> &matrix)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            std::cout << "Elemento " << i << ' ' << j << ":";
            std::cin >> matrix[i][j];
            std::cout << '\n';
        }
    }
}

void DMPrint(std::vector<std::vector<double>> matrix)
{
    for (std::vector<double> row : matrix)
    {
        for (double element : row)
        {
            std::cout << element << ' ';
        }
        std::cout << '\n';
    }
}

int main(int argc, char **argv)
{
    char *argm = argv[1];
    int num_threads = atoi(argv[2]);
    omp_set_num_threads(num_threads);
    int m = std::atoi(argm);
    std::vector<std::vector<double>> A = GenDoubleMatrix(m, m), B = GenDoubleMatrix(m, m);

    // === Validation Code ===

    // std::vector<std::vector<double>> A = GenDoubleMatrix(2, 2), B = GenDoubleMatrix(2, 2);
    // std::cout << "Matriz A\n";
    // DMRead(A);
    // std::cout << "Matriz B\n";
    // DMRead(B);
    // std::cout << "Matriz A\n";
    // DMPrint(A);
    // std::cout << "Matriz B\n";
    // DMPrint(B);

    // === ===
    SumSquaredDifference(A, B);
    return 0;
}