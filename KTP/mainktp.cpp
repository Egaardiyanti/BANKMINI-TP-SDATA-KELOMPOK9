#include <iostream>
#include "ktp.h"
#include "linkedlist.h"
#include "array1d.h"
using namespace std;

int main() {
    CircularLinkedListKTP linklist;    // struktur pertama
    Array1D arr(20);       // struktur kedua (kapasitas bebas)

    int pilihan;
    do {
        cout << "\n===== MENU DATA KTP =====\n";
        cout << "1. Tambah Data KTP\n";
        cout << "2. Tampilkan Data (LinkedList)\n";
        cout << "3. Tampilkan Data (Array1D)\n";
        cout << "4. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;

        if (pilihan == 1) {
            KTP ktpBaru;

            cout << "\nMasukkan NIK: ";
            cin >> ktpBaru.nik;

            cout << "Masukkan Nama: ";
            cin.ignore();
            getline(cin, ktpBaru.nama);

            cout << "Masukkan Tanggal Lahir (tanggal bulan tahun): ";
            cin >> ktpBaru.tanggalLahir.hari
                >> ktpBaru.tanggalLahir.bulan
                >> ktpBaru.tanggalLahir.tahun;
            
            cout << "Masukkan Jenis Kelamin (1: LAKI-LAKI, 2: PEREMPUAN): ";
			int jk;
			cin >> jk;
			if (jk == 1)
                ktpBaru.jenisKelamin = LAKI_LAKI;
            else
                ktpBaru.jenisKelamin = PEREMPUAN;
			cin.ignore();
            cout << "Masukkan Alamat: ";
            getline(cin, ktpBaru.alamat);

            cout << "Masukkan Agama (1:ISLAM, 2:KRISTEN, 3:KATOLIK, 4:HINDU, 5: BUDDHA, 6: KHONGHUCU): ";
            int ag;
            cin >> ag;
            ktpBaru.agama = (Agama)ag;

            cout << "Masukkan Status Perkawinan (1: BELUM KAWIN, 2: KAWIN): ";
            int st;
            cin >> st;
            if (st == 1)
                ktpBaru.statusPerkawinan = BELUM_KAWIN;
            else
                ktpBaru.statusPerkawinan = KAWIN;
			cin.ignore();
            cout << "Masukkan Pekerjaan: ";
            getline(cin, ktpBaru.pekerjaan);
            
            cout << "Kewarganegaraan (1: WNI, 2: WNA): ";
            int kw;
            cin >> kw;              
            if (kw == 1)
                ktpBaru.kewarganegaraan = WNI;
            else
                ktpBaru.kewarganegaraan = WNA;

            linklist.tambahData(ktpBaru); // masuk linkedlist
            arr.add(ktpBaru);                // masuk array1d

            cout << "\nData KTP berhasil ditambahkan!\n";

        } else if (pilihan == 2) { 
            linklist.tampilkan();

        } else if (pilihan == 3) {
            arr.displayAll();
        }

    } while (pilihan != 4);

    return 0;
}