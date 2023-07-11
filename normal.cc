#include <vector>
#include <random>
#include <math.h>
#include <string>
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
    int rows = B.size(), columns = A[0].size();
    std::vector<std::vector<double>> D = GenDoubleMatrix(rows, columns);
    int i, j;
    double accum = 0;
    clock_t begin = clock();
    // Step 1
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                A[i][j] = std::pow(A[i][j], 2);
            }
        }
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                B[i][j] = std::pow(B[i][j], 2);
            }
        }
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < columns; j++)
            {
                D[i][j] = A[i][j] - B[i][j];
            }
        }
        for (i = 0; i < D.size(); i++)
        {
            for (j = 0; j < D[0].size(); j++)
            {
                accum += D[i][j];
            }
        }
    }
    clock_t end = clock();
    std::cout << "Time: " << double(end-begin)/CLOCKS_PER_SEC << "s\n";
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
    // char *argm = argv[1];
    // int m = std::atoi(argm);
    // std::vector<std::vector<double>> A = GenDoubleMatrix(m, m), B = GenDoubleMatrix(m, m);
    // === Validation Code ===

    std::vector<std::vector<double>> A = GenDoubleMatrix(2, 2), B = GenDoubleMatrix(2, 2);
    std::cout << "Matriz A\n";
    DMRead(A);
    std::cout << "Matriz B\n";
    DMRead(B);
    std::cout << "Matriz A\n";
    DMPrint(A);
    std::cout << "Matriz B\n";
    DMPrint(B);

    // === ===
    SumSquaredDifference(A, B);
    return 0;
}