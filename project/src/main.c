#include <stdio.h>
#include <stdlib.h>
#include "../include/structmasterrecord.h"
#include "../include/masterwrite.h"
#include "../include/transactionwrite.h"
#include "../include/blackrecord.h"

#define FILENAME_RECORD "record.dat"
#define FILENAME_TRANSACTION "transaction.dat"
#define FILENAME_BLACKRECORD "blackrecord.dat"

enum MAIN_CHOICE {
	RECORD_CHOICE = 1,
	TRANSACTION_CHOICE,
	BLACKRECORD_CHOICE
};

int main(void) {
int choice = 0;
while (scanf("%d", &choice) != -1) {
printf("%s", "please enter action\n1 - enter data client:\n2 - enter data transaction:\n3 - update base:\n\n");
switch (choice) {
		case RECORD_CHOICE:
			masterwrite(FILENAME_RECORD);

			break;
		case TRANSACTION_CHOICE:
			transactionwrite(FILENAME_TRANSACTION);

			break;
		case BLACKRECORD_CHOICE:
			blackRecord(FILENAME_RECORD, FILENAME_TRANSACTION, FILENAME_BLACKRECORD);

			break;
		default:
			puts("error");

			break;
		}
	}
	return 0;
}
