# GrayscaleCpp

Versiyon 1 için notlar

```
JPG yani jpeg dosyası tercih edilmemesinin sebebi JPEG fortamının sıkıştırma ve
çözme işlemleri, Huffman kodlaması ve DCT (Discrete Cosine Transform) gibi kompleks
teknikler içerdiği için OpenCV kullanmadan yapılması, görüntü işleme alanında
uzmanlık gerektirir. Bundan dolayı projede Microsoft'un geliştirdiği
BMP yani BitMap dosya formatını kullanmayı uygun gördüm.

BMP dosyalarının ilk 54 baytında dosya bilgilerini taşıyan, resmin boyutu ve formatı
gibi farklı bilgiler bulunur. Bu bilgileri programda unsigned char olarak baslik
değişkenin içerisinde saklıyoruz. Kendi cektigim resmin boyutlarını da 1088'e 1088 olarak
ayarladığım için bu programda da genislik ve yukseklik değerlerini sabit olarak aldık.

Resmin kırmızı, yeşil, mavi ve gri tonlarındaki verilerin unsigned char olarak
tutulmasının sebebi char'ın değeri -128 ile 127 arasında olabilir, unsigned char
0 ile 255 arası değerler alabilir. RGB değerleri de 0 ile 255 arasında bulunur.*/

Resmin boyutları float olarak alındığı için satır ve diğer for döngülerinde
typecasting işlemi uygulanmıştır. resim işleme işlemleri integer değerleriyle uygulanır,
uygulanmadığı taktirde program compile time sırasında hata verir. (sorunlar/resim001)

İç içe for döngüsündeki ilk for'da resmin yüksekliğine yani satıra denk gelen pikseller,
ikinci for'da genişliğe yani sütuna denk gelen pikseller işlenerek griye dönüştürülür.

unsigned char gray = (unsigned char)(r * 0.299 + g * 0.587 + b * 0.114) işlemindeki
amaç, insan gözünün en iyi yeşili algılaması sebebiyle 0.587 ile çarpımıyla resmideki
en yüksek değere sahip olmasıyken; kırmızı biraz daha az algılanması sebebiyle 0.299,
mavi de en az algılanmasıyla 0.114 değerleriyle çarpılarak resim gri tonlanmıştır.

Bu programın v1'de yalnızca 1 adet resmi griye dönüştürmesi amaçlanarak float
kullanılıp yavaşlatılarak olabilecek en basit şekilde çalışması amaçlanmıştır.
```