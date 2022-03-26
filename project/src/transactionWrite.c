#include "../include/transactionwrite.h"

void transactionwrite(const char* fname) {
	FILE *ofPtr;
	Data transfer;
	ofPtr = fopen(fname, "r+");
	if (ofPtr == NULL) {
		puts("Not acess");
	} else {
		printf("%s\n%s\n", "1 Number account: ", "2 Client cash payments: ");

		while (scanf("%d %lf", &transfer.Number, &transfer.cash_payments) != -1) {
			fprintf(ofPtr, "%-3d%-6.2f\n",transfer.Number, transfer.cash_payments);
			printf("%s\n%s\n","1 Number account:","2 Client cash payments: ");
		}
	}
	fclose(ofPtr);
}