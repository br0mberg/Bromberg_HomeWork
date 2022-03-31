#ifndef PROJECT_INCLUDE_STRUCTS_H_
#define PROJECT_INCLUDE_STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>

enum OUTMODE_STRUCT {CLIENT_OUTMODE = 1, TRANSFER_OUTMODE};
struct masterRecord {
    int Number;
    char Name[20];
    char Surname[20];
    char addres[30];
    char TelNumber[15];
    double indebtedness;
    double credit_limit;
    double cash_payments;
};

typedef struct masterRecord Data;


#endif  // PROJECT_INCLUDE_STRUCTS_H_
