#include <stdio.h>
#include <unistd.h>
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

void showForXinRange(char *input, int xMax, int yMax, int xSpacing, int ySpacing)
{
    double *values;
    int **matrix;
    int y, x = 0;
    int xSize, ySize;

    ySize = (yMax / ySpacing) + 1;
    xSize = (xMax / xSpacing) + 1;

    initTokenList(input, 1);
    values = getForXinRange(0, xMax, xSpacing);
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

void mapPlane(int xMax, int yMax, int xSpacing, int ySpacing)
{
    char input[100];
    int speed;

    configPlane(&speed, input);
    showForXinRange(input, xMax, yMax, xSpacing, ySpacing);
}

int main()
{
    int xMax, yMax;
    int ySpacing, xSpacing;

    printDescription();
    configGraph(&xMax, &yMax, &xSpacing, &ySpacing);
    mapPlane(xMax, yMax, xSpacing, ySpacing);

    return 0;
}