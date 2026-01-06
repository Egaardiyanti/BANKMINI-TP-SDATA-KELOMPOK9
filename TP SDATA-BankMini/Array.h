#ifndef ARRAY_H
#define ARRAY_H

#include <string>
using namespace std;

// =============================
// ARRAY DINAMIS TRANSAKSI
// =============================

struct ArrayTransaksi {
    string* jenis;
    double* jumlah;
    int size;
    int kapasitas;

    ArrayTransaksi(int cap = 100) {
        kapasitas = cap;
        size = 0;
        jenis  = new string[kapasitas];
        jumlah = new double[kapasitas];
    }

    ~ArrayTransaksi() {
        delete[] jenis;
        delete[] jumlah;
    }
};

#endif
