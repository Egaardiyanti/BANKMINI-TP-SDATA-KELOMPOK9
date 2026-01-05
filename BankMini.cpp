#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>
#include "ktp.h"
#include "data.h"

using namespace std;

// =========================================================
// 1. STRUKTUR DATA
// =========================================================

struct Transaksi {
    string jenis;  
    double jumlah; 
    string waktu;
    Transaksi* next;
};

struct Log {
    string aksi, waktu;
    Log *next, *prev;
};

struct Antrian {
    int no;
    string nama;
    Antrian* next;
};

struct NodeNasabah {
    int id;
    string nama;
    string nik;
    string jk;
    string tglLahir;
    string alamat;
    string telp;
    string pekerjaan;
    string noRek;
    double saldo;
    string profHuffman; //Kode Huffman

    struct RekeningNode {
        string norek;
        double saldo;
        RekeningNode *left, *right;
    };

    RekeningNode* rootRekening;
    string* daftarRek;
    int jumlahRek, kapasitasRek;
    string* riwayat;
    int jmlRiwayat, kapasitas;
    NodeNasabah* next;
};

// =========================================================
// 2. VARIABEL GLOBAL & HELPER
// =========================================================

string* jenisTrans;
double* jumlahTrans;
int kapasitasStack = 100, topStack = -1;

string* qNama;
int* qNomor;
int frontQ = -1, rearQ = -1, kapasitasQ = 50;

string waktuSekarang() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string waktu = dt;
    if (!waktu.empty()) waktu.pop_back();
    return waktu;
}

void initQueue() {
    qNama  = new string[kapasitasQ];
    qNomor = new int[kapasitasQ];
}

bool isEmptyQ() { 
    return frontQ == -1; 
}
bool isFullQ() { 
    return rearQ == kapasitasQ - 1; 
}

//Queue - Enqueue dan Dequeue
void enqueueQ(const string& nama, int nomor)
{
    if (isFullQ()) {
        cout << "Antrian pembukaan rekening penuh!\n";
        return;
    }

    // Jika antrian kosong, atur front ke 0
    if (isEmptyQ()) {
        frontQ = 0;
    }

    rearQ++;                        // Pindah ke posisi berikutnya
    qNama[rearQ]  = nama;           // Simpan nama
    qNomor[rearQ] = nomor;          // Simpan nomor antrian

    cout << "Nomor antrian pembukaan rekening: " << nomor << endl;
}

void dequeueQ()
{
    if (isEmptyQ()) {
        cout << "Tidak ada antrian pembukaan rekening.\n";
        return;
    }

    // Proses nasabah di posisi depan antrian
    cout << "Sedang memproses pembukaan rekening: "
         << qNama[frontQ] << " (No. " << qNomor[frontQ] << ")\n";

    frontQ++;  // Majukan pointer front

    // Jika antrian menjadi kosong setelah dequeue, reset pointer
    if (frontQ > rearQ) {
        frontQ = -1;
        rearQ  = -1;
    }
}

void tampilQueueQ()
{
    if (isEmptyQ()) {
        cout << "Antrian pembukaan rekening kosong.\n";
        return;
    }

    cout << "\n=== Daftar Antrian Pembukaan Rekening ===\n";

    // Tampilkan semua elemen dari front hingga rear
    for (int i = frontQ; i <= rearQ; i++) {
        cout << qNomor[i] << " - " << qNama[i] << endl;
    }

    cout << endl;  // Baris kosong di akhir untuk pemisah yang rapi
}

// =========================================================
// 3. DEFINISI CLASS
// =========================================================

class Admin {
private:
    NodeNasabah *head = nullptr;
    Log *headLog = nullptr;
    Log *tailLog = nullptr;
    Antrian *front = nullptr;
    Antrian *rear = nullptr;
    int nomorAntrian = 1;
    string usernameAdmin = "admin";
    string passwordAdmin = "12345";

public:
    int hitungNasabah();
    void tambahNodeNasabah(NodeNasabah* baru);
    int getLastID();
    NodeNasabah* getNasabahById(int id);
    void setSaldoNasabah(int id, double saldoBaru);
    bool login();
    void catatLog(string aksi);
    void tampilLog();
    void tambahNasabah();
    void lihatNasabah();
    void hapusNasabah();
    void tampilNasabah(NodeNasabah* ptr);
    void cariByID();
    void cariByNama();
    void cariByNIK();
    void cariByNoRek();
    void cariNasabah();
    void laporanTotalNasabah();
    void statistikSaldo();
    void backupSaldo();
    void tambahAntrian(string nama);
    void layaniAntrian();
    void tampilAntrian();
    NodeNasabah::RekeningNode* insertRekening(NodeNasabah::RekeningNode* root, string norek, double saldo);
    NodeNasabah::RekeningNode* cariRekening(NodeNasabah::RekeningNode* root, string norek);
    void tampilInOrder(NodeNasabah::RekeningNode* root);
    void tambahKeArrayRekening(NodeNasabah* n, string norek);
    void tambahRekeningNasabah();
    void transferRekening();
    void loadDataDariFile();
    void updateFile();
    void menu();
};

// === IMPLEMENTASI CLASS ADMIN ===

int Admin::hitungNasabah()
{
    int count = 0;
    NodeNasabah* temp = head;

    while (temp != nullptr) {
        count++;
        temp = temp->next;
    }

    return count;
}

//Singly Linked List - Tambah Node Nasabah
void Admin::tambahNodeNasabah(NodeNasabah* baru)
{
    if (head == nullptr) {
        head = baru;
    }
    else {
        NodeNasabah* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = baru;
    }
}

void Admin::loadDataDariFile() {
    std::ifstream file("data_ktp.txt");
    std::string line;
    
    if (!file.is_open()) return;

    while (getline(file, line)) {
        if (line.find("ID Nasabah") != std::string::npos) {
            NodeNasabah* baru = new NodeNasabah();
            
            size_t pos = line.find(":");
            baru->id = std::stoi(line.substr(pos + 1));

            // Inisialisasi Alokasi Memori (Penting agar tidak crash)
            baru->rootRekening = nullptr; 
            baru->kapasitasRek = 5; 
            baru->jumlahRek = 0;
            baru->daftarRek = new string[baru->kapasitasRek];

            baru->kapasitas = 100;
            baru->jmlRiwayat = 0;
            baru->riwayat = new string[baru->kapasitas];

            // Inisialisasi string kosong
            baru->nama = "";
            baru->nik = "";
            baru->jk = "";
            baru->tglLahir = "";
            baru->alamat = "";
            baru->pekerjaan = "";
            baru->noRek = "";
            baru->telp = "-"; 
            baru->saldo = 0; 

            while (getline(file, line) && line.find("====") == std::string::npos) {
                if (line.find("NIK") != std::string::npos) {
                    baru->nik = line.substr(line.find(":") + 2);
                }
                else if (line.find("Nama") != std::string::npos) {
                    baru->nama = line.substr(line.find(":") + 2);
                } 
                else if (line.find("Tanggal Lahir") != std::string::npos) {
                    baru->tglLahir = line.substr(line.find(":") + 2);
                }
                else if (line.find("Jenis Kelamin") != std::string::npos) {
                    baru->jk = line.substr(line.find(":") + 2);
                }
                else if (line.find("Alamat") != std::string::npos) {
                    baru->alamat = line.substr(line.find(":") + 2);
                }
                else if (line.find("Pekerjaan") != std::string::npos) {
                    baru->pekerjaan = line.substr(line.find(":") + 2);
                }
                else if (line.find("No. Rekening") != std::string::npos) {
                    baru->noRek = line.substr(line.find(":") + 2);
                    baru->telp = baru->noRek; 
                }
                else if (line.find("Saldo") != std::string::npos) {
                    baru->saldo = std::stod(line.substr(line.find(":") + 1));
                }
            }

            // Memasukkan ke Linked List (Append belakang)
            baru->next = nullptr;
            if (head == nullptr) {
                head = baru;
            } else {
                NodeNasabah* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = baru;
            }
        }
    }
    file.close();
}

int Admin::getLastID()
{
    int last = 1000;
    NodeNasabah* temp = head;

    while (temp != nullptr) {
        if (temp->id > last) {
            last = temp->id;
        }
        temp = temp->next;
    }

    return last;
}

NodeNasabah* Admin::getNasabahById(int id)
{
    NodeNasabah* temp = head;

    while (temp != nullptr) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }

    return nullptr;
}

void Admin::setSaldoNasabah(int id, double saldoBaru)
{
    NodeNasabah* temp = getNasabahById(id);

    if (temp != nullptr) {
        temp->saldo = saldoBaru;
    }
}

bool Admin::login()
{
    string user, pass;

    while (true) {
        cout << "\n============================================\n";
        cout << "||                                        ||\n";
        cout << "||          === LOGIN ADMIN ===           ||\n";
        cout << "||                                        ||\n";
        cout << "============================================\n";

        cout << "|| Username : ";
        cin.ignore();
        getline(cin, user);

        cout << "|| Password : ";
        getline(cin, pass);

        cout << "============================================\n";

        if (user == usernameAdmin && pass == passwordAdmin) {
            cout << "||     Login berhasil! Selamat datang     ||\n";
            cout << "============================================\n\n";
            return true;
        }
        else {
            cout << "||      Username atau password salah!     ||\n";
            cout << "||             Silakan coba lagi.         ||\n";
            cout << "===========================================\n\n";
        }
    }
}

// Doubly Linked List - Catat Log Admin
void Admin::catatLog(string aksi)
{
    Log* baru = new Log{aksi, waktuSekarang(), nullptr, nullptr};

    if (headLog == nullptr) {
        headLog = tailLog = baru;
    }
    else {
        tailLog->next = baru;
        baru->prev = tailLog;
        tailLog = baru;
    }
}

void Admin::tampilLog()
{
    if (headLog == nullptr) {
        cout << "Belum ada aktivitas admin.\n\n";
        return;
    }

    Log* temp = headLog;

    cout << "\n==================================================\n";
    cout << "=== LOG AKTIVITAS ADMIN (dari awal ke akhir) ===\n";
    cout << "==================================================\n";

    while (temp != nullptr) {
        cout << "[" << temp->waktu << "] " << temp->aksi << endl;
        temp = temp->next;
    }

    cout << "\n==================================================\n";
    cout << "=== LOG AKTIVITAS ADMIN (dari akhir ke awal) ===\n";
    cout << "==================================================\n";

    temp = tailLog;

    while (temp != nullptr) {
        cout << "[" << temp->waktu << "] " << temp->aksi << endl;
        temp = temp->prev;
    }

    cout << "\n";
}

void Admin::tambahNasabah() {
    NodeNasabah* baru = new NodeNasabah;

    cout << "\n*****************************************************" << endl;
    cout << "*************                          **************" << endl;
    cout << "**********                                ***********" << endl;
    cout << "***********      M E N U   T A M B A H   ************" << endl;
    cout << "**********          N A S A B A H         ***********" << endl;
    cout << "*************                          **************" << endl;
    cout << "*****************************************************" << endl;

    // Input ID dan Inisialisasi Awal
    baru->id = getLastID() + 1;
    cout << "\nID Nasabah: " << baru->id << endl;
    cin.ignore();

    baru->rootRekening = nullptr; 
    baru->kapasitasRek = 5; 
    baru->jumlahRek = 0;
    baru->daftarRek = new string[baru->kapasitasRek];

// Pengecekan Duplikasi ID
    NodeNasabah* cek = head;
    while (cek != nullptr) {
        if (cek->id == baru->id) { 
            cout << "ID sudah digunakan!\n"; 
            delete baru; 
            return; 
        }
        cek = cek->next;
    }

// Input Data Diri Nasabah
    cout << "Masukkan Nama Nasabah              : "; 
    getline(cin, baru->nama);
    cout << "Masukkan NIK                       : "; 
    getline(cin, baru->nik);
    cout << "Masukkan Jenis Kelamin (L/P)       : "; 
    getline(cin, baru->jk);
    cout << "Masukkan Tanggal Lahir (DD-MM-YYYY): "; 
    getline(cin, baru->tglLahir);
    cout << "Masukkan Alamat                    : "; 
    getline(cin, baru->alamat);
    cout << "Masukkan No. Telepon               : "; 
    getline(cin, baru->telp);
    cout << "Masukkan Pekerjaan                 : "; 
    getline(cin, baru->pekerjaan);
    cout << "Masukkan No. Rekening              : "; 
    getline(cin, baru->noRek);

// Inisialisasi Saldo dan Riwayat
    baru->saldo = 0; 
    baru->next = nullptr;
    baru->kapasitas = 100; 
    baru->jmlRiwayat = 0;
    baru->riwayat = new string[baru->kapasitas];

// Memasukkan Node Baru ke Akhir Linked List (Append)
    if (head == nullptr) {
        head = baru;
    } else {
        NodeNasabah* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = baru;
    }

    cout << "Nasabah berhasil ditambahkan!\n";
   
    catatLog("Admin menambah nasabah baru: " + baru->nama);

    // ========================================================
    // SIMPAN KE FILE TEKS KTP
    // ========================================================
    ofstream fileKTP("data_ktp.txt", ios::app); 
    
    if (fileKTP.is_open()) {
        int index = hitungNasabah();

        fileKTP << "========================================" << endl;
        fileKTP << "Data KTP ke-      : " << index << endl;
        fileKTP << "ID Nasabah        : " << baru->id << endl;
        fileKTP << "NIK               : " << baru->nik << endl;
        fileKTP << "Nama              : " << baru->nama << endl;
        fileKTP << "Tanggal Lahir     : " << baru->tglLahir << endl;
        fileKTP << "Jenis Kelamin     : " << (baru->jk == "L" ? "Laki-laki" : "Perempuan") << endl;
        fileKTP << "Alamat            : " << baru->alamat << endl;
        fileKTP << "Pekerjaan         : " << baru->pekerjaan << endl;
        fileKTP << "No. Telp          : " << baru->telp << endl;
        fileKTP << "No. Rekening      : " << baru->noRek << endl;
        fileKTP << "Saldo             : " << baru->saldo << endl;
        fileKTP << "========================================" << endl << endl;

        fileKTP.close();
        cout << "Data KTP otomatis tersimpan ke data_ktp.txt" << endl;
    } else {
        cout << "Peringatan: Gagal membuka file database KTP!" << endl;
    }
}

void Admin::updateFile() {
    ofstream file("data_ktp.txt", ios::trunc); // 'trunc' menghapus isi lama agar tidak duplikat
    if (!file.is_open()) return;

    NodeNasabah* temp = head;
    int i = 1;
    while (temp != nullptr) {
        file << "========================================" << endl;
        file << "Data KTP ke-      : " << i++ << endl;
        file << "ID Nasabah        : " << temp->id << endl;
        file << "NIK               : " << temp->nik << endl;
        file << "Nama              : " << temp->nama << endl;
        file << "Tanggal Lahir     : " << temp->tglLahir << endl;
        file << "Jenis Kelamin     : " << temp->jk << endl;
        file << "Alamat            : " << temp->alamat << endl;
        file << "Pekerjaan         : " << temp->pekerjaan << endl;
        file << "No. Telp          : " << temp->telp << endl;
        file << "No. Rekening      : " << temp->noRek << endl;
        file << "Saldo             : " << (long long)temp->saldo << endl; 
        file << "========================================" << endl << endl;
        
        temp = temp->next;
    }
    file.close();
}

void Admin::lihatNasabah() {
    if (head == nullptr) {
        cout << "Belum ada nasabah.\n";
        return;
    }

    cout << "\n***************************************************************************************************************\n";
    cout << "*****************************************        DAFTAR NASABAH       *****************************************\n";
    cout << "***************************************************************************************************************\n";
    cout << "+------+----------+---------------------------+----------------------+-------------------+--------------------+\n";
    cout << "|  No  | ID       | Nama                      | Alamat               | Telp              | Saldo              |\n";
    cout << "+----+----------+-----------------------------+----------------------+-------------------+--------------------+\n";
    NodeNasabah* temp = head;
    int no = 1;

    while (temp != nullptr) {
        cout << "| "  << setw(4)  << no++ 
             << " | " << setw(8) << temp->id 
             << " | " << setw(25) << temp->nama 
             << " | " << setw(20) << temp->alamat 
             << " | " << setw(17) << temp->telp 
             << " | " << setw(18) << (long long)temp->saldo << " |\n";
        
        temp = temp->next;
    }

    cout << "+----+----------+-----------------------------+----------------------+-------------------+--------------------+\n";
}

//Singly Linked List - Hapus Node Nasabah
void Admin::hapusNasabah() {
    if (head == nullptr) {
        cout << "Belum ada nasabah.\n";
        return;
    }

    int id;
    cout << "\nMasukkan ID nasabah yang ingin dihapus: ";
    cin >> id;

    // Validasi input jika cin gagal (misal input bukan angka)
    if (cin.fail()) {
        cin.clear(); // bersihkan error flag
        cin.ignore(10000, '\n'); // buang input yang salah
        cout << "Input tidak valid! Harap masukkan angka.\n";
        return;
    }

    NodeNasabah* temp = head;
    NodeNasabah* prev = nullptr;

    // Cari node dengan ID yang sesuai
    while (temp != nullptr && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    // Jika tidak ditemukan
    if (temp == nullptr) {
        cout << "Nasabah dengan ID " << id << " tidak ditemukan!\n";
        return;
    }

    // Hapus node
    if (prev == nullptr) {
        // Node yang dihapus adalah head
        head = head->next;
    } else {
        // Node di tengah atau akhir
        prev->next = temp->next;
    }

    cout << "Nasabah dengan ID " << id << " berhasil dihapus!\n";
    catatLog("Admin menghapus nasabah ID: " + to_string(id));

    delete temp;
}


void Admin::tampilNasabah(NodeNasabah* temp) {
    if (temp == nullptr) return;

    cout << "\n===================================================================================================================================================================================================" << endl;
    cout << "=================================================================                           D A T A   N A S A B A H                              ==================================================" << endl;
    cout << "===================================================================================================================================================================================================" << endl;

    cout << "+----+------------+--------------------------------+-----------------------+---------------------+-------------+-------------------+-----------------------+----------------+----------------------+\n";
    cout << "| No | ID         | Nama                           |          NIK          |        No Rek       |       JK    |      Tgl Lahir    |         Alamat        |      Telp      |        Saldo         |\n";
    cout << "+----+------------+--------------------------------+-----------------------+---------------------+-------------+-------------------+-----------------------+----------------+----------------------+\n";

    cout << left
         << "| " << setw(4)  << 1
         << "| " << setw(10) << temp->id
         << "| " << setw(31) << temp->nama
         << "| " << setw(22) << temp->nik
         << "| " << setw(20) << temp->noRek
         << "| " << setw(12) << temp->jk
         << "| " << setw(18) << temp->tglLahir
         << "| " << setw(22) << temp->alamat
         << "| " << setw(15) << temp->telp
         << "| " << setw(20) << fixed << setprecision(2) << temp->saldo
         << " |\n";

    cout << "+----+------------+--------------------------------+-----------------------+---------------------+-------------+-------------------+-----------------------+----------------+----------------------+\n";
}


void Admin::cariByID() {
    int idCari;
    cout << "Masukkan ID Nasabah: ";
    cin >> idCari;
    cin.ignore();

    NodeNasabah* ptr = head;
    bool found = false;

    while (ptr != nullptr) {
        if (ptr->id == idCari) {
            tampilNasabah(ptr);
            found = true;
            break;
        }
        ptr = ptr->next;
    }

    if (!found)
        cout << "Nasabah tidak ditemukan.\n";
}

void Admin::cariByNama() {
    string namaCari;
    cout << "Masukkan Nama Nasabah : ";
    cin.ignore();
    getline(cin, namaCari);

    NodeNasabah* ptr = head;
    bool found = false;

    while (ptr != nullptr) {
        // cari sebagian nama (kata kunci)
        if (ptr->nama.find(namaCari) != string::npos) {
            tampilNasabah(ptr);
            found = true;
        }
        ptr = ptr->next;
    }

    if (!found)
        cout << "Nasabah tidak ditemukan.\n";
}


void Admin::cariByNIK() {
    string nikCari;
    cout << "Masukkan NIK Nasabah : ";
    cin.ignore();
    getline(cin, nikCari);

    NodeNasabah* ptr = head;
    bool found = false;

    while(ptr != nullptr) {
        if(ptr->nik == nikCari) {
            tampilNasabah(ptr);
            found = true;
        }
        ptr = ptr->next;
    }
    if(!found) cout << "Nasabah tidak ditemukan.\n";
}

void Admin::cariByNoRek() {
    string rekCari;
    cout << "Masukkan Nomor Rekening: ";
    cin.ignore();
    getline(cin, rekCari);

    NodeNasabah* ptr = head;
    bool found = false;

    while (ptr != nullptr) {
        if (ptr->noRek == rekCari) {
            tampilNasabah(ptr);
            found = true;
        }
        ptr = ptr->next;
    }

    if (!found)
        cout << "Nasabah tidak ditemukan.\n";
}

void Admin::cariNasabah() {
    if (head == nullptr) {
        cout << "Belum ada nasabah.\n";
        return;
    }

    int pilihan;
    do {
        cout << "\n*****************************************************\n";
        cout << "**                                                 **\n";
        cout << "**        M E N U   P E N C A R I A N              **\n";
        cout << "**              N A S A B A H                      **\n";
        cout << "**                                                 **\n";
        cout << "*****************************************************\n";
        cout << "**-------------------------------------------------**\n";
        cout << "** [1]. Cari berdasarkan ID Nasabah               **\n";
        cout << "** [2]. Cari berdasarkan Nama Nasabah             **\n";
        cout << "** [3]. Cari berdasarkan NIK Nasabah              **\n";
        cout << "** [4]. Cari berdasarkan Nomor Rekening           **\n";
        cout << "** [5]. Kembali ke Menu Utama                     **\n";
        cout << "*****************************************************\n";
        
        cout << "Pilihan : ";
        cin >> pilihan;

        if (pilihan < 1 || pilihan > 5) {
            cout << "Pilihan tidak valid! Masukkan angka 1 - 5.\n";
            continue;
        }

        switch (pilihan) {
            case 1: 
                cariByID();    
                break;
            case 2: 
                cariByNama();  
                break;
            case 3: 
                cariByNIK();   
                break;
            case 4: 
                cariByNoRek(); 
                break;
        }
       
    } while (pilihan != 5);
}

void Admin::laporanTotalNasabah() {
    int total = hitungNasabah();

    cout << "\n=====================================================\n";
    cout << "                L A P O R A N  N A S A B A H        \n";
    cout << "=====================================================\n";
    cout << "Total Nasabah : " << total << endl;
    cout << "=====================================================\n";
}


void Admin::statistikSaldo() {
    if (head == nullptr) {
        cout << "\nBelum ada data nasabah.\n";
        return;
    }

    double total = 0;
    double tertinggi;
    double terendah;

    NodeNasabah* temp = head;

    tertinggi = terendah = temp->saldo;

    while (temp != nullptr) {
        total += temp->saldo;

        if (temp->saldo > tertinggi)
            tertinggi = temp->saldo;

        if (temp->saldo < terendah)
            terendah = temp->saldo;

        temp = temp->next;
    }

    double rata = total / hitungNasabah();

    cout << "\n=====================================================\n";
    cout << "=============  LAPORAN SALDO NASABAH  ===============\n";
    cout << "=====================================================\n";
    cout << fixed << setprecision(0);
    cout << "Total Nasabah   : " << hitungNasabah() << endl;
    cout << "Total Saldo     : Rp" << total << endl;
    cout << "Rata-rata Saldo : Rp" << rata << endl;
    cout << "Saldo Tertinggi : Rp" << tertinggi << endl;
    cout << "Saldo Terendah  : Rp" << terendah << endl;
    cout << "=====================================================\n";
}

void Admin::backupSaldo() {
    if (head == nullptr)
        return;

    ofstream file("backup_saldo.txt", ios::out);

    cout << "=====================================================\n";
    cout << "============= DATA BACKUP SALDO NASABAH =============\n";
    cout << "=====================================================\n";

    NodeNasabah* temp = head;
    int i = 1;

    while (temp != nullptr) {
        cout << setw(3) << i << ". "
             << left << setw(35) << temp->nama   // <- lebarnya diperbesar
             << " | Saldo: " << fixed << setprecision(0) << temp->saldo << endl;

        file << setw(3) << i << ". "
             << left << setw(35) << temp->nama
             << " | Saldo: " << fixed << setprecision(0) << temp->saldo << "\n";

        temp = temp->next;
        i++;
    }

    cout << "==============================================================\n";
    cout << "Backup selesai!\n";

    file.close();
}

void Admin::tambahAntrian(string nama) {
    Antrian* baru = new Antrian{ nomorAntrian++, nama, nullptr };

    if (!front) {
        front = rear = baru;
        baru->next = front;
    } else {
        rear->next = baru;
        rear = baru;
        rear->next = front;
    }

    cout << "Antrian untuk " << nama << " berhasil ditambahkan.\n";
}

void Admin::layaniAntrian() {
    if (!front)
        return;

    cout << "Sedang melayani: " << front->nama << endl;

    if (front == rear) {
        delete front;
        front = rear = nullptr;
    } else {
        Antrian* temp = front;
        front = front->next;
        rear->next = front;
        delete temp;
    }
}

void Admin::tampilAntrian() {
    if (!front)
        return;

    Antrian* temp = front;

    cout << "\n=== Daftar Antrian ===\n";
    do {
        cout << "No: " << temp->no
             << " | Nama: " << temp->nama << endl;
        temp = temp->next;
    } while (temp != front);
}

// ==================================
// BST (Binary Search Tree) REKENING
// ==================================
NodeNasabah::RekeningNode* Admin::insertRekening(
    NodeNasabah::RekeningNode* root,
    string norek,
    double saldo
) {
    if (!root)
        return new NodeNasabah::RekeningNode{ norek, saldo, nullptr, nullptr };

    if (norek < root->norek)
        root->left = insertRekening(root->left, norek, saldo);
    else
        root->right = insertRekening(root->right, norek, saldo);

    return root;
}

NodeNasabah::RekeningNode* Admin::cariRekening(
    NodeNasabah::RekeningNode* root,
    string norek
) {
    if (!root || root->norek == norek)
        return root;

    if (norek < root->norek)
        return cariRekening(root->left, norek);

    return cariRekening(root->right, norek);
}

void Admin::tampilInOrder(NodeNasabah::RekeningNode* root) {
    if (!root)
        return;

    tampilInOrder(root->left);

    cout << "Rekening: " << root->norek
         << " | Saldo: " << root->saldo << endl;

    tampilInOrder(root->right);
}


void Admin::tambahKeArrayRekening(NodeNasabah* n, string norek) {
    if (n->jumlahRek == n->kapasitasRek) {
        int newCap = n->kapasitasRek * 2;
        string* baru = new string[newCap];

        for (int i = 0; i < n->jumlahRek; i++)
            baru[i] = n->daftarRek[i];

        delete[] n->daftarRek;
        n->daftarRek = baru;
        n->kapasitasRek = newCap;
    }

    n->daftarRek[n->jumlahRek++] = norek;
}

void Admin::tambahRekeningNasabah() {
    int id;
    cout << "\nMasukkan ID Nasabah: ";
    cin >> id;
    cin.ignore(); 
    // Membersihkan newline agar getline berikutnya tidak terlewat

    // Cari nasabah berdasarkan ID
    NodeNasabah* n = getNasabahById(id);
    if (!n) {
        cout << "Nasabah tidak ditemukan.\n";
        return; 
        // Hentikan proses jika nasabah tidak ada
    }

    string norek;
    cout << "Masukkan nomor rekening baru: ";
    getline(cin, norek); 
    // Input nomor rekening baru

    double saldoAwal;
    cout << "Saldo awal rekening: ";
    cin >> saldoAwal; 
    // Input saldo awal rekening

    // Masukkan rekening ke dalam BST rekening nasabah
    n->rootRekening = insertRekening(n->rootRekening, norek, saldoAwal);

    // Simpan nomor rekening ke array rekening nasabah
    tambahKeArrayRekening(n, norek);

    cout << "Rekening berhasil ditambahkan.\n";
}

void Admin::transferRekening() {
    int id;
    cout << "\nMasukkan ID Nasabah: ";
    cin >> id;
    cin.ignore(); 

    // Ambil data nasabah
    NodeNasabah* n = getNasabahById(id);
    if (!n) {
        cout << "Nasabah tidak ditemukan.\n";
        return;
    }

    // Input rekening sumber
    string asal, tujuan;
    cout << "Rekening asal   : ";
    getline(cin, asal); 
    
    // Input rekening tujuan
    cout << "Rekening tujuan : ";
    getline(cin, tujuan); 

        // Input jumlah transfer
    double nominal;
    cout << "Jumlah transfer : ";
    cin >> nominal; 

    // Cari rekening asal dan tujuan di BST
    auto* rekAsal   = cariRekening(n->rootRekening, asal);
    auto* rekTujuan = cariRekening(n->rootRekening, tujuan);

    // Validasi rekening asal
    if (!rekAsal) {
        cout << "Rekening asal tidak ditemukan.\n";
        return;
    }

    // Validasi rekening tujuan
    if (!rekTujuan) {
        cout << "Rekening tujuan tidak ditemukan.\n";
        return;
    }

    // Validasi saldo mencukupi
    if (rekAsal->saldo < nominal) {
        cout << "Saldo tidak mencukupi.\n";
        return;
    }

    // Proses transfer saldo
    rekAsal->saldo   -= nominal;
    rekTujuan->saldo += nominal;

    cout << "Transfer berhasil!\n";
}

void Admin::menu() {
    int pilihan;

    do {
        cout << "\n*****************************************************" << endl;
        cout << "************************** **************************" << endl;
        cout << "***********************       ***********************" << endl;
        cout << "********************             ********************" << endl;
        cout << "*****************                   *****************" << endl;
        cout << "**************                         **************" << endl;
        cout << "***********                               ***********" << endl;
        cout << "********                                     ********" << endl;
		cout << "*****                                           *****" << endl;
        cout << "*****************************************************" << endl;
        cout << "**                                                 **" << endl;
        cout << "**            S E L A M A T   D A T A N G          **" << endl;
        cout << "**                     A D M I N                   **" << endl;
        cout << "**                                                 **" << endl;
        cout << "*****************************************************" << endl;
        cout << "*****************************************************" << endl;
        cout << "||  [1]  Lihat Daftar Nasabah                      ||" << endl;
        cout << "||  [2]  Tambah Nasabah                            ||" << endl;
        cout << "||  [3]  Hapus Nasabah                             ||" << endl;
        cout << "||  [4]  Cari Nasabah                              ||" << endl;
        cout << "||  [5]  Laporan Total Nasabah                     ||" << endl;
        cout << "||  [6]  Statistik Saldo Nasabah                   ||" << endl;
        cout << "||  [7]  Backup Saldo                              ||" << endl;
        cout << "||  [8]  Lihat Log Aktivitas Admin                 ||" << endl;
        cout << "||  [9]  Tambah Antrian Nasabah                    ||" << endl;
        cout << "||  [10] Tampilkan Antrian Nasabah                 ||" << endl;
        cout << "||  [11] Layani Antrian Nasabah                    ||" << endl;
        cout << "||  [12] Antrian Pembukaan Rekening                ||" << endl;
        cout << "||  [13] Layani Pembukaan Rekening                 ||" << endl;
        cout << "||  [14] Tampilkan Antrian Pembukaan Rekening      ||" << endl;
        cout << "||  [15] Tambah Rekening Nasabah                   ||" << endl;
        cout << "||  [16] Lihat Semua Rekening (BST)                ||" << endl;
        cout << "||  [17] Transfer Antar Rekening (Tree)            ||" << endl;
        cout << "||  [18] Kembali ke Menu Utama                     ||" << endl;
        cout << "*****************************************************" << endl;
        cout << "*****************************************************" << endl;
        cout << "Masukkan pilihan Anda : ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                lihatNasabah();
                break;

            case 2:
                tambahNasabah();
                break;

            case 3:
                hapusNasabah();
                break;

            case 4:
                cariNasabah();
                break;

            case 5:
                laporanTotalNasabah();
                break;

            case 6:
                statistikSaldo();
                break;

            case 7:
                backupSaldo();
                break;

            case 8:
                tampilLog();
                break;

            case 9: {
                string nm;
                cout << "Nama: ";
                cin.ignore();
                getline(cin, nm);
                tambahAntrian(nm);
                break;
            }

            case 10:
                tampilAntrian();
                break;

            case 11:
                layaniAntrian();
                break;

            case 12: {
                string nm;
                cout << "Nama: ";
                cin.ignore();
                getline(cin, nm);
                enqueueQ(nm, rearQ + 2);
                break;
            }

            case 13:
                dequeueQ();
                break;

            case 14:
                tampilQueueQ();
                break;

            case 15:
                tambahRekeningNasabah();
                break;

            case 16: {
                int i;
                cout << "ID: ";
                cin >> i;
                auto* n = getNasabahById(i);
                if (n)
                    tampilInOrder(n->rootRekening);
                break;
            }

            case 17:
                transferRekening();
                break;
        }

        if (pilihan != 18) {
            cout << "\nTekan Enter...";
            cin.ignore();
            cin.get();
        }

    } while (pilihan != 18);
}

// =========================================================
// 4. DEFINISI CLASS NASABAH
// =========================================================

class Nasabah { 
private:
    int idLogin = -1;
    Transaksi* headTransaksi = nullptr;

public:
    void daftarNasabahBaru(Admin& admin);
    bool loginNasabah(Admin& admin);

    void bukaRekeningBaru(NodeNasabah& n);

    void resizeStack();
    void tambahTransaksi(string j, double jm);
    void tambahRiwayat(NodeNasabah* n, string isi);

    void tampilRiwayatTransaksi();

    void pushTransaksi(string j, double jm);
    bool popTransaksi(string& j, double& jm);

    void setorUang(NodeNasabah* p, Admin& admin);
    void tarikUang(NodeNasabah* p, Admin& admin);

    void undoTransaksi(Admin& admin);
    void lihatTransaksiTerakhir();

    void menuNasabah(Admin& admin);
    void nasabahBaruAtauLogin(Admin& admin);
};

void Nasabah::daftarNasabahBaru(Admin &admin) {
    cout << "\n*****************************************************" << endl;
    cout << "*           P E N D A F T A R A N   N A S A B A H     *" << endl;
    cout << "*                     B A R U                       *" << endl;
    cout << "*****************************************************" << endl;


    // Alokasi node nasabah baru
    NodeNasabah* n = new NodeNasabah;

    // Penentuan ID otomatis
    n->id = admin.getLastID() + 1;
    cout << "ID Nasabah Anda              : " << n->id << endl;

    cin.ignore(); // Membersihkan buffer input

    // Input data identitas nasabah
    cout << "Masukkan Nama                : ";
    getline(cin, n->nama);

    cout << "Masukkan NIK                 : ";
    getline(cin, n->nik);

    cout << "Masukkan Jenis Kelamin (L/P) : ";
    getline(cin, n->jk);

    cout << "Masukkan Tanggal Lahir       : ";
    getline(cin, n->tglLahir);

    cout << "Masukkan Alamat              : ";
    getline(cin, n->alamat);

    cout << "Masukkan No. Telepon         : ";
    getline(cin, n->telp);

    cout << "Masukkan Pekerjaan           : ";
    getline(cin, n->pekerjaan);

    // Inisialisasi data keuangan dan pointer
    n->saldo = 0;
    n->next = NULL;
    n->rootRekening = nullptr;

    // Inisialisasi daftar rekening
    n->kapasitasRek = 5;
    n->jumlahRek = 0;
    n->daftarRek = new string[n->kapasitasRek];

    // Inisialisasi riwayat transaksi
    n->kapasitas = 100;
    n->jmlRiwayat = 0;
    n->riwayat = new string[n->kapasitas];

    // Menambahkan nasabah ke dalam sistem admin
    admin.tambahNodeNasabah(n);

    cout << "\nPendaftaran berhasil!\n";

    // Otomatis membuka rekening baru
    bukaRekeningBaru(*n);
}

bool Nasabah::loginNasabah(Admin &admin) {
    cout << "\n=====================================================" << endl;
    cout << "                  L O G I N   N A S A B A H           " << endl;
    cout << "=====================================================" << endl;

    cout << "Masukkan ID Nasabah : ";
    cin >> idLogin;

    NodeNasabah* temp = admin.getNasabahById(idLogin);

    if (temp != nullptr) {
        cout << "\nLogin berhasil!" << endl;
        cout << "Selamat datang, " << temp->nama << endl;
        return true;
    } else {
        cout << "\nID Nasabah tidak ditemukan!" << endl;
        idLogin = -1;
        return false;
    }
}

void Nasabah::bukaRekeningBaru(NodeNasabah &n) {
    cout << "\n+=====================================================+\n";
    cout << "|        P E M B U K A A N   R E K E N I N G            |\n";
    cout << "+=====================================================+\n";
    cout << "| [1]. Tabungan Reguler   (Min Rp50.000)                |\n";
    cout << "| [2]. Tabungan Bisnis    (Min Rp100.000)               |\n";
    cout << "| [3]. Deposito           (Min Rp1.000.000)             |\n";
    cout << "| [4]. Giro               (Min Rp500.000)               |\n";
    cout << "+-----------------------------------------------------+\n";
    cout << "Pilihan Anda : ";

    int p;
    cin >> p;

    double min = 0;
    if (p == 1) min = 50000;
    else if (p == 2) min = 100000;
    else if (p == 3) min = 1000000;
    else if (p == 4) min = 500000;

    double setoran;
    do {
        cout << "Setoran awal (Min Rp" << min << ") : ";
        cin >> setoran;
    } while (setoran < min);

    n.saldo = setoran;

    cout << "\nRekening berhasil dibuka!" << endl;
}

// =========================================================
// Fungsi untuk memperbesar kapasitas stack transaksi
// =========================================================
void Nasabah::resizeStack() {
    // Menambah kapasitas stack menjadi 2x lipat
    int capBaru = kapasitasStack * 2;

    // Alokasi array baru
    string* jB  = new string[capBaru];
    double* jmB = new double[capBaru];

    // Salin data lama ke array baru
    for (int i = 0; i <= topStack; i++) {
        jB[i]  = jenisTrans[i];
        jmB[i] = jumlahTrans[i];
    }

    // Hapus array lama
    delete[] jenisTrans;
    delete[] jumlahTrans;

    // Arahkan ke array baru
    jenisTrans    = jB;
    jumlahTrans   = jmB;
    kapasitasStack = capBaru;
}

// =========================================================
// Fungsi untuk menambah transaksi ke dalam linked list
// =========================================================

//Circular Linked List - Tambah Transaksi
void Nasabah::tambahTransaksi(string j, double jm) {
    // Membuat node transaksi baru
    Transaksi* baru = new Transaksi;
    baru->jenis  = j;
    baru->jumlah = jm;
    baru->waktu  = waktuSekarang();
    baru->next   = nullptr;

    // Jika list masih kosong
    if (!headTransaksi) {
        headTransaksi = baru;
        headTransaksi->next = headTransaksi; // Circular linked list
    }
    // Jika sudah ada transaksi
    else {
        Transaksi* temp = headTransaksi;

        // Cari node terakhir
        while (temp->next != headTransaksi) {
            temp = temp->next;
        }

        // Tambahkan node baru di akhir
        temp->next = baru;
        baru->next = headTransaksi;
    }
}

// ======================================================================
// Fungsi untuk menambah riwayat transaksi nasabah ke array dinamis
// ======================================================================
void Nasabah::tambahRiwayat(NodeNasabah* n, string isi) {
    // Jika kapasitas riwayat penuh, perbesar array
    if (n->jmlRiwayat == n->kapasitas) {
        int capBaru = n->kapasitas * 2;
        string* baru = new string[capBaru];

        // Salin data lama
        for (int i = 0; i < n->jmlRiwayat; i++) {
            baru[i] = n->riwayat[i];
        }

        // Hapus array lama dan ganti dengan yang baru
        delete[] n->riwayat;
        n->riwayat   = baru;
        n->kapasitas = capBaru;
    }

    // Tambahkan riwayat baru
    n->riwayat[n->jmlRiwayat++] = isi;
}

// =========================================================
// Fungsi untuk menampilkan riwayat transaksi nasabah
// =========================================================
void Nasabah::tampilRiwayatTransaksi() {
    // Jika belum ada transaksi
    if (!headTransaksi) {
        cout << "\nBelum ada transaksi.\n";
        return;
    }

    Transaksi* t = headTransaksi;

    cout << "\n=====================================\n";
    cout << "        RIWAYAT TRANSAKSI NASABAH     \n";
    cout << "=====================================\n";

    // Traversal circular linked list
    do {
        cout << "[" << t->waktu << "] "
             << t->jenis
             << " - Rp" << t->jumlah << endl;
        t = t->next;
    } while (t != headTransaksi);
}

// =========================================================
// Fungsi untuk STACK transaksi
// =========================================================

// STACK - PUSH TRANSAKSI
void Nasabah::pushTransaksi(string j, double jm) {
    // Jika stack penuh, perbesar kapasitas
    if (topStack + 1 == kapasitasStack) {
        resizeStack();
    }

    // Tambah elemen ke stack
    topStack++;
    jenisTrans[topStack]  = j;
    jumlahTrans[topStack] = jm;
}

// STACK - POP (UNDO)
bool Nasabah::popTransaksi(string &j, double &jm) {
    // Jika stack kosong
    if (topStack == -1) {
        return false;
    }

    // Ambil data transaksi terakhir
    j  = jenisTrans[topStack];
    jm = jumlahTrans[topStack];

    // Kurangi indeks stack
    topStack--;
    return true;
}

// =========================================================
// Definisi fungsi setor, tarik, undo, lihat terakhir
// =========================================================

// Fungsi untuk setor uang
void Nasabah::setorUang(NodeNasabah* p, Admin &admin) {
    if (p == nullptr) {
        cout << "Kesalahan: Pointer nasabah kosong!" << endl;
        return;
    }

    double jm;
    cout << "\n--- Setor Tunai ---" << endl;
    cout << "Jumlah setor: ";
    
    // Validasi apakah input benar-benar angka
    if (!(cin >> jm)) {
        cout << "Input harus angka!" << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    if (jm <= 0) {
        cout << "Jumlah setor harus lebih dari 0!" << endl;
        return;
    }

    // Update Saldo
    p->saldo += jm;

    // Bagian ini rawan crash, kita beri penanda (flag)
    cout << "Sedang menyimpan data..." << endl; 
    admin.updateFile();
    
    tambahRiwayat(p, "Setor " + to_string((int)jm));
    pushTransaksi("Setor", jm);
    tambahTransaksi("Setor", jm);

    cout << "Berhasil! Saldo sekarang: " << p->saldo << endl;
    
    // Tambahkan ini agar program tidak langsung balik ke menu/keluar
    cout << "\nTekan Enter untuk kembali...";
    cin.ignore(1000, '\n');
    cin.get(); 
}

// Fungsi untuk tarik uang
void Nasabah::tarikUang(NodeNasabah* p, Admin &admin) {
    double jm;
    cout << "\n--- Penarikan Uang ---" << endl;
    cout << "Saldo saat ini: " << p->saldo << endl;
    cout << "Jumlah tarik: ";
    
    // Mengecek apakah input adalah angka yang valid
    if (!(cin >> jm)) {
        cout << "Error: Input harus berupa angka!" << endl;
        cin.clear(); // membersihkan status error
        cin.ignore(1000, '\n'); // membuang sisa input di buffer
        return;
    }

    // Validasi jumlah tarik dengan pesan feedback
    if (jm <= 0) {
        cout << "Gagal: Jumlah penarikan tidak boleh nol atau negatif." << endl;
        return;
    } 
    
    if (jm > p->saldo) {
        cout << "Gagal: Saldo tidak mencukupi. Sisa saldo Anda: " << p->saldo << endl;
        return;
    }

    // Proses Pengurangan Saldo
    p->saldo -= jm;
    admin.updateFile(); // Simpan perubahan ke database/file

    // Simpan ke riwayat & stack undo
    tambahRiwayat(p, "Tarik " + to_string(jm));
    pushTransaksi("Tarik", jm);
    tambahTransaksi("Tarik", jm);

    cout << "Berhasil! Penarikan sebesar " << jm << " sukses." << endl;
    cout << "Sisa Saldo: " << p->saldo << endl;
}

// Fungsi untuk undo transaksi terakhir
void Nasabah::undoTransaksi(Admin &admin) {
    string j;
    double jm;

    // Ambil transaksi terakhir dari stack
    if (!popTransaksi(j, jm)) return;

    NodeNasabah* n = admin.getNasabahById(idLogin);
    if (!n) return;

    // Kembalikan saldo sesuai jenis transaksi
    if (j == "Setor") {
        n->saldo -= jm;
    } else if (j == "Tarik") {
        n->saldo += jm;
    }

    cout << "Undo berhasil! Saldo: " << n->saldo << endl;
}


// Fungsi untuk melihat transaksi terakhir
void Nasabah::lihatTransaksiTerakhir() {
    // Jika stack kosong, tidak ada transaksi
    if (topStack == -1) {
        cout << "Belum ada transaksi.\n";
        return;
    }

    // Tampilkan transaksi terakhir (top of stack)
    cout << "Transaksi Terakhir : "
         << jenisTrans[topStack]
         << " Rp"
         << jumlahTrans[topStack]
         << endl;
}

// =========================================================
// Definisi menu nasabah dan login/daftar
// =========================================================

void Nasabah::menuNasabah(Admin &admin) {
    NodeNasabah* p = admin.getNasabahById(idLogin);
    if (!p) return;

    int pilih;
    do {
    cout << "\n*****************************************************" << endl;
    cout << "************************** **************************" << endl;
    cout << "***********************       ***********************" << endl;
    cout << "********************             ********************" << endl;
    cout << "*****************                   *****************" << endl;
    cout << "**************                         **************" << endl;
    cout << "***********                               ***********" << endl;
    cout << "********                                     ********" << endl;
    cout << "*****                                           *****" << endl;
    cout << "*****************************************************\n";
    cout << "**            M E N U   N A S A B A H              **\n";
    cout << "*****************************************************\n";
    cout << "** [1] Data Diri                                   **\n";
    cout << "** [2] Buka Rekening                               **\n";
    cout << "** [3] Cek Saldo                                   **\n";
    cout << "** [4] Setor                                       **\n";
    cout << "** [5] Tarik                                       **\n";
    cout << "** [6] Riwayat                                     **\n";
    cout << "** [7] Undo                                        **\n";
    cout << "** [8] Transaksi Terakhir                          **\n";
    cout << "** [9] Keluar                                      **\n";
    cout << "*****************************************************\n";
    
    cout << "Pilih : ";
    cin >> pilih;

        switch (pilih) {
            case 1:
                cout << "Nama  : " << p->nama << endl;
                cout << "Saldo : Rp" << fixed << setprecision(0) << p->saldo << endl;
                break;

            case 2:
                bukaRekeningBaru(*p);
                break;

            case 3:
                cout << "Saldo : Rp" << fixed << setprecision(0) << p->saldo << endl;
                break;

            case 4:
                setorUang(p, admin);
                break;

            case 5:
                tarikUang(p, admin);
                break;

            case 6:
                tampilRiwayatTransaksi();
                break;

            case 7:
                undoTransaksi(admin);
                break;

            case 8:
                lihatTransaksiTerakhir();
                break;
        }

    } while (pilih != 9);
}

// =========================================================
// Menu untuk nasabah baru atau login
// =========================================================
void Nasabah::nasabahBaruAtauLogin(Admin &admin) {
    int p;
    do {
       	cout << "\n*****************************************************" << endl;
       	cout << "************************** **************************" << endl;
       	cout << "***********************       ***********************" << endl;
       	cout << "********************             ********************" << endl;
    	cout << "*****************                   *****************" << endl;
    	cout << "**************                         **************" << endl;
    	cout << "***********                               ***********" << endl;
    	cout << "********                                     ********" << endl;
		cout << "*****                                           *****" << endl;
		cout << "**`                                                **" << endl;           
        cout << "*****************************************************" << endl;   
        cout << "**              M E N U   N A S A B A H            **" << endl;   
        cout << "*****************************************************" << endl;   
        cout << "** [1] Daftar                                      **" << endl;
        cout << "** [2] Login                                       **" << endl;
        cout << "** [3] Kembali                                     **" << endl;
        cout << "*****************************************************" << endl;
        cout << "Masukkan Pilihan Anda : ";
        cin >> p;

        if (p == 1) {
            daftarNasabahBaru(admin);
        } else if (p == 2) {
            if (loginNasabah(admin)) {
                menuNasabah(admin);
            }
        }

    } while (p != 3);
}

// =========================================================
// 5. DEFINISI CLASS BANK MINI
// ==========================================================

class BankMini {
public:
    void mainMenu(Admin &admin) {
        int pilihan;
        Nasabah nasabah;

        do {
        	cout << "*****************************************************" << endl;
        	cout << "************************** **************************" << endl;
        	cout << "***********************       ***********************" << endl;
        	cout << "********************             ********************" << endl;
        	cout << "*****************                   *****************" << endl;
        	cout << "**************                         **************" << endl;
        	cout << "***********                               ***********" << endl;
        	cout << "********                                     ********" << endl;
			cout << "*****                                           *****" << endl;
		    cout << "**                                                 **" << endl;
		    cout << "**       S E L A M A T   D A T A N G   D I         **" << endl;
		    cout << "**            S I S T E M   B A N K   M I N I      **" << endl;
		    cout << "**                                                 **" << endl;
		    cout << "**-------------------------------------------------**" << endl;
		    cout << "**              Masuk ke Sistem sebagai:           **" << endl;
		    cout << "**-------------------------------------------------**" << endl;
		    cout << "**  [1]. Admin                                     **" << endl;
		    cout << "**  [2]. Nasabah                                   **" << endl;
		    cout << "**  [3]. Keluar                                    **" << endl;
			cout << "*****************************************************" << endl;
			cout << "*****************************************************" << endl;
            
            cout << "Pilihan : ";
            cin >> pilihan;

            switch (pilihan) {
                case 1:
                    if (admin.login()) {
                        admin.menu();
                    }
                    break;

                case 2:
                    nasabah.nasabahBaruAtauLogin(admin);
                    break;

                case 3:
                    cout << "\nTerima kasih!\n";
                    break;
            }

        } while (pilihan != 3);
    }
};


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

