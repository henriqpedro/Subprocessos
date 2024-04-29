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

void drawMatrix(int **matrix, int xCount, int yCount)
{
    int line = 0, column = 0;
    while (line < yCount)
    {
        while (column < xCount)
            printf("%d\t", *(*(matrix + line) + column++));
        printf("\n");
        line++;
        column = 0;
    }
}

void showForXinRange(int xCount, int yCount)
{
    int y, x = 0;
    int **matrix;
    matrix = initMatrix(xCount, yCount);
    do
    {
        system("clear");
        y = compile(x);
        matrix[yCount - y][x] = 1;
        drawMatrix(matrix, xCount, yCount);
        sleep(1);
    } while (++x < xCount);
}

int main()
{
    int xCount, yCount, max, min;
    char input[100];

    printf("X count: 0-");
    scanf("%d", &xCount);

    printf("f(x) = ");
    scanf("%s", input);

    initTokenList(input, 1);

    min = compile(0);
    max = compile(xCount);

    yCount = min > max ? min : max;
    showForXinRange(xCount, yCount);
    return 0;
}