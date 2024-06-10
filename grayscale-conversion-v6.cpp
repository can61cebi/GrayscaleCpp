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

    const int genislik = 200;
    const int yukseklik = 200;
    const int satir = ((genislik * 3) + 3) & (~3);
    unsigned char* veriler = (unsigned char*) malloc(satir);

    if (veriler == nullptr) {
        cerr << "Bellek ayrilamadi" << endl;
        return false;
    }

    for (int i = 0; i < yukseklik; ++i) {
        dosya.read((char*)veriler, satir);

        for (int j = 0; j < genislik; ++j) {
            const int indis = j * 3;
            const unsigned char r = veriler[indis + 2];
            const unsigned char g = veriler[indis + 1];
            const unsigned char b = veriler[indis];
            const unsigned char gray = (unsigned char)(0.299f * r + 0.587f * g + 0.114f * b);

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

void islemYap(const int basla, const int bitis) {
    for (int dosyaNumarasi = basla; dosyaNumarasi <= bitis; ++dosyaNumarasi) {
        stringstream ss;
        ss << "/mnt/ramdisk/images/image" << setw(4) << setfill('0') << dosyaNumarasi << ".bmp";
        const string dosyaAdi = ss.str();

        if (!OverwriteGrayscaleFonksiyonu(dosyaAdi)) {
            cerr << "Resim griye donusturulemedi." << endl;
            continue;
        }
    }
}

int main() {
    const int dosyaSayisi = 90000;
    const int threadSayisi = 12;
    const int dosyaPerThread = dosyaSayisi / threadSayisi;

    vector<thread> threadler;
    for (int i = 0; i < threadSayisi; ++i) {
        const int basla = i * dosyaPerThread + 1;
        const int bitis = (i == threadSayisi - 1) ? dosyaSayisi : (i + 1) * dosyaPerThread;
        threadler.push_back(thread(islemYap, basla, bitis));
    }

    for (auto &t : threadler) {
        t.join();
    }

    return 0;
}
