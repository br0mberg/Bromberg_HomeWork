#include "../include/recordclientdata.h"
#include "../include/recordtransactiondata.h"
#include "../include/updclientdata.h"
#include "../include/customtest.h"
#include "../include/structs.h"

#define FILENAME_RECORD "record.dat"
#define FILENAME_TRANSACTION "transaction.dat"
#define FILENAME_BLACKRECORD "blackrecord.dat"

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
                    recordclientdata(FILENAME_RECORD);

                    break;
                case RECORD_TRANSACTION_CHOICE:
                    recordtransactiondata(FILENAME_TRANSACTION);

                    break;
                case UPDATE_CLIENT_CHOICE:
                    updclientdata(FILENAME_RECORD, FILENAME_TRANSACTION, FILENAME_BLACKRECORD);

                    break;
                default:
                    puts("error");

                    break;
                }
            }

    return 0;
}
