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

/* ---------------------------------------------------------
 * Database Operations & Routing
 * --------------------------------------------------------- */

void initDatabase(CalculationDatabase *db) {
    if (!db) return;
    db->count = 0;
    memset(db->records, 0, sizeof(CalculationRecord) * MAX_RECORDS);
}

double performOperation(const char *operation, double a, double b) {
    if (strcmp(operation, "sqrt") == 0) {
        return squareRoot(a);
    }
    for (int i = 0; i < num_ops; i++) {
        if (strcmp(operation, operation_map[i].op_name) == 0 && operation_map[i].func != NULL) {
            return operation_map[i].func(a, b);
        }
    }
    return 0;
}

int addRecord(CalculationDatabase *db, const char *name, double num1, double num2, const char *operation) {
    if (db->count >= MAX_RECORDS) {
        printf("Database full. Cannot add more records.\n");
        return -1;
    }

    CalculationRecord *rec = db->records + db->count;
    rec->id = ++db->count;
    
    snprintf(rec->name, MAX_NAME_LEN, "%s", name);
    snprintf(rec->operation, sizeof(rec->operation), "%s", operation);
    
    rec->num1 = num1;
    rec->num2 = num2;
    rec->result = performOperation(operation, num1, num2);

    return rec->id;
}

int findRecordById(CalculationDatabase *db, int id) {
    for (int i = 0; i < db->count; i++) {
        if (db->records[i].id == id) return i;
    }
    return -1;
}

void deleteRecord(CalculationDatabase *db, int id) {
    int index = findRecordById(db, id);
    if (index == -1) {
        printf("Record with ID %d not found.\n", id);
        return;
    }

    int items_to_move = db->count - index - 1;
    if (items_to_move > 0) {
        memmove(&db->records[index], &db->records[index + 1], sizeof(CalculationRecord) * items_to_move);
    }
    db->count--;
    printf("Record with ID %d deleted.\n", id);
}

void updateRecord(CalculationDatabase *db, int id, double newNum1, double newNum2) {
    int index = findRecordById(db, id);
    if (index == -1) {
        printf("Record with ID %d not found.\n", id);
        return;
    }

    CalculationRecord *rec = &db->records[index];
    rec->num1 = newNum1;
    rec->num2 = newNum2;
    rec->result = performOperation(rec->operation, newNum1, newNum2);
    printf("Record with ID %d updated.\n", id);
}
