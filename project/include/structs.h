#ifndef PROJECT_INCLUDE_STRUCTS_H_
#define PROJECT_INCLUDE_STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>

#define CLIENT_STRUCT_FIELDS 8
#define TRASNSATION_STRUCT_FIELDS 2
enum STRUCT_CONST {NAME_SIZE = 20, SNAME_CONST = 20, ADDRES_CONST = 30, TNUM_CONST = 15};
enum OUTMODE_STRUCT {CLIENT_OUTMODE = 1, TRANSFER_OUTMODE};

struct ClientCreditData {
    int Number;
    char Name[NAME_SIZE];
    char Surname[SNAME_CONST];
    char addres[ADDRES_CONST];
    char TelNumber[TNUM_CONST];
    double indebtedness;
    double credit_limit;
    double cash_payments;
};

typedef struct ClientCreditData CreditData;


#endif  // PROJECT_INCLUDE_STRUCTS_H_
