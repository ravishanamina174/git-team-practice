/*
 * File: calculator_system.c
 * Description: Large dummy C program simulating a "Calculator Management System".
 * Note: This is a placeholder/sample file for demonstration purposes only.
 *       It is NOT a real working production system.
 */

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

/* ---------------------------------------------------------
 * Function Prototypes
 * --------------------------------------------------------- */

void initDatabase(CalculationDatabase *db);
int addRecord(CalculationDatabase *db, const char *name, double num1, double num2, const char *operation);
void printRecord(CalculationRecord *record);
void printAllRecords(CalculationDatabase *db);
double addNumbers(double a, double b);
double subtractNumbers(double a, double b);
double multiplyNumbers(double a, double b);
double divideNumbers(double a, double b);
double powerNumbers(double base, double exponent);
double squareRoot(double value);
int findRecordById(CalculationDatabase *db, int id);
void deleteRecord(CalculationDatabase *db, int id);
void updateRecord(CalculationDatabase *db, int id, double newNum1, double newNum2);
double calculateAverage(CalculationDatabase *db);
double findMaxResult(CalculationDatabase *db);
double findMinResult(CalculationDatabase *db);
void sortRecordsByResult(CalculationDatabase *db);
void exportToCSVFormat(CalculationDatabase *db);
void printMenu(void);
void printSeparator(void);
void printHeader(const char *title);
int validateOperation(const char *operation);
double performOperation(const char *operation, double a, double b);
void generateSampleData(CalculationDatabase *db);
void printStatistics(CalculationDatabase *db);
int countByOperation(CalculationDatabase *db, const char *operation);
void reverseRecords(CalculationDatabase *db);

/* ---------------------------------------------------------
 * Function Implementations
 * --------------------------------------------------------- */

void initDatabase(CalculationDatabase *db) {
    db->count = 0;
    memset(db->records, 0, sizeof(db->records));
}

int addRecord(CalculationDatabase *db, const char *name, double num1, double num2, const char *operation) {
    if (db->count >= MAX_RECORDS) {
        printf("Database full. Cannot add more records.\n");
        return -1;
    }

    CalculationRecord *rec = &db->records[db->count];
    rec->id = db->count + 1;
    strncpy(rec->name, name, MAX_NAME_LEN - 1);
    rec->name[MAX_NAME_LEN - 1] = '\0';
    rec->num1 = num1;
    rec->num2 = num2;
    strncpy(rec->operation, operation, sizeof(rec->operation) - 1);
    rec->operation[sizeof(rec->operation) - 1] = '\0';
    rec->result = performOperation(operation, num1, num2);

    db->count++;
    return rec->id;
}

void printRecord(CalculationRecord *record) {
    printf("ID: %-4d Name: %-15s Operation: %-6s %.2f, %.2f => Result: %.2f\n",
           record->id, record->name, record->operation,
           record->num1, record->num2, record->result);
}

void printAllRecords(CalculationDatabase *db) {
    printHeader("All Calculation Records");
    for (int i = 0; i < db->count; i++) {
        printRecord(&db->records[i]);
    }
    printSeparator();
}

double addNumbers(double a, double b) {
    return a + b;
}

double subtractNumbers(double a, double b) {
    return a - b;
}

double multiplyNumbers(double a, double b) {
    return a * b;
}

double divideNumbers(double a, double b) {
    if (b == 0) {
        printf("Warning: Division by zero attempted.\n");
        return 0;
    }
    return a / b;
}

double powerNumbers(double base, double exponent) {
    return pow(base, exponent);
}

double squareRoot(double value) {
    if (value < 0) {
        printf("Warning: Cannot compute square root of negative number.\n");
        return 0;
    }
    return sqrt(value);
}

int findRecordById(CalculationDatabase *db, int id) {
    for (int i = 0; i < db->count; i++) {
        if (db->records[i].id == id) {
            return i;
        }
    }
    return -1;
}

void deleteRecord(CalculationDatabase *db, int id) {
    int index = findRecordById(db, id);
    if (index == -1) {
        printf("Record with ID %d not found.\n", id);
        return;
    }

    for (int i = index; i < db->count - 1; i++) {
        db->records[i] = db->records[i + 1];
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

double calculateAverage(CalculationDatabase *db) {
    if (db->count == 0) return 0;

    double sum = 0;
    for (int i = 0; i < db->count; i++) {
        sum += db->records[i].result;
    }
    return sum / db->count;
}

double findMaxResult(CalculationDatabase *db) {
    if (db->count == 0) return 0;

    double max = db->records[0].result;
    for (int i = 1; i < db->count; i++) {
        if (db->records[i].result > max) {
            max = db->records[i].result;
        }
    }
    return max;
}

double findMinResult(CalculationDatabase *db) {
    if (db->count == 0) return 0;

    double min = db->records[0].result;
    for (int i = 1; i < db->count; i++) {
        if (db->records[i].result < min) {
            min = db->records[i].result;
        }
    }
    return min;
}

void sortRecordsByResult(CalculationDatabase *db) {
    for (int i = 0; i < db->count - 1; i++) {
        for (int j = 0; j < db->count - i - 1; j++) {
            if (db->records[j].result > db->records[j + 1].result) {
                CalculationRecord temp = db->records[j];
                db->records[j] = db->records[j + 1];
                db->records[j + 1] = temp;
            }
        }
    }
    printf("Records sorted by result (ascending).\n");
}

void exportToCSVFormat(CalculationDatabase *db) {
    printHeader("CSV Export Preview");
    printf("id,name,operation,num1,num2,result\n");
    for (int i = 0; i < db->count; i++) {
        CalculationRecord *r = &db->records[i];
        printf("%d,%s,%s,%.2f,%.2f,%.2f\n",
               r->id, r->name, r->operation, r->num1, r->num2, r->result);
    }
    printSeparator();
}

void printMenu(void) {
    printSeparator();
    printf("           CALCULATOR MANAGEMENT SYSTEM MENU\n");
    printSeparator();
    printf("1. Add Record\n");
    printf("2. View All Records\n");
    printf("3. Update Record\n");
    printf("4. Delete Record\n");
    printf("5. Sort Records\n");
    printf("6. View Statistics\n");
    printf("7. Export to CSV Format\n");
    printf("8. Exit\n");
    printSeparator();
}

void printSeparator(void) {
    printf("--------------------------------------------------------\n");
}

void printHeader(const char *title) {
    printSeparator();
    printf("           %s\n", title);
    printSeparator();
}

int validateOperation(const char *operation) {
    const char *validOps[] = {"add", "sub", "mul", "div", "pow", "sqrt"};
    int numOps = sizeof(validOps) / sizeof(validOps[0]);

    for (int i = 0; i < numOps; i++) {
        if (strcmp(operation, validOps[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

double performOperation(const char *operation, double a, double b) {
    if (strcmp(operation, "add") == 0) {
        return addNumbers(a, b);
    } else if (strcmp(operation, "sub") == 0) {
        return subtractNumbers(a, b);
    } else if (strcmp(operation, "mul") == 0) {
        return multiplyNumbers(a, b);
    } else if (strcmp(operation, "div") == 0) {
        return divideNumbers(a, b);
    } else if (strcmp(operation, "pow") == 0) {
        return powerNumbers(a, b);
    } else if (strcmp(operation, "sqrt") == 0) {
        return squareRoot(a);
    }
    printf("Unknown operation: %s\n", operation);
    return 0;
}

void generateSampleData(CalculationDatabase *db) {
    addRecord(db, "Sample1", 10, 5, "add");
    addRecord(db, "Sample2", 20, 8, "sub");
    addRecord(db, "Sample3", 6, 7, "mul");
    addRecord(db, "Sample4", 100, 4, "div");
    addRecord(db, "Sample5", 2, 10, "pow");
    addRecord(db, "Sample6", 81, 0, "sqrt");
    addRecord(db, "Sample7", 15, 15, "add");
    addRecord(db, "Sample8", 50, 25, "sub");
    addRecord(db, "Sample9", 9, 9, "mul");
    addRecord(db, "Sample10", 200, 10, "div");
    printf("Sample data generated: %d records.\n", db->count);
}

void printStatistics(CalculationDatabase *db) {
    printHeader("Database Statistics");
    printf("Total Records : %d\n", db->count);
    printf("Average Result: %.2f\n", calculateAverage(db));
    printf("Max Result    : %.2f\n", findMaxResult(db));
    printf("Min Result    : %.2f\n", findMinResult(db));
    printf("Add Operations: %d\n", countByOperation(db, "add"));
    printf("Sub Operations: %d\n", countByOperation(db, "sub"));
    printf("Mul Operations: %d\n", countByOperation(db, "mul"));
    printf("Div Operations: %d\n", countByOperation(db, "div"));
    printSeparator();
}

int countByOperation(CalculationDatabase *db, const char *operation) {
    int count = 0;
    for (int i = 0; i < db->count; i++) {
        if (strcmp(db->records[i].operation, operation) == 0) {
            count++;
        }
    }
    return count;
}

void reverseRecords(CalculationDatabase *db) {
    int start = 0;
    int end = db->count - 1;

    while (start < end) {
        CalculationRecord temp = db->records[start];
        db->records[start] = db->records[end];
        db->records[end] = temp;
        start++;
        end--;
    }
    printf("Record order reversed.\n");
}

/* ---------------------------------------------------------
 * Main Function
 * --------------------------------------------------------- */

int main() {
    CalculationDatabase db;
    initDatabase(&db);

    printHeader("Calculator Management System (Dummy)");
    printf("Note: This is a sample/dummy system for demonstration only.\n");

    generateSampleData(&db);
    printAllRecords(&db);

    printf("\nUpdating record ID 2...\n");
    updateRecord(&db, 2, 100, 50);
    printAllRecords(&db);

    printf("\nSorting records by result...\n");
    sortRecordsByResult(&db);
    printAllRecords(&db);

    printStatistics(&db);

    printf("\nDeleting record ID 5...\n");
    deleteRecord(&db, 5);
    printAllRecords(&db);

    printf("\nReversing record order...\n");
    reverseRecords(&db);
    printAllRecords(&db);

    exportToCSVFormat(&db);

    printf("\nProgram finished (dummy demonstration).\n");

    return 0;
}