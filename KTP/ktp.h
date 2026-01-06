#ifndef KTP_H
#define KTP_H

#include <string>
using namespace std;

enum Kewarganegaraan {
    WNI = 1,
    WNA
};

// 1 = ISLAM, 2 = KRISTEN, 3 = KATOLIK, dst
enum Agama {
    ISLAM = 1,
    KRISTEN,
    KATOLIK,
    HINDU,
    BUDDHA,
    KHONGHUCU
};

enum JenisKelamin {
    LAKI_LAKI = 1,
    PEREMPUAN
};


enum StatusPerkawinan {
    BELUM_KAWIN = 1,
    KAWIN
};

struct Date {
    int hari, bulan, tahun;
};

struct KTP {
    long long int nik;
    string nama;
    string alamat;
    Date tanggalLahir;

    JenisKelamin jenisKelamin;
    Agama agama;
    StatusPerkawinan statusPerkawinan;  
    string pekerjaan;
    Kewarganegaraan kewarganegaraan;

    KTP* next; 
};

#endif