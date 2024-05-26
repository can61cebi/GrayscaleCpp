#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

int main() {
    for (int dosyaNumarasi = 1; dosyaNumarasi < 9999; ++dosyaNumarasi) {
        stringstream ss;
        ss << "resimler_renkli/image" << setw(4) << setfill('0') << dosyaNumarasi << ".bmp";
        string girenDosyaAdi = ss.str();
        ss.str("");
        ss << "resimler_gri/gray_image" << setw(4) << setfill('0') << dosyaNumarasi << ".bmp";
        string cikanDosyaAdi = ss.str();

        ifstream girenDosya(girenDosyaAdi, ios::binary);
        ofstream cikanDosya(cikanDosyaAdi, ios::binary);

        if (!girenDosya.is_open()) {
            cerr << "Dosya acilamadi: " << girenDosyaAdi << endl;
            continue;
        }

        unsigned char baslik[54];
        girenDosya.read((char*)baslik, 54);
        cikanDosya.write((char*)baslik, 54);

        int genislik = 1088;
        int yukseklik = 1088;
        int satir = ((genislik * 3) + 3) & (~3);
        unsigned char veriler[satir];

        for (int i = 0; i < yukseklik; i++) {
            girenDosya.read((char*)veriler, satir);

            for (int j = 0; j < genislik; j++) {
                int indis = j * 3;
                unsigned char r = veriler[indis + 2];
                unsigned char g = veriler[indis + 1];
                unsigned char b = veriler[indis];
                unsigned char gray = (unsigned char)(r * 0.299 + g * 0.587 + b * 0.114);

                cikanDosya.put(gray);
                cikanDosya.put(gray);
                cikanDosya.put(gray);
            }

            for (int k = 0; k < satir - (genislik * 3); k++) {
                cikanDosya.put(0);
            }
        }

        girenDosya.close();
        cikanDosya.close();

        //  cout << "Resim basariyla griye donusturuldu: " << cikanDosyaAdi << endl;
    }

    return 0;
}

bu programdaki resmi griye dönüştürme işini fonksiyon haline getirip malloc ile işlemlerin yapılmasını sağlar mısın?