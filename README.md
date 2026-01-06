🏦 Bank Mini System (Project Struktur Data)

Bank Mini System adalah aplikasi simulasi sistem perbankan sederhana berbasis bahasa C++ yang dirancang untuk menerapkan berbagai konsep Struktur Data dalam satu program terintegrasi.
Proyek ini dikembangkan sebagai tugas proyek mata kuliah Struktur Data, dengan fokus pada pengelolaan data nasabah, rekening, transaksi, dan antrian layanan. Sistem ini memungkinkan admin dan nasabah untuk melakukan berbagai operasi seperti pendaftaran nasabah, pengelolaan rekening, transaksi perbankan, pencatatan log aktivitas, serta manajemen antrian layanan.

🎯 Tujuan Proyek
1. Mengimplementasikan berbagai struktur data dalam kasus nyata sistem perbankan
2. Memahami perbedaan penggunaan struktur data sesuai kebutuhan

Struktur Data yang Digunakan
Proyek ini menggunakan beberapa struktur data utama, yaitu:
1. Singly Linked List
Digunakan untuk menyimpan dan mengelola data nasabah secara dinamis.
2. Doubly Linked List
Digunakan untuk menyimpan log aktivitas admin, memungkinkan traversal maju dan mundur.
3. Circular Linked List
Digunakan untuk menyimpan riwayat transaksi nasabah secara berulang.
4. Queue (Array)
Digunakan untuk antrian pembukaan rekening dengan prinsip FIFO.
5. Queue (Circular Linked List)
Digunakan untuk antrian layanan nasabah yang bersifat dinamis.
6. Stack (Array Dinamis)
Digunakan untuk fitur undo atau pembatalan transaksi.
7. Binary Search Tree (BST)
Digunakan untuk pengelolaan rekening nasabah agar pencarian dan penambahan rekening lebih efisien.
8. File Handling
Digunakan untuk penyimpanan data nasabah, data KTP, dan backup saldo.

⚙️ Fitur Utama
1. Manajemen data nasabah (tambah, hapus, cari)
2. Pengelolaan rekening menggunakan Binary Search Tree
3. Transaksi setor, tarik, dan transfer
4. Riwayat transaksi berbasis Circular Linked List
5. Antrian layanan dan pembukaan rekening
6. Log aktivitas admin
7. Penyimpanan dan pemuatan data dari file

📂 Struktur File
1. main.cpp → fungsi main() untuk menjalankan program pertama kali dan menginisialisasi menu utama.
2. BankMini.h → Berisi logika menu (Admin & Nasabah) serta fungsi-fungsi utama perbankan.
3. Tree.h → Implementasi Binary Search Tree (BST) untuk menyimpan data rekening agar pencarian cepat.
4. Queue.h → Implementasi antrian (bisa Array atau Linked List) untuk nasabah di teller/CS.
5. Stack.h → Implementasi Stack untuk mencatat histori yang bisa dibatalkan (undo transaction).
6. KTP.h & DATA.h → Definisi struct atau class untuk identitas nasabah dan data penunjang lainnya.
7. Array.h → berisi fungsi bantuan untuk manipulasi array atau struktur data statis.
8. txt Files → Database Sederhana. Tempat penyimpanan data permanen agar saat CMD ditutup, data tidak hilang.

👨‍🎓 Kontributor
Anggota :
1. Devalya Lameysha Manurung (2400018213)
2. Ega Ardiyanti (2400018222)
3. Fitri Amelia Dwiyanti (2400018229)

Kelas : D
Mata Kuliah: Struktur Data
