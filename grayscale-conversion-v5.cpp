#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

mutex mtx;

bool OverwriteGrayscaleFonksiyonu(const string &dosyaAdi) {
    fstream dosya(dosyaAdi, ios::in | ios::out | ios::binary);
    if (!dosya.is_open()) {
        cerr << "Dosya acilamadi: " << dosyaAdi << endl;
        return false;
    }

    unsigned char baslik[54];
    dosya.read((char*)baslik, 54);

    int genislik = 200;
    int yukseklik = 200;
    int satir = ((genislik * 3) + 3) & (~3);
    unsigned char* veriler = (unsigned char*) malloc(satir);

    if (veriler == nullptr) {
        cerr << "Bellek ayrilamadi" << endl;
        return false;
    }

    for (int i = 0; i < yukseklik; i++) {
        dosya.read((char*)veriler, satir);

        for (int j = 0; j < genislik; j++) {
            int indis = j * 3;
            unsigned char r = veriler[indis + 2];
            unsigned char g = veriler[indis + 1];
            unsigned char b = veriler[indis];
            unsigned char gray = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);

            veriler[indis] = gray;
            veriler[indis + 1] = gray;
            veriler[indis + 2] = gray;
        }

        mtx.lock();
        dosya.seekp(54 + i * satir, ios::beg);
        dosya.write((char*)veriler, satir);
        mtx.unlock();
    }

    free(veriler);
    dosya.close();
    return true;
}

void islemYap(int basla, int bitis) {
    for (int dosyaNumarasi = basla; dosyaNumarasi <= bitis; ++dosyaNumarasi) {
        stringstream ss;
        ss << "/mnt/ramdisk/images/image" << setw(4) << setfill('0') << dosyaNumarasi << ".bmp";
        string dosyaAdi = ss.str();

        if (!OverwriteGrayscaleFonksiyonu(dosyaAdi)) {
            cerr << "Resim griye donusturulemedi." << endl;
            continue;
        }
    }
}

int main() {
    vector<thread> threadler;
    int dosyaSayisi = 90000;
    int threadSayisi = 12;
    int dosyaPerThread = dosyaSayisi / threadSayisi;

    for (int i = 0; i < threadSayisi; ++i) {
        int basla = i * dosyaPerThread + 1;
        int bitis = (i == threadSayisi - 1) ? dosyaSayisi : (i + 1) * dosyaPerThread;
        threadler.push_back(thread(islemYap, basla, bitis));
    }

    for (auto &t : threadler) {
        t.join();
    }

    return 0;
}
