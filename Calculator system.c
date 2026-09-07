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


/* ---------------------------------------------------------
 * Analysis, Utilities, and UI Presentation Layers
 * --------------------------------------------------------- */

double calculateAverage(CalculationDatabase *db) {
    if (!db || db->count == 0) return 0;
    double sum = 0;
    for (int i = 0; i < db->count; i++) {
        sum += db->records[i].result;
    }
    return sum / db->count;
}

double findMaxResult(CalculationDatabase *db) {
    if (!db || db->count == 0) return 0;
    double max = db->records[0].result;
    for (int i = 1; i < db->count; i++) {
        if (db->records[i].result > max) max = db->records[i].result;
    }
    return max;
}

double findMinResult(CalculationDatabase *db) {
    if (!db || db->count == 0) return 0;
    double min = db->records[0].result;
    for (int i = 1; i < db->count; i++) {
        if (db->records[i].result < min) min = db->records[i].result;
    }
    return min;
}

int compareRecords(const void *a, const void *b) {
    double resA = ((CalculationRecord *)a)->result;
    double resB = ((CalculationRecord *)b)->result;
    return (resA > resB) - (resA < resB);
}

void sortRecordsByResult(CalculationDatabase *db) {
    if (db && db->count > 1) {
        qsort(db->records, db->count, sizeof(CalculationRecord), compareRecords);
    }
    printf("Records sorted by result (ascending).\n");
}

int validateOperation(const char *operation) {
    for (int i = 0; i < num_ops; i++) {
        if (strcmp(operation, operation_map[i].op_name) == 0) return 1;
    }
    return 0;
}

void printSeparator(void) {
    printf("--------------------------------------------------------\n");
}

void printHeader(const char *title) {
    printSeparator();
    printf("           %s\n", title);
    printSeparator();
}

void printRecord(CalculationRecord *record) {
    if (record) {
        printf("ID: %-4d Name: %-15s Operation: %-6s %.2f, %.2f => Result: %.2f\n",
               record->id, record->name, record->operation,
               record->num1, record->num2, record->result);
    }
}

void printAllRecords(CalculationDatabase *db) {
    if (!db) return;
    printHeader("All Calculation Records");
    CalculationRecord *curr = db->records;
    for (int i = 0; i < db->count; i++, curr++) {
        printRecord(curr);
    }
    printSeparator();
}

void exportToCSVFormat(CalculationDatabase *db) {
    if (!db) return;
    printHeader("CSV Export Preview");
    printf("id,name,operation,num1,num2,result\n");
    for (int i = 0; i < db->count; i++) {
        CalculationRecord *r = db->records + i;
        printf("%d,%s,%s,%.2f,%.2f,%.2f\n", r->id, r->name, r->operation, r->num1, r->num2, r->result);
    }
    printSeparator();
}

void printMenu(void) {
    printSeparator();
    printf("           CALCULATOR MANAGEMENT SYSTEM MENU\n");
    printSeparator();
    printf("1. Add Record\n2. View All Records\n3. Update Record\n4. Delete Record\n"
           "5. Sort Records\n6. View Statistics\n7. Export to CSV Format\n8. Exit\n");
    printSeparator();
}
