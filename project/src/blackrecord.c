#include "../include/blackrecord.h"

void blackRecord(const char* fname_record, const char* fname_transaction, const char* fname_blackrecord) {
	FILE *ofPTR, *ofPTR_2, *blackrecord;
	Data client_data; 
	Data transfer;
	ofPTR = fopen(fname_record, "r+");
	ofPTR_2 = fopen(fname_transaction, "r+");
	blackrecord = fopen(fname_blackrecord, "w+");
	if (ofPTR == NULL || ofPTR_2 == NULL || blackrecord == NULL) {
		puts("exit");
	} else {
		while (fscanf(ofPTR, "%d%99s%99s%99s%99s%lf%lf%lf", &client_data.Number, client_data.Name, client_data.Surname, 
		client_data.addres, client_data.TelNumber, &client_data.indebtedness, &client_data.credit_limit, &client_data.cash_payments) != -1) {
			while (fscanf(ofPTR_2, "%d %lf", &transfer.Number, &transfer.cash_payments) != -1) {
				if (client_data.Number == transfer.Number && transfer.cash_payments != 0) {
					client_data.credit_limit += transfer.cash_payments;
				}
			}
			fprintf(blackrecord, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n", client_data.Number, client_data.Name, client_data.Surname, client_data.addres, client_data.TelNumber, client_data.indebtedness, client_data.credit_limit, client_data.cash_payments);
			rewind(ofPTR_2);
		}
	}
	fclose(ofPTR);
	fclose(ofPTR_2);
	fclose(blackrecord);
}