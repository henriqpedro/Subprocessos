#include <unistd.h>
#include <pthread.h>
#include "compile.h"

int maxCount = 200;
struct args
{
    int speed;
    int mainSpeed;
    int xMax;
    int yMax;
    int xSpacing;
    int ySpacing;
    double *values;
    double *mainValues;
};

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

void clearMatrix(int **matrix, int clearValue, int xSize, int ySize, int xSpacing, int ySpacing)
{
    int line = ySize - 1, column = 0, value;
    do
    {
        while (column < xSize)
        {
            value = *(*(matrix + line) + column);
            *(*(matrix + line) + column++) = value == clearValue ? 0 : value;
        }
        column = 0;
    } while (--line >= 0);
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

void *showForXinRange(void *args)
{
    int **matrix;
    int y1, y2, x = 0;
    int xSize, ySize;
    int speedDifference, mainSpeedDifference;

    struct args *arguments;
    arguments = (struct args *)args;

    ySize = (arguments->yMax / arguments->ySpacing) + 1;
    xSize = (arguments->xMax / arguments->xSpacing) + 1;

    speedDifference = arguments->speed / arguments->mainSpeed;
    mainSpeedDifference = arguments->mainSpeed / arguments->speed;

    matrix = initMatrix(xSize, arguments->yMax);
    do
    {
        clearMatrix(matrix, 1, xSize, ySize, arguments->xSpacing, arguments->ySpacing);
        y1 = *(arguments->mainValues + x) / arguments->ySpacing;
        if (y1 >= 0 && y1 <= arguments->yMax / arguments->ySpacing)
            matrix[y1][x] = 1;

        sleep(speedDifference);
        system("clear");
        drawMatrix(matrix, xSize, ySize, arguments->xSpacing, arguments->ySpacing);

        clearMatrix(matrix, 2, xSize, ySize, arguments->xSpacing, arguments->ySpacing);
        y2 = *(arguments->values + x) / arguments->ySpacing;
        if (y2 >= 0 && y2 <= arguments->yMax / arguments->ySpacing)
            matrix[y2][x] = matrix[y2][x] == 0 ? 2 : 3;

        sleep(mainSpeedDifference);
        system("clear");
        drawMatrix(matrix, xSize, ySize, arguments->xSpacing, arguments->ySpacing);
    } while (++x <= arguments->xMax / arguments->xSpacing);
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

void initPlaneBuffer(char *buffer, int *speed, char *input)
{
    char speedBuffer[10];

    configPlane(speed, input);
    snprintf(speedBuffer, 10, "%d ", *speed);
    strcpy(buffer, speedBuffer);
}

void getValuesBuffer(char *buffer, double *values, int lastIndex)
{
    int x = 0;
    strcpy(buffer, "\0");
    do
    {
        char number[10];
        snprintf(number, 10, "%08d ", (int)*(values + x));
        strcat(buffer, number);
    } while (++x < lastIndex);
}

double *getValuesFromBuffer(char *buffer, int length)
{
    int x = 0;
    double *values;
    char *token;

    values = malloc(sizeof(double) * length);
    token = strtok(buffer, " ");
    do
    {
        *(values + x++) = atoi(token);
    } while ((token = strtok(NULL, " ")));

    return values;
}

void airplane(int readfd, int writefd, int xMax, int yMax, int xSpacing, int ySpacing)
{
    int speed;
    double *values;
    char input[100], buffer[maxCount];

    read(readfd, buffer, maxCount);
    initPlaneBuffer(buffer, &speed, input);
    write(writefd, buffer, maxCount);

    initTokenList(input, 0);
    values = getForXinRange(0, xMax, xSpacing);

    getValuesBuffer(buffer, values, xMax / xSpacing);
    write(writefd, buffer, maxCount);
}

void callThread(double *mainValues, double *values, int mainSpeed, int speed, int xMax, int yMax, int xSpacing, int ySpacing)
{
    pthread_t tid;
    struct args *arguments = (struct args *)malloc(sizeof(struct args));

    arguments->mainValues = mainValues;
    arguments->values = values;
    arguments->mainSpeed = mainSpeed;
    arguments->speed = speed;

    arguments->xMax = xMax;
    arguments->yMax = yMax;
    arguments->xSpacing = xSpacing;
    arguments->ySpacing = ySpacing;

    pthread_create(&tid, NULL, showForXinRange, (void *)arguments);
    pthread_join(tid, NULL);
}

void mainAirplane(int readfd, int writefd, int xMax, int yMax, int xSpacing, int ySpacing)
{
    int speed, mainSpeed;
    double *values, *mainValues;
    char input[100], buffer[maxCount];

    initPlaneBuffer(buffer, &mainSpeed, input);
    write(writefd, buffer, maxCount);

    initTokenList(input, 0);
    mainValues = getForXinRange(0, xMax, xSpacing);

    read(readfd, buffer, maxCount);
    speed = atoi(buffer);
    if (speed <= 0)
    {
        printf("Error: Invalid speed;\n");
        exit(0);
    }

    read(readfd, buffer, maxCount);
    values = getValuesFromBuffer(buffer, xMax / xSpacing + 1);

    callThread(mainValues, values, mainSpeed, speed, xMax, yMax, xSpacing, ySpacing);
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

        mainAirplane(planeNd[0], planeSt[1], xMax, yMax, xSpacing, ySpacing);

        close(planeSt[1]);
        close(planeNd[0]);
    }
    else
    {
        close(planeSt[1]);
        close(planeNd[0]);

        airplane(planeSt[0], planeNd[1], xMax, yMax, xSpacing, ySpacing);

        close(planeSt[0]);
        close(planeNd[1]);
    }

    return 0;
}