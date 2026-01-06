#ifndef KTP_H
#define KTP_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// =====================
// Struktur Tanggal
// =====================
struct Tanggal {
    int hari;
    int bulan;
    int tahun;
};

// =====================
// Struktur Data KTP
// =====================
struct DataKTP {
    long long nik;

    string nama;
    string tempat_lahir;
    string gol_darah;
    string alamat;
    string kel_desa;
    string kecamatan;
    string pekerjaan;

    Tanggal tgl_lahir;

    bool jenis_kelamin;        // true = Laki-laki, false = Perempuan
    bool status_perkawinan;    // true = Sudah, false = Belum
    bool kewarganegaraan;      // true = WNI, false = WNA

    int rt;
    int rw;
    int agama;
    int berlaku_hingga;
};

// =================================================
//  DEFINISI CLASS KTP
// =================================================
class KTP {
private:
    DataKTP data;
    
    // Helper untuk mengubah angka menjadi teks (PENTING: Agar tidak error)
    string getAgamaTeks() const {
        switch (data.agama) {
            case 1: 
                return "Islam";
            case 2: 
                return "Kristen";
            case 3:
                return "Hindu";
            case 4: 
                return "Buddha";
            default: 
                return "Lainnya";
        }
    }

    string getBerlakuTeks() const {
        switch (data.berlaku_hingga) {
            case 1: 
                return "Sementara";
            case 2: 
                return "5 Tahun";
            case 3: 
                return "Selamanya";
            default: 
                return "-";
        }
    }

public:
    // =====================
    // Setter (Baru: Untuk memindahkan data dari NodeNasabah)
    // =====================
    void setData(long long nik, string nama, string alamat, string pekerjaan) {
        data.nik = nik;
        data.nama = nama;
        data.alamat = alamat;
        data.pekerjaan = pekerjaan;
    }

    // =====================
    // Input Data
    // =====================

void input(long long nikInput) {
    data.nik = nikInput;

    // Header Input
    cout << "\n========================================" << endl;
    cout << "        FORM INPUT DATA KTP             " << endl;
    cout << "========================================" << endl;
    cout << "NIK : " << data.nik << endl;

    // Bersihkan buffer setelah input NIK dari menu utama
    cin.ignore(); 

    // --- Identitas Pribadi ---
    cout << "Masukkan Nama Nasabah     : "; 
    getline(cin, data.nama);
    cout << "Masukkan Tempat Lahir     : "; 
    getline(cin, data.tempat_lahir);
    cout << "Masukkan Tanggal Lahir (DD MM YYYY): ";
    cin >> data.tgl_lahir.hari >> data.tgl_lahir.bulan >> data.tgl_lahir.tahun;

    // --- Status & Karakteristik ---
    cout << "Jenis Kelamin (1:L / 2:P) : ";
    int jk; cin >> jk; 
    data.jenis_kelamin = (jk == 1);
    
    cin.ignore(); // Bersihkan buffer sebelum getline
    cout << "Masukkan Golongan Darah   : "; 
    getline(cin, data.gol_darah);

    // --- Alamat & Domisili ---
    cout << "Masukkan Alamat Lengkap   : "; 
    getline(cin, data.alamat);
    cout << "Masukkan RT dan RW (00 00): "; 
    cin >> data.rt >> data.rw;
    
    cin.ignore(); // Bersihkan buffer sebelum getline
    cout << "Masukkan Kel/Desa         : "; 
    getline(cin, data.kel_desa);
    cout << "Masukkan Kecamatan        : "; 
    getline(cin, data.kecamatan);

    // --- Informasi Sosial ---
    cout << "Agama (1:Islam, 2:Krist, 3:Hnd, 4:Bud): ";
    cin >> data.agama;
    cout << "Status Perkawinan (1:Sdh / 2:Blm)     : ";
    int st; cin >> st; 
    data.status_perkawinan = (st == 1);
    
    cin.ignore(); // Bersihkan buffer sebelum getline
    cout << "Masukkan Pekerjaan        : "; 
    getline(cin, data.pekerjaan);

    // --- Kewarganegaraan & Masa Berlaku ---
    cout << "Kewarganegaraan (1:WNI / 2:WNA)       : ";
    int kw; cin >> kw; 
    data.kewarganegaraan = (kw == 1);
    cout << "Berlaku Hingga (1:Smt, 2:5th, 3:Seumur): ";
    cin >> data.berlaku_hingga;

    cout << "========================================" << endl;
    cout << "      Data Berhasil Diinput!            " << endl;
    cout << "========================================" << endl;
}

    // =====================
    // Getter
    // =====================
    long long getNIK() const {
        return data.nik;
    }

    string getNama() const {
        return data.nama;
    }

    bool getJK() const {
        return data.jenis_kelamin;
    }

    // =====================
    // Simpan ke File
    // =====================
    void simpan(ofstream &file) {
        file << data.nik << ';'
             << data.nama << ';'
             << data.tempat_lahir << ';'
             << data.tgl_lahir.hari << ';'
             << data.tgl_lahir.bulan << ';'
             << data.tgl_lahir.tahun << ';'
             << data.jenis_kelamin << ';'
             << data.gol_darah << ';'
             << data.alamat << ';'
             << data.rt << ';'
             << data.rw << ';'
             << data.kel_desa << ';'
             << data.kecamatan << ';'
             << data.agama << ';'
             << data.status_perkawinan << ';'
             << data.pekerjaan << ';'
             << data.kewarganegaraan << ';'
             << data.berlaku_hingga << '\n';
    }

// 2. Fungsi Simpan untuk Laporan KTP
    void simpanKeLaporan(ofstream &file, int index) const {
        file << "========================================\n";
        file << "Data KTP ke-" << index + 1 << "\n";
        file << "NIK               : " << data.nik << "\n";
        file << "Nama              : " << data.nama << "\n";
        file << "Tempat/Tgl Lahir  : " << data.tempat_lahir << ", "
             << data.tgl_lahir.hari << "-" << data.tgl_lahir.bulan << "-" << data.tgl_lahir.tahun << "\n";
        file << "Jenis Kelamin     : " << (data.jenis_kelamin ? "Laki-laki" : "Perempuan") << "\n";
        file << "Golongan Darah    : " << data.gol_darah << "\n";
        file << "Alamat            : " << data.alamat << "\n";
        file << "RT/RW             : " << data.rt << "/" << data.rw << "\n";
        file << "Kel/Desa          : " << data.kel_desa << "\n";
        file << "Kecamatan         : " << data.kecamatan << "\n";
        file << "Agama             : " << getAgamaTeks() << "\n";
        file << "Status Perkawinan : " << (data.status_perkawinan ? "Sudah Kawin" : "Belum Kawin") << "\n";
        file << "Pekerjaan         : " << data.pekerjaan << "\n";
        file << "Kewarganegaraan   : " << (data.kewarganegaraan ? "WNI" : "WNA") << "\n";
        file << "Berlaku Hingga    : " << getBerlakuTeks() << "\n";
        file << "========================================\n\n";
    }    

    // =====================
    // Baca dari File
    // =====================
    bool baca(const string &line) {
        stringstream ss(line);
        string token;

        try {
            getline(ss, token, ';'); data.nik = stoll(token);
            getline(ss, data.nama, ';');
            getline(ss, data.tempat_lahir, ';');

            getline(ss, token, ';'); data.tgl_lahir.hari = stoi(token);
            getline(ss, token, ';'); data.tgl_lahir.bulan = stoi(token);
            getline(ss, token, ';'); data.tgl_lahir.tahun = stoi(token);

            getline(ss, token, ';'); data.jenis_kelamin = stoi(token);

            getline(ss, data.gol_darah, ';');
            getline(ss, data.alamat, ';');

            getline(ss, token, ';'); data.rt = stoi(token);
            getline(ss, token, ';'); data.rw = stoi(token);

            getline(ss, data.kel_desa, ';');
            getline(ss, data.kecamatan, ';');

            getline(ss, token, ';'); data.agama = stoi(token);
            getline(ss, token, ';'); data.status_perkawinan = stoi(token);

            getline(ss, data.pekerjaan, ';');

            getline(ss, token, ';'); data.kewarganegaraan = stoi(token);
            getline(ss, token, ';'); data.berlaku_hingga = stoi(token);

            return true;
        }
        catch (...) {
            return false;
        }
    }
};

#endif // KTP_H