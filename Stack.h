#ifndef STACK_H
#define STACK_H

#include <string>
using namespace std;

// =============================
// STACK TRANSAKSI
// =============================

struct StackTransaksi {
    string* jenis;
    double* jumlah;
    int top;
    int kapasitas;

    StackTransaksi(int cap = 100) {
        kapasitas = cap;
        top = -1;
        jenis  = new string[kapasitas];
        jumlah = new double[kapasitas];
    }

    ~StackTransaksi() {
        delete[] jenis;
        delete[] jumlah;
    }
};

#endif