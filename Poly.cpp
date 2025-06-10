#include <iostream>
#include "Poly.h"
using namespace std;

PolyNode* CreatePoly(char* expr) {
    PolyNode* head = nullptr;
    PolyNode* tail = nullptr;
    bool isnegative = false; // Katsayının negatif olup olmadığını kontrol eden boolean
    double coef = 0; // Polinom teriminin katsayısının tutulduğu değişken
    int expo = 0; // Polinom teriminin üstel kısmının tutulduğu değişken

    for (int i = 0; expr[i] != '\0';) { // Girdinin son karakteri olmadığı sürece döngüyü devam ettir

        while (expr[i] == ' ') i++; // Boşluk varsa, boşluklar bitene kadar geç

        if (expr[i] == '-') { // Negatif işareti görünce negatif boolean'ı true yap ve bir ilerle
            isnegative = true;
            i++;
        }
        else { // Negatif işareti yoksa pozitif sayılır ve devam eder
            isnegative = false;
            if (expr[i] == '+') i++; // Eğer + işareti varsa, onu atla
        }

        while (expr[i] == ' ') i++; // Potansiyel boşluk varsa, boşluklar bitene kadar geç

        coef = 0; // Katsayıyı sıfırla
        bool isdouble = false; // Ondalık bir sayı ise, bu değer . karakterine gelince true olacak
        double decimalPlace = 0.1; // Ondalık yerleri yönetmek için değişken
        while ((expr[i] >= '0' && expr[i] <= '9') || expr[i] == '.') { // 0 ile 9 veya . karakterini görünceye kadar devam et
            if (expr[i] >= '0' && expr[i] <= '9') { // Nokta görünceye kadar tam sayı kısmını hesaplar
                if (isdouble) {
                    coef += (expr[i] - '0') * decimalPlace;
                    decimalPlace /= 10; // Ondalık kısmın bir sonraki basamakta kaymasını ayarlar
                }
                else {
                    coef = coef * 10 + (expr[i] - '0'); // Tam sayı kısmını hesaplar
                }
            }
            else if (expr[i] == '.') { // . karakterini görünce float olduğunu anlar ve isdouble'ı true yapar
                isdouble = true;
            }
            i++; // bir sonraki karakteri kontrol etmek için ilerler
        }
        if (coef == 0) coef = 1; // Yukarıdaki döngüde katsayı yoksa, 0 döner. Ancak katsayı 1 olmalı, bu yüzden düzeltir
        if (isnegative) coef = -coef; // Negatif bayrağı true ise katsayıyı -1 ile çarpar

        while (expr[i] == ' ') i++; // Potansiyel boşlukları geçmek için

        expo = 0; // Üstel ifadenin değerini sıfırlar
        if (expr[i] == 'x') {
            i++; // x karakterini görünce i değerini 1 arttırır
            if (expr[i] == '^') { // x'in ardından ^ karakterini görünce bir ilerle
                i++;
                while (expr[i] >= '0' && expr[i] <= '9') { // Eğer ^ karakterinden sonraki karakter 0 ile 9 arasındaysa döngüye girer ve üssü hesaplar
                    expo = expo * 10 + (expr[i] - '0');
                    i++;
                }
            }
            else { // Eğer ^ sonrası bir sayı yoksa otomatik olarak 1 olarak hesaplar
                expo = 1;
            }
        }

        while (expr[i] == ' ') i++; // Potansiyel boşlukları geçer ve bir sonraki terim için döngünün daha sağlıklı çalışmasını sağlar

        // Bulunan coef ve expo değerlerini, bu değerleri tutacak bağlantılı liste düğümlerine atar ve oluşturur
        PolyNode* newPolyNode = new PolyNode;
        newPolyNode->coef = coef; // Katsayıyı ayarla
        newPolyNode->exp = expo;  // Üssü ayarla
        newPolyNode->next = nullptr; // Sonraki işaretçiyi null olarak başlat
        if (head == nullptr) {
            head = tail = newPolyNode; // Eğer liste boşsa, baş ve son olarak yeni düğümü ayarla
        }
        else {
            tail->next = newPolyNode; // Yeni düğümü listenin sonuna ekle
            tail = newPolyNode; // Son işaretçiyi güncelle
        }
    }

    return head; // Oluşturulan polinom bağlantılı listesinin başını döndür
}


void DeletePoly(PolyNode* poly) {
    // Polinomun düğümlerini tek tek dolaşırak bellekten silindi.
    while (poly != nullptr) {
        PolyNode* temp = poly;
        poly = poly->next;
        delete temp;
    }
}



PolyNode* AddNode(PolyNode* head, double coef, int exp) {

    if (coef == 0) return head;


    PolyNode* newNode = new PolyNode;
    newNode->coef = coef;
    newNode->exp = exp;
    newNode->next = nullptr;

    // Liste boşsa veya yeni düğümün daha büyük bir üssü varsa başa eklendi.
    if (head == nullptr || head->exp < exp) {
        newNode->next = head;
        return newNode;
    }

    // Aynı üsse sahip bir düğüm varsa katsayı güncellendi.
    PolyNode* current = head, * prev = nullptr;
    while (current != nullptr && current->exp > exp) {
        prev = current;
        current = current->next;
    }

    if (current != nullptr && current->exp == exp) {
        current->coef += coef;
        if (current->coef == 0) { // Eğer katsayı sıfır olursa düğüm silindi
            if (prev == nullptr) {
                head = current->next;
            }
            else {
                prev->next = current->next;
            }
            delete current;
        }
        delete newNode;
        return head;
    }

    // Yeni düğüm uygun yere eklendi.
    newNode->next = current;
    if (prev == nullptr) {
        return newNode;
    }
    else {
        prev->next = newNode;
    }

    return head;
}


PolyNode* Add(PolyNode* poly1, PolyNode* poly2) {
    PolyNode* poly3 = nullptr;

    // poly2'nin her terimi için poly1'i dolaşarak işle
    for (PolyNode* p2 = poly2; p2 != nullptr; p2 = p2->next) {
        PolyNode* temp = poly1;
        bool found = false;

        // poly1 içinde poly2'nin üsse sahip terimini bul
        while (temp != nullptr) {
            if (p2->exp == temp->exp) {
                // Aynı üsse sahip terim bulundu, katsayıları topla
                double newCoef = temp->coef + p2->coef;
                poly3 = AddNode(poly3, newCoef, temp->exp);  // Yeni terimi poly3'e ekle
                found = true;
                break;
            }
            temp = temp->next;
        }

        // Eğer poly1 içinde aynı üsse sahip terim yoksa, poly2'nin terimini ekle
        if (!found) {
            poly3 = AddNode(poly3, p2->coef, p2->exp);
        }
    }

    // Geriye kalan poly1'deki terimleri poly3'e ekle
    for (PolyNode* p1 = poly1; p1 != nullptr; p1 = p1->next) {
        bool found = false;

        // poly3 içinde bu üsse sahip bir terim olup olmadığını kontrol et
        for (PolyNode* p3 = poly3; p3 != nullptr; p3 = p3->next) {
            if (p1->exp == p3->exp) {
                found = true;
                break;
            }
        }

        // Eğer poly3 içinde aynı üsse sahip terim yoksa poly1'in terimini ekle
        if (!found) {
            poly3 = AddNode(poly3, p1->coef, p1->exp);
        }
    }

    return poly3;
}

PolyNode* Subtract(PolyNode* poly1, PolyNode* poly2) {
    PolyNode* poly3 = nullptr;

    // poly2'nin her terimi için poly1'i dolaşarak işle
    for (PolyNode* p2 = poly2; p2 != nullptr; p2 = p2->next) {
        PolyNode* temp = poly1;
        bool found = false;

        // poly1 içinde poly2'nin üsse sahip terimini bul
        while (temp != nullptr) {
            if (p2->exp == temp->exp) {
                // Aynı üsse sahip terim bulundu, katsayıları çıkar
                double newCoef = temp->coef - p2->coef;
                poly3 = AddNode(poly3, newCoef, temp->exp);
                found = true;
                break;
            }
            temp = temp->next;
        }

        // Eğer poly1 içinde aynı üsse sahip terim yoksa, poly2'nin terimini - ile ekle
        if (!found) {
            poly3 = AddNode(poly3, -p2->coef, p2->exp);
        }
    }

    // Geriye kalan poly1'deki terimleri poly3'e ekle (çünkü poly2 ile karşılaştırılmamışlar)
    for (PolyNode* p1 = poly1; p1 != nullptr; p1 = p1->next) {
        bool found = false;

        // poly3 içinde bu üsse sahip bir terim olup olmadığını kontrol et
        for (PolyNode* p3 = poly3; p3 != nullptr; p3 = p3->next) {
            if (p1->exp == p3->exp) {
                found = true;
                break;
            }
        }

        // Eğer poly3 içinde aynı üsse sahip terim yoksa poly1'in terimini ekle
        if (!found) {
            poly3 = AddNode(poly3, p1->coef, p1->exp);
        }
    }

    return poly3;
}

PolyNode* Multiply(PolyNode* poly1, PolyNode* poly2) {
    PolyNode* poly3 = nullptr;


    for (PolyNode* p2 = poly2; p2 != nullptr; p2 = p2->next) {
        PolyNode* tempPoly = nullptr;
        PolyNode* temp = poly1;

        // poly1'in her terimi ile p2'nin terimini çarp
        while (temp != nullptr) {
            int newExp = temp->exp + p2->exp;
            double newCoef = temp->coef * p2->coef;

            // Geçici polinoma yeni terimi ekle
            tempPoly = AddNode(tempPoly, newCoef, newExp);
            temp = temp->next; // temp'i ilerlet
        }

        // tempPoly'yi sonuç polinomuna ekle
        poly3 = Add(poly3, tempPoly);


        DeletePoly(tempPoly);
    }

    return poly3;
}


double Evaluate(PolyNode* poly, double x) { //belirli bir değer için polinomun değerini hesaplayan fonksiyon.
    double result = 0.0;
    PolyNode* curr = poly;

    while (curr != nullptr) { //polinomun tüm düğümleri işlenir.
        result += curr->coef * pow(x, curr->exp);
        curr = curr->next; // döngünün bir sonraki iterasyonunda bir sonraki terim işlenir.
    }

    return result;
}

PolyNode* Derivative(PolyNode* poly) {
    PolyNode* curr = poly; // Mevcut polinomun başını işaret eden pointer
    PolyNode* derivedPoly = NULL; // Türevi tutacak polinomun başı

    // Mevcut polinomun her terimi için döngü
    while (curr != NULL) {
        if (curr->exp != 0) { // Eğer üstel kısım sıfırdan farklıysa
            PolyNode* newPolyNode = new PolyNode; // Yeni bir polinom düğümü oluştur
            newPolyNode->coef = curr->coef * curr->exp; // Yeni katsayı, mevcut katsayı * mevcut üs olarak hesapla
            newPolyNode->exp = curr->exp - 1; // Yeni üssü, mevcut üsten 1 çıkararak hesapla
            newPolyNode->next = NULL; // Sonraki işaretçiyi null olarak ayarla

            // Eğer türev polinom yoksa, onu baş olarak ayarla
            if (derivedPoly == NULL) {
                derivedPoly = newPolyNode; // Türevi başa atar
            }
            else {
                // Türev polinomun sonuna yeni düğümü ekle
                PolyNode* temp = derivedPoly; // Türev polinomunu geçici gösterici ile başla
                while (temp->next != NULL) { // Son düğümü bulana kadar ilerle
                    temp = temp->next;
                }
                temp->next = newPolyNode; // Yeni düğümü sona ekle
            }
        }
        curr = curr->next; // Mevcut polinomun bir sonraki terimine geç
    }

    return derivedPoly; // Hesaplanan türev polinomunu döndür
}




void Plot(PolyNode* poly, int x1, int x2) {
    const int SCREEN_HEIGHT = 30; // ekranın yüksekliği (satır sayısı)
    const int SCREEN_WIDTH = 80;  // ekranın genişliği (sabit sütun sayısı)
    const int X_CENTER = SCREEN_WIDTH / 2; //orta değerler
    const int Y_CENTER = SCREEN_HEIGHT / 2;


    char screen[SCREEN_HEIGHT][SCREEN_WIDTH]; //2D karakter dizisi
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            screen[i][j] = ' '; // ekranı boş karakterlerle doldurur.
        }
    }

    // X eksenini çizer.
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        screen[i][X_CENTER] = '|';
    }
    // Y eksenini çizer
    for (int j = 0; j < SCREEN_WIDTH; j++) {
        screen[Y_CENTER][j] = '-';
    }
    screen[Y_CENTER][X_CENTER] = '+'; //X ve Y eksenlerinin kesişimlerine ekler.

    // polinomun x değeri için y'yi hesaplayıp ekrana çizer.
    for (int x = x1; x <= x2; x++) {
        double y = Evaluate(poly, x); //Evaluate fonksiyonu ile o noktadaki değer hesaplanır.
        int screenX = X_CENTER + x;   // X konumu
        int screenY = Y_CENTER - (int)(y + 0.5); // Y konumu

        // eğer y ekran sınırları içindeyse çizer.
        if (screenY >= 0 && screenY < SCREEN_HEIGHT && screenX >= 0 && screenX < SCREEN_WIDTH) {
            screen[screenY][screenX] = '*';
        }
    }

    // ekranı yazdırır.
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            cout << screen[i][j]; // her satırı ekrana yazdırır
        }
        cout << endl;
    }
}
