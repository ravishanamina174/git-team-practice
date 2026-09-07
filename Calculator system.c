#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_RECORDS 100
#define MAX_NAME_LEN 50

/* ---------------------------------------------------------
 * Struct Definitions
 * --------------------------------------------------------- */

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    double num1;
    double num2;
    double result;
    char operation[10];
} CalculationRecord;

typedef struct {
    CalculationRecord records[MAX_RECORDS];
    int count;
} CalculationDatabase;

/* Function signature for operations map lookup */
typedef double (*MathFunc)(double, double);

typedef struct {
    const char *op_name;
    MathFunc func;
} OpMapping;

/* ---------------------------------------------------------
 * Core Math Implementations
 * --------------------------------------------------------- */

double addNumbers(double a, double b) { return a + b; }
double subtractNumbers(double a, double b) { return a - b; }
double multiplyNumbers(double a, double b) { return a * b; }

double divideNumbers(double a, double b) {
    if (b == 0.0) {
        printf("Warning: Division by zero attempted.\n");
        return 0;
    }
    return a / b;
}

double powerNumbers(double base, double exponent) {
    return pow(base, exponent);
}

double squareRoot(double value) {
    if (value < 0.0) {
        printf("Warning: Cannot compute square root of negative number.\n");
        return 0;
    }
    return sqrt(value);
}

static const OpMapping operation_map[] = {
    {"add", addNumbers},
    {"sub", subtractNumbers},
    {"mul", multiplyNumbers},
    {"div", divideNumbers},
    {"pow", powerNumbers},
    {"sqrt", NULL}
};
static const int num_ops = sizeof(operation_map) / sizeof(operation_map[0]);
