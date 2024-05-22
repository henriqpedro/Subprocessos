#include <stdio.h>
#include <unistd.h>
#include "compile.h"

int **initMatrix(int xCount, int yCount)
{
    int **matrix;
    int line = 0, column = 0;

    matrix = (int **)malloc(sizeof(int *) * yCount);
    while (line < yCount)
        *(matrix + line++) = (int *)malloc(sizeof(int) * xCount);

    line = 0;
    while (line < yCount)
    {
        while (column < xCount)
            *(*(matrix + line) + column++) = 0;
        line++;
        column = 0;
    }

    return matrix;
}

void drawMatrix(int **matrix, int xCount, int yCount, int xSpacing, int ySpacing)
{
    int line = yCount - 1, column = 0;
    do
    {
        printf("[ %04d ] ", line * ySpacing);
        while (column < xCount)
            printf("%d ", *(*(matrix + line) + column++));
        printf("\n");
        column = 0;
    } while (--line >= 0);
}

void showForXinRange(int xCount, int yCount, int xSpacing, int ySpacing)
{
    int **matrix;
    double *values;
    int y, x = 0;

    values = getForXinRange(0, xCount - 1);
    matrix = initMatrix(xCount, yCount);
    do
    {
        y = *(values + x) / ySpacing;
        if (y >= 0 && y <= yCount)
        {
            //system("clear");
            printf("%d ", y);
            matrix[y][x] = 1;
            //drawMatrix(matrix, xCount, yCount, xSpacing, ySpacing);
            //sleep(1);
        }
    } while (++x < xCount);
}

int main()
{
    char input[100];
    int xCount = 26, yCount = 26;
    int ySpacing = 50, xSpacing = 1;

    printf("f(x) = ");
    scanf("%s", input);
    initTokenList(input, 1);

    showForXinRange(xCount, yCount, xSpacing, ySpacing);

    return 0;
}