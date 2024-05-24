#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "compile.h"

int maxCount = 200;

int **initMatrix(int xMax, int yMax)
{
    int **matrix;
    int line = 0, column = 0;

    matrix = (int **)malloc(sizeof(int *) * yMax);
    while (line < yMax)
        *(matrix + line++) = (int *)malloc(sizeof(int) * xMax);

    line = 0;
    while (line < yMax)
    {
        while (column < xMax)
            *(*(matrix + line) + column++) = 0;
        line++;
        column = 0;
    }

    return matrix;
}

void drawMatrix(int **matrix, int xSize, int ySize, int xSpacing, int ySpacing)
{
    int line = ySize - 1, column = 0;
    do
    {
        printf("[%04d] ", line * ySpacing);
        while (column < xSize)
            printf("%3d  ", *(*(matrix + line) + column++));
        printf("\n");
        column = 0;
    } while (--line >= 0);

    printf("       ");

    do
    {
        printf("[%03d]", column * xSpacing);
    } while (++column < xSize);

    printf("\n");
}

void showForXinRange(double *valuesSt, double *valuesNd, int xMax, int yMax, int xSpacing, int ySpacing)
{
    int **matrix;
    int y1, y2, x = 0;
    int xSize, ySize;

    ySize = (yMax / ySpacing) + 1;
    xSize = (xMax / xSpacing) + 1;

    matrix = initMatrix(xSize, yMax);
    do
    {
        system("clear");

        y1 = *(valuesSt + x) / ySpacing;
        y2 = *(valuesNd + x) / ySpacing;

        if (y1 >= 0 && y1 <= yMax / ySpacing)
            matrix[y1][x] = 1;
        if (y2 >= 0 && y2 <= yMax / ySpacing)
            matrix[y2][x] = 1;

        drawMatrix(matrix, xSize, ySize, xSpacing, ySpacing);
        sleep(1);
    } while (++x <= xMax / xSpacing);
}

void printDescription()
{
    printf("\n\nSISTEMA CARTESIANO (TRAJETÓRIA DE AERONAVES)\n\n");
    printf("***************************************\n");
    printf("O programa espera como entrada uma função matemática representando a trajetória traçada por dois veículos aéreos ");
    printf("e, sabendo a velocidade de ambos, esboça num plano cartesiano (concorrentemente) o trajeto correspondente a cada um deles, ");
    printf("indicando a ocorrência de colisão.\n");
    printf("***************************************\n");

    printf("\nOPERAÇÕES\n");
    printf("***************************************\n");
    printf("+ | Soma\n");
    printf("- | Subtração\n");
    printf("* | Multiplicação\n");
    printf("/ | Divisão\n");
    printf("^ | Potenciação\n");
    printf("***************************************\n");

    printf("\nFUNÇÕES MATEMÁTICAS\n");
    printf("***************************************\n");
    printf("sin() | Seno\n");
    printf("cos() | Cosseno\n");
    printf("tan() | Tangente\n");
    printf("log() | Logaritmo\n");
    printf("log10() | Logaritmo na base 10\n");
    printf("***************************************\n\n");
}

void configGraph(int *xMax, int *yMax, int *xSpacing, int *ySpacing)
{
    printf("\nESBOÇO DO PLANO\n");
    printf("***************************************\n");
    printf("LIMITE DOS EIXOS\n");
    printf("**SUGESTÃO: X (0-180); Y (0-1400)**\n");
    printf("Eixo X (metros): 0-");
    scanf("%d", xMax);
    printf("Eixo Y (metros): 0-");
    scanf("%d", yMax);

    printf("\nINTERVALO ENTRE VALORES\n");
    printf("**SUGESTÃO: X (10); Y (50)**\n");
    printf("Eixo X (metros): ");
    scanf("%d", xSpacing);
    printf("Eixo Y (metros): ");
    scanf("%d", ySpacing);
    printf("***************************************\n\n");
}

void exec(int signal) {}

void configPlane(int *speed, char *input)
{
    printf("\nAERONAVES\n");
    printf("***************************************\n");
    printf("Velocidade (km/s): ");
    scanf("%d", speed);

    printf("\nFUNÇÃO DO TRAJETO\n");
    printf("**SUGESTÕES: f(x) = -log(x)*100+1000; f(x) = 150*sin(x/2-1)+900; f(x) = 5*x; f(x) = (1+1/10)^(x - 30)**\n");
    printf("f(x) = ");
    scanf("%s", input);
    printf("***************************************\n\n");
}

double *mapPlane(int descriptor, int readfd, int writefd, int xMax, int yMax, int xSpacing, int ySpacing)
{
    char speedBuffer[10];
    char buffer[maxCount];
    char input[100];
    int speed;

    if (descriptor > 0)
    {
        read(readfd, buffer, maxCount);

        configPlane(&speed, input);
        snprintf(speedBuffer, 10, "%d ", speed);
        strcat(buffer, speedBuffer);
        strcat(buffer, input);

        write(writefd, buffer, maxCount);
    }
    else
    {
        configPlane(&speed, input);
        snprintf(speedBuffer, 10, "%d ", speed);
        strcat(buffer, speedBuffer);
        strcat(buffer, input);

        write(writefd, buffer, maxCount);
        read(readfd, buffer, maxCount);

        printf("%s\n", buffer);
    }

    initTokenList(input, 1);
    return getForXinRange(0, xMax, xSpacing);
}

void airplane(int descriptor, int readfd, int writefd, int xMax, int yMax, int xSpacing, int ySpacing)
{
    int x;
    char *token;
    char bufferIn[maxCount], bufferOut[maxCount];
    double *valuesSt, *valuesNd;

    if (descriptor > 0)
    {
        valuesSt = mapPlane(descriptor, readfd, writefd, xMax, yMax, xSpacing, ySpacing);
        do
        {
            char number[10];
            snprintf(number, 10, "%08d ", (int)*(valuesSt + x));
            strcat(bufferIn, number);
        } while (++x < xMax / xSpacing);

        write(writefd, bufferIn, maxCount);
        read(readfd, bufferOut, maxCount);

        x = 0;
        valuesNd = malloc(sizeof(double) * (xMax / xSpacing + 1));
        token = strtok(bufferOut, " ");
        do
        {
            *(valuesNd + x++) = atoi(token);
        } while ((token = strtok(NULL, " ")));
        showForXinRange(valuesSt, valuesNd, xMax, yMax, xSpacing, ySpacing);
    }
    else
    {
        valuesSt = mapPlane(descriptor, readfd, writefd, xMax, yMax, xSpacing, ySpacing);
        do
        {
            char number[10];
            snprintf(number, 10, "%08d ", (int)*(valuesSt + x));
            strcat(bufferIn, number);
        } while (++x < xMax / xSpacing);
        write(writefd, bufferIn, maxCount);
    }
}

int main()
{
    int xMax, yMax;
    int ySpacing, xSpacing;

    int descriptor;
    int planeSt[2], planeNd[2];

    printDescription();
    configGraph(&xMax, &yMax, &xSpacing, &ySpacing);

    if (pipe(planeSt) < 0 || pipe(planeNd) < 0)
    {
        printf("Error: cannot call pipe;\n");
        exit(0);
    }

    if ((descriptor = fork()) < 0)
    {
        printf("Error: cannot call fork;\n");
        exit(0);
    }
    else if (descriptor > 0)
    {
        close(planeSt[0]);
        close(planeNd[1]);

        airplane(descriptor, planeNd[0], planeSt[1], xMax, yMax, xSpacing, ySpacing);

        close(planeSt[1]);
        close(planeNd[0]);
    }
    else
    {
        close(planeSt[1]);
        close(planeNd[0]);

        airplane(descriptor, planeSt[0], planeNd[1], xMax, yMax, xSpacing, ySpacing);

        close(planeSt[0]);
        close(planeNd[1]);
    }

    return 0;
}