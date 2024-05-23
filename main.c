#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "compile.h"

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

void showForXinRange(double *values, int xMax, int yMax, int xSpacing, int ySpacing)
{
    int **matrix;
    int y, x = 0;
    int xSize, ySize;

    ySize = (yMax / ySpacing) + 1;
    xSize = (xMax / xSpacing) + 1;

    matrix = initMatrix(xSize, yMax);
    do
    {
        y = *(values + x) / ySpacing;
        if (y >= 0 && y <= yMax / ySpacing)
        {
            system("clear");
            matrix[y][x] = 1;
            drawMatrix(matrix, xSize, ySize, xSpacing, ySpacing);
            sleep(1);
        }
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

void configPlane(int *speed, char *input, int descriptor)
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

    if (descriptor > 0)
        kill(descriptor, SIGUSR1);
    else 
        signal(SIGUSR1, server);
}

double *mapPlane(int descriptor, int xMax, int yMax, int xSpacing, int ySpacing)
{
    char input[100];
    int speed;

    configPlane(&speed, input, descriptor);
    initTokenList(input, 1);
    return getForXinRange(0, xMax, xSpacing);
}

char *getString(double *values, int xCount)
{
    char *buffer;
    int xSize, bufferSize, x = 0;

    xSize = xCount + 1;
    bufferSize = sizeof(char) * 8 * xSize + 1;
    buffer = malloc(bufferSize);
    do
    {
        char number[9];
        snprintf(number, 9, "%07d ", (int)*(values + x));
        strcat(buffer, number);
    } while (++x < xCount);

    return buffer;
}

void server(int descriptor, int readfd, int writefd, int xMax, int yMax, int xSpacing, int ySpacing)
{
    char *buffer;
    double *values;

    values = mapPlane(descriptor, xMax, yMax, xSpacing, ySpacing);
    buffer = getString(values, xMax / xSpacing);

    printf("%s\n", buffer);
    // showForXinRange(values, xMax, yMax, xSpacing, ySpacing);
}

int main()
{
    int xMax, yMax;
    int ySpacing, xSpacing;
    int pid;

    int descriptor;
    int planeSt[2], planeNd[2];

    printDescription();
    configGraph(&xMax, &yMax, &xSpacing, &ySpacing);

    pid = getpid();

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

        server(descriptor, planeNd[0], planeSt[1], xMax, yMax, xSpacing, ySpacing);

        close(planeSt[1]);
        close(planeNd[0]);
        exit(0);
    }
    else
    {
        close(planeSt[1]);
        close(planeNd[0]);

        signal(SIGUSR1, server);
        pause();
        server(descriptor, planeSt[0], planeNd[1], xMax, yMax, xSpacing, ySpacing);

        close(planeSt[0]);
        close(planeNd[1]);
        exit(0);
    }

    return 0;
}