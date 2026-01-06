#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>
#include "ktp.h"
using namespace std;

class CircularLinkedListKTP {
private:
    KTP* head;

public:
    CircularLinkedListKTP() {
        head = nullptr;
    }

    ~CircularLinkedListKTP() {
        if (!head) return;

        KTP* curr = head->next;
        while (curr != head) {
            KTP* temp = curr;
            curr = curr->next;
            delete temp;
        }
        delete head;
    }

    void tambahData(KTP data) {
        KTP* newNode = new KTP(data);
        newNode->next = nullptr;

        if (!head) {
            head = newNode;
            newNode->next = head;
            return;
        }

        KTP* temp = head;
        while (temp->next != head) temp = temp->next;

        temp->next = newNode;
        newNode->next = head;
    }

    void tampilkan() {
        if (!head) {
            cout << "Data masih kosong.\n";
            return;
        }

        cout << "\n============= DATA KTP (Circular Linked List) =============\n";

        KTP* temp = head;
        int nomor = 1;

        do {
            cout << "======================== Data ke-" << nomor++ << " ========================\n";
            cout << "NIK               : " << temp->nik << endl;
            cout << "Nama              : " << temp->nama << endl;
            cout << "Alamat            : " << temp->alamat << endl;

            cout << "Tanggal Lahir     : "
                 << temp->tanggalLahir.hari << "-"
                 << temp->tanggalLahir.bulan << "-"
                 << temp->tanggalLahir.tahun << endl;

            cout << "Jenis Kelamin     : ";
            switch (temp->jenisKelamin) {
                case LAKI_LAKI: cout << "LAKI-LAKI"; break;
                case PEREMPUAN: cout << "PEREMPUAN"; break;
                default: cout << "Tidak Diketahui"; break;
            }
            cout << endl;

            cout << "Agama             : ";
            switch (temp->agama) {
                case ISLAM: cout << "ISLAM"; break;
                case KRISTEN: cout << "KRISTEN"; break;
                case KATOLIK: cout << "KATOLIK"; break;
                case HINDU: cout << "HINDU"; break;
                case BUDDHA: cout << "BUDDHA"; break;
                case KHONGHUCU: cout << "KHONGHUCU"; break;
                default: cout << "LAINNYA"; break;
            }
            cout << endl;

            cout << "Status Perkawinan : ";
            switch (temp->statusPerkawinan) {
                case BELUM_KAWIN: cout << "BELUM KAWIN"; break;
                case KAWIN: cout << "KAWIN"; break;
                default: cout << "Tidak Diketahui"; break;
            }
            cout << endl;

            cout << "Pekerjaan         : " << temp->pekerjaan << endl;

            cout << "Kewarganegaraan   : ";
            switch (temp->kewarganegaraan) {
                case WNI: cout << "WNI"; break;
                case WNA: cout << "WNA"; break;
                default: cout << "Tidak Diketahui"; break;
            }
            cout << endl;
            cout << "Berlaku Hingga    : SEUMUR HIDUP\n";
            cout << endl;

            cout << "===========================================================\n";

            temp = temp->next;

        } while (temp != head);
    }
};

#endif