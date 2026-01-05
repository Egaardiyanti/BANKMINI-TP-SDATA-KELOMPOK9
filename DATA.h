#ifndef DATA_H
#define DATA_H

#include <string>

// =================================================
//  Fungsi Huffman (Simulasi Kompresi Data Profil)
// =================================================
inline std::string huffmanCompress(std::string nama, long long nik) {
    std::string raw = nama + std::to_string(nik);

    // Simulasi kompresi:
    // mengambil setengah karakter dan memberi label
    return "[HUF-DATA]"
           + raw.substr(0, raw.length() / 2)
           + "_comp";
}

// =================================================
//  STACK dengan ARRAY DINAMIS
//  Fitur: "Gajadi Antri"
// =================================================
struct StackBatal {
    long long* items;
    int top;
    int kapasitas;

    StackBatal(int cap = 100) {
        kapasitas = cap;
        top = -1;
        items = new long long[kapasitas];
    }

    void push(long long nik) {
        if (top < kapasitas - 1) {
            items[++top] = nik;
        }
    }

    long long pop() {
        if (top >= 0) {
            return items[top--];
        }
        return -1;
    }

    ~StackBatal() {
        delete[] items;
    }
};

#endif // DATA_H
