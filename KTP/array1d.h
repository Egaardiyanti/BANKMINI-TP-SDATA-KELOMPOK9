#ifndef ARRAY1D_H
#define ARRAY1D_H

#include<iostream>
#include "ktp.h"
using namespace std;

class Array1D{
	private:
		KTP* data;
		int kapasitas;
		int size;
	public:
		Array1D (){
			data=nullptr;
		}
		Array1D(int inisialKapasitas):kapasitas(inisialKapasitas), size(0){
			data= new KTP[kapasitas];
		}
		~Array1D(){
			delete[]data;
			cout<<"Memory array dibersihkan.\n";
		}
		bool add(const KTP& element){
			if (size <kapasitas){
				data[size]=element;
				size++;
				return true;
			}
			cout<<"Eror: Array penuh, tidak bisa menambahkan elemen baru.\n";
			return false;
		}
		
		KTP& get(int index){
			if(index >= 0 && index < size){
				return data[index];
			}
			cout<<"Error: Indeks di luar batas!.\n";
			return data[0];
		}
		
		int getSize() const{
			return size;
		}
		
void displayAll() const{
				 cout<< "\n=============== DATA KTP ARRAY 1D ================\n";
             for (int i=0;i< size;i++){        	
                 cout<< "================= Data KTP ke-" << i + 1 << " ==================\n";
                 cout<<"NIK\t\t\t: "<<data[i].nik<<endl;
                 cout<<"Nama\t\t\t: "<<data[i].nama<<endl;
                 cout<<"Tanggal Lahir\t\t: "<<data[i].tanggalLahir.hari << "-"
                      << data[i].tanggalLahir.bulan << "-"
                      << data[i].tanggalLahir.tahun<<endl;
                 
                 cout<<"Jenis Kelamin\t\t: ";
                 switch (data[i].jenisKelamin) {
                     case LAKI_LAKI: cout << "LAKI-LAKI"; break;
                     case PEREMPUAN: cout << "PEREMPUAN"; break;
                 }
                 cout << endl;
                 
                 cout<<"Alamat\t\t\t: "<<data[i].alamat<<endl;
                 
                 cout<<"Agama\t\t\t: ";
                 switch (data[i].agama) {
                     case ISLAM: cout << "ISLAM"; break;
                     case KRISTEN: cout << "KRISTEN"; break;
                     case KATOLIK: cout << "KATOLIK"; break;
                     case HINDU: cout << "HINDU"; break;
                     case BUDDHA: cout << "BUDDHA"; break;
                     case KHONGHUCU: cout << "KHONGHUCU"; break;
                 }
                 cout << endl;
                 
                 cout<<"Status Perkawinan\t: ";
                 switch (data[i].statusPerkawinan) {
                     case BELUM_KAWIN: cout << "BELUM KAWIN"; break;
                     case KAWIN: cout << "KAWIN"; break;
                 }
                 cout << endl;
                 
                 cout<<"Pekerjaan\t\t: "<<data[i].pekerjaan<<endl;
                 
                 cout<<"Kewarganegaraan\t\t: ";
                 switch (data[i].kewarganegaraan) {
                     case WNI: cout << "WNI"; break;
                     case WNA: cout << "WNA"; break;
                 }
                 cout << endl;
             }
             cout << "Berlaku Hingga\t\t: SEUMUR HIDUP\n";
             cout<<"==================================================\n";
         }
		
		
};

#endif