#ifndef QUEUE_H
#define QUEUE_H

#include <string>
using namespace std;

// QUEUE PEMBUKAAN REKENING (ARRAY)
struct QueuePembukaan {
    string* nama;
    int* nomor;
    int front;
    int rear;
    int kapasitas;

    QueuePembukaan(int cap = 50) {
        kapasitas = cap;
        front = rear = -1;
        nama  = new string[kapasitas];
        nomor = new int[kapasitas];
    }

    ~QueuePembukaan() {
        delete[] nama;
        delete[] nomor;
    }
};

#endif
