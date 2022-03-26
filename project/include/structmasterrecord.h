#ifndef PROJECT_INCLUDE_STRUCTMASTERRECORD_H_
#define PROJECT_INCLUDE_STRUCTMASTERRECORD_H_

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

Data client_data, transfer;

#endif  // PROJECT_INCLUDE_STRUCTMASTERRECORD_H_
