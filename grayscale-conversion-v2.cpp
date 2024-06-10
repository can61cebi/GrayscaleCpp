#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

int main() {
    for (int dosyaNumarasi = 1; dosyaNumarasi < 4999; ++dosyaNumarasi) {
        stringstream ss;
        ss << "images/image" << setw(4) << setfill('0') << dosyaNumarasi << ".bmp";
        string dosyaAdi = ss.str();

        fstream dosya(dosyaAdi, ios::binary | ios::in | ios::out);
        if (!dosya.is_open()) {
            cerr << "Dosya acilamadi: " << dosyaAdi << endl;
            continue;
        }

        unsigned char baslik[54];
        dosya.read((char*)baslik, 54);

        int genislik = 1088;
        int yukseklik = 1088;
        int satir = ((genislik * 3) + 3) & (~3);
        unsigned char veriler[satir];
        unsigned char yeniVeriler[satir];

        dosya.seekp(54);

        for (int i = 0; i < yukseklik; ++i) {
            dosya.read((char*)veriler, satir);
            for (int j = 0; j < genislik; ++j) {
                int indis = j * 3;
                unsigned char r = veriler[indis + 2];
                unsigned char g = veriler[indis + 1];
                unsigned char b = veriler[indis];
                unsigned char gray = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);

                yeniVeriler[indis] = gray;
                yeniVeriler[indis + 1] = gray;
                yeniVeriler[indis + 2] = gray;
            }
            dosya.seekp(54 + i * satir);
            dosya.write((char*)yeniVeriler, satir);
        }

        dosya.close();
    }

    return 0;
}
