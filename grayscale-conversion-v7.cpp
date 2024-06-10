#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv;
queue<string> dosyaKuyrugu;
bool tumDosyalarEkledi = false;

bool OverwriteGrayscaleFonksiyonu(const string &dosyaAdi) {
    fstream dosya(dosyaAdi, ios::in | ios::out | ios::binary);
    if (!dosya.is_open()) {
        cerr << "Dosya acilamadi: " << dosyaAdi << endl;
        return false;
    }

    unsigned char baslik[54];
    dosya.read((char*)baslik, 54);
    dosya.seekp(54);

    const int genislik = 200;
    const int yukseklik = 200;
    const int satir = ((genislik * 3) + 3) & (~3);
    vector<unsigned char> veriler(satir);

    for (int i = 0; i < yukseklik; ++i) {
        dosya.read((char*)veriler.data(), satir);

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

        dosya.seekp(54 + i * satir, ios::beg);
        dosya.write((char*)veriler.data(), satir);
    }

    dosya.close();
    return true;
}

void islemYap() {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, []{ return !dosyaKuyrugu.empty() || tumDosyalarEkledi; });

        if (!dosyaKuyrugu.empty()) {
            string dosyaAdi = dosyaKuyrugu.front();
            dosyaKuyrugu.pop();
            lock.unlock();

            if (!OverwriteGrayscaleFonksiyonu(dosyaAdi)) {
                cerr << "Resim griye donusturulemedi: " << dosyaAdi << endl;
            }
        } else if (tumDosyalarEkledi) {
            break;
        }
    }
}

int main() {
    const int dosyaSayisi = 90000;
    const int threadSayisi = 11;

    vector<thread> threadler;
    for (int i = 0; i < threadSayisi; ++i) {
        threadler.push_back(thread(islemYap));
    }

    auto start = chrono::high_resolution_clock::now();
    for (int dosyaNumarasi = 1; dosyaNumarasi <= dosyaSayisi; ++dosyaNumarasi) {
        stringstream ss;
        ss << "/mnt/ramdisk/images/image" << setw(4) << setfill('0') << dosyaNumarasi << ".bmp";
        const string dosyaAdi = ss.str();

        {
            lock_guard<mutex> lock(mtx);
            dosyaKuyrugu.push(dosyaAdi);
        }
        cv.notify_one();
    }

    {
        lock_guard<mutex> lock(mtx);
        tumDosyalarEkledi = true;
    }
    cv.notify_all();

    for (auto &t : threadler) {
        t.join();
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Toplam süre: " << elapsed.count() << " saniye" << endl;

    return 0;
}
