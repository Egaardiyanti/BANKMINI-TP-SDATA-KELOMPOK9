#include "BankMini.h"

int main() {
    initQueue();

    jenisTrans  = new string[kapasitasStack];
    jumlahTrans = new double[kapasitasStack];

    Admin adminUtama;
    adminUtama.loadDataDariFile();

    BankMini bank;
    bank.mainMenu(adminUtama);

    return 0;
}
