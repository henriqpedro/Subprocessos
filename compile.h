#include "token.h"

struct tokenList tokenList;
token *currentToken;

void printExpectedTokenException(char *);

int isTokenType(int);
void nextToken();

double E(double);
double lineE(double, double);
double T(double, double);
double lineT(double, double);
double K(double, double);
double lineK(double, double);
double F(double);
double MF(double);

void initTokenList(char *input, int show);
double compile(double);

void printExpectedTokenException(char *expected)
{
    if (currentToken == NULL)
        printf("Error: expected %s, but was NULL\n", expected);
    else
        printf("Error: expected %s, but was '%s'\n", expected, currentToken->input);
    exit(0);
}

int isTokenType(int type)
{
    return currentToken != NULL && currentToken->type == type;
}

void nextToken()
{
    currentToken = currentToken->next;
}

double E(double incognita)
{
    double number = 0;
    number = T(number, incognita);
    number = lineE(number, incognita);
    return number;
}

double lineE(double number, double incognita)
{
    if (isTokenType('+'))
    {
        nextToken();
        number += T(number, incognita);
        number = lineE(number, incognita);
    }
    else if (isTokenType('-'))
    {
        nextToken();
        number -= T(number, incognita);
        number = lineE(number, incognita);
    }
    return number;
}

double T(double number, double incognita)
{
    number = K(number, incognita);
    number = lineT(number, incognita);
    return number;
}

double lineT(double number, double incognita)
{
    if (isTokenType('*'))
    {
        nextToken();
        number *= K(number, incognita);
        number = lineT(number, incognita);
    }
    else if (isTokenType('/'))
    {
        nextToken();
        number /= K(number, incognita);
        number = lineT(number, incognita);
    }
    return number;
}

double K(double number, double incognita)
{
    number = F(incognita);
    number = lineK(number, incognita);
    return number;
}

double lineK(double number, double incognita)
{
    if (isTokenType('^'))
    {
        nextToken();
        number = pow(number, F(incognita));
        number = lineK(number, incognita);
    }
    return number;
}

double F(double incognita)
{
    double number;
    int negative = 0;

    if (isTokenType('-'))
    {
        negative = 1;
        nextToken();
    }

    if (isTokenType(INCOGNITA))
    {
        number = incognita;
        nextToken();
    }
    else if (isTokenType(MATHFUNCTION))
        number = MF(incognita);
    else if (isTokenType(NUMBER))
    {
        number = currentToken->value;
        nextToken();
    }
    else if (isTokenType('('))
    {
        nextToken();
        number = E(incognita);
        if (!isTokenType(')'))
            printExpectedTokenException("')'");
        nextToken();
    }
    else
        printExpectedTokenException("INCOGNITA, MATHFUNCTION, NUMBER, EXPRESSION");
    return negative ? number * -1 : number;
}

double MF(double incognita)
{
    double number;
    token *auxToken;
    auxToken = currentToken;

    if (currentToken->next == NULL ||
        currentToken->next->type != '(')
        printExpectedTokenException("'('");

    currentToken = currentToken->next->next;
    number = E(incognita);

    if (currentToken == NULL ||
        currentToken->type != ')')
        printExpectedTokenException("')'");

    currentToken = currentToken->next;

    if (strcmp(auxToken->input, "sin") == 0)
        return number = sin(number);
    if (strcmp(auxToken->input, "cos") == 0)
        return cos(number);
    if (strcmp(auxToken->input, "tan") == 0)
        return tan(number);
    if (strcmp(auxToken->input, "log") == 0)
        return log(number);
    return log10(number);
}

void initTokenList(char *input, int show)
{
    getTokens(&tokenList, input);
    if (show)
        showTokens(tokenList);
}

double compile(double incognita)
{
    double number;
    currentToken = tokenList.head;

    number = E(incognita);
    if (currentToken != NULL)
    {
        printf("Error: value not expected '%s'\n", currentToken->input);
        exit(1);
    }

    return number;
}

double *getForXinRange(int min, int max, int space)
{
    int i = 0, size, initial;
    double *values;

    initial = min;
    size = ((max - min) / space) + 1;
    values = malloc(sizeof(double) * size);
    do
    {
        *(values + i) = compile(initial);
        initial += space;
    } while (++i < size);

    return values;
}