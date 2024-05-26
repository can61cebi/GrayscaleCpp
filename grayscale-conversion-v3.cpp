#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

using namespace std;

bool convertToGrayscale(const string &girenDosyaAdi, const string &cikanDosyaAdi) {
    ifstream girenDosya(girenDosyaAdi, ios::binary);
    ofstream cikanDosya(cikanDosyaAdi, ios::binary);

    if (!girenDosya.is_open()) {
        cerr << "Dosya acilamadi: " << girenDosyaAdi << endl;
        return false;
    }

    unsigned char baslik[54];
    girenDosya.read((char*)baslik, 54);
    cikanDosya.write((char*)baslik, 54);

    int genislik = 1088;
    int yukseklik = 1088;
    int satir = ((genislik * 3) + 3) & (~3);
    unsigned char* veriler = (unsigned char*) malloc(satir);

    if (veriler == nullptr) {
        cerr << "Bellek ayirilamadi" << endl;
        return false;
    }

    for (int i = 0; i < yukseklik; i++) {
        girenDosya.read((char*)veriler, satir);

        for (int j = 0; j < genislik; j++) {
            int indis = j * 3;
            unsigned char r = veriler[indis + 2];
            unsigned char g = veriler[indis + 1];
            unsigned char b = veriler[indis];
            unsigned char gray = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);

            cikanDosya.put(gray);
            cikanDosya.put(gray);
            cikanDosya.put(gray);
        }

        for (int k = 0; k < satir - (genislik * 3); k++) {
            cikanDosya.put(0);
        }
    }

    free(veriler);
    girenDosya.close();
    cikanDosya.close();
    return true;
}

int main() {
    for (int dosyaNumarasi = 1; dosyaNumarasi < 9999; ++dosyaNumarasi) {
        stringstream ss;
        ss << "resimler_renkli/image" << setw(4) << setfill('0') << dosyaNumarasi << ".bmp";
        string girenDosyaAdi = ss.str();
        ss.str("");
        ss << "resimler_gri/gray_image" << setw(4) << setfill('0') << dosyaNumarasi << ".bmp";
        string cikanDosyaAdi = ss.str();

        if (!convertToGrayscale(girenDosyaAdi, cikanDosyaAdi)) {
            cerr << "Resimler griye donusturulemedi." << endl;
            continue;
        }
        
        // cout << "Resim başarıyla griye dönüştürüldü: " << cikanDosyaAdi << endl;
    }

    return 0;
}
