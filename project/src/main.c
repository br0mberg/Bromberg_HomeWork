#include "recordclientdata.h"
#include "recordtransactiondata.h"
#include "updclientdata.h"
#include "customtest.h"
#include "structs.h"

#define F_RECORD "record.dat"
#define F_TRANSACTION "transaction.dat"
#define F_UPDATE_DATA "blackrecord.dat"

enum MAIN_CHOICE {
    RECORD_CLIENT_CHOICE = 1,
    RECORD_TRANSACTION_CHOICE,
    UPDATE_CLIENT_CHOICE
};


int main(void) {
    int choice = 0;

    while (scanf("%d", &choice) == 1) {
        printf("%s%s%s%s", "please enter action\n",
        "1 - enter data client:\n",
        "2 - enter data transaction:\n",
        "3 - update base:\n\n");
        switch (choice) {
                case RECORD_CLIENT_CHOICE:
                    Record_client_data(F_RECORD);

                    break;
                case RECORD_TRANSACTION_CHOICE:
                    Record_transaction_data(F_TRANSACTION);

                    break;
                case UPDATE_CLIENT_CHOICE:
                    Upd_client_data(F_RECORD, F_TRANSACTION, F_UPDATE_DATA);

                    break;
                default:
                    puts("error");

                    break;
                }
            }

    return 0;
}
