#include <iostream>
#include "Poly.h"
using namespace std;

PolyNode* CreatePoly(char* expr) {
    PolyNode* head = nullptr;
    PolyNode* tail = nullptr;
    bool isnegative = false; // Katsayýnýn negatif olup olmadýðýný kontrol eden boolean
    double coef = 0; // Polinom teriminin katsayýsýnýn tutulduðu deðiþken
    int expo = 0; // Polinom teriminin üstel kýsmýnýn tutulduðu deðiþken

    for (int i = 0; expr[i] != '\0';) { // Girdinin son karakteri olmadýðý sürece döngüyü devam ettir

        while (expr[i] == ' ') i++; // Boþluk varsa, boþluklar bitene kadar geç

        if (expr[i] == '-') { // Negatif iþareti görünce negatif boolean'ý true yap ve bir ilerle
            isnegative = true;
            i++;
        }
        else { // Negatif iþareti yoksa pozitif sayýlýr ve devam eder
            isnegative = false;
            if (expr[i] == '+') i++; // Eðer + iþareti varsa, onu atla
        }

        while (expr[i] == ' ') i++; // Potansiyel boþluk varsa, boþluklar bitene kadar geç

        coef = 0; // Katsayýyý sýfýrla
        bool isdouble = false; // Ondalýk bir sayý ise, bu deðer . karakterine gelince true olacak
        double decimalPlace = 0.1; // Ondalýk yerleri yönetmek için deðiþken
        while ((expr[i] >= '0' && expr[i] <= '9') || expr[i] == '.') { // 0 ile 9 veya . karakterini görünceye kadar devam et
            if (expr[i] >= '0' && expr[i] <= '9') { // Nokta görünceye kadar tam sayý kýsmýný hesaplar
                if (isdouble) {
                    coef += (expr[i] - '0') * decimalPlace;
                    decimalPlace /= 10; // Ondalýk kýsmýn bir sonraki basamakta kaymasýný ayarlar
                }
                else {
                    coef = coef * 10 + (expr[i] - '0'); // Tam sayý kýsmýný hesaplar
                }
            }
            else if (expr[i] == '.') { // . karakterini görünce float olduðunu anlar ve isdouble'ý true yapar
                isdouble = true;
            }
            i++; // bir sonraki karakteri kontrol etmek için ilerler
        }
        if (coef == 0) coef = 1; // Yukarýdaki döngüde katsayý yoksa, 0 döner. Ancak katsayý 1 olmalý, bu yüzden düzeltir
        if (isnegative) coef = -coef; // Negatif bayraðý true ise katsayýyý -1 ile çarpar

        while (expr[i] == ' ') i++; // Potansiyel boþluklarý geçmek için

        expo = 0; // Üstel ifadenin deðerini sýfýrlar
        if (expr[i] == 'x') {
            i++; // x karakterini görünce i deðerini 1 arttýrýr
            if (expr[i] == '^') { // x'in ardýndan ^ karakterini görünce bir ilerle
                i++;
                while (expr[i] >= '0' && expr[i] <= '9') { // Eðer ^ karakterinden sonraki karakter 0 ile 9 arasýndaysa döngüye girer ve üssü hesaplar
                    expo = expo * 10 + (expr[i] - '0');
                    i++;
                }
            }
            else { // Eðer ^ sonrasý bir sayý yoksa otomatik olarak 1 olarak hesaplar
                expo = 1;
            }
        }

        while (expr[i] == ' ') i++; // Potansiyel boþluklarý geçer ve bir sonraki terim için döngünün daha saðlýklý çalýþmasýný saðlar

        // Bulunan coef ve expo deðerlerini, bu deðerleri tutacak baðlantýlý liste düðümlerine atar ve oluþturur
        PolyNode* newPolyNode = new PolyNode;
        newPolyNode->coef = coef; // Katsayýyý ayarla
        newPolyNode->exp = expo;  // Üssü ayarla
        newPolyNode->next = nullptr; // Sonraki iþaretçiyi null olarak baþlat
        if (head == nullptr) {
            head = tail = newPolyNode; // Eðer liste boþsa, baþ ve son olarak yeni düðümü ayarla
        }
        else {
            tail->next = newPolyNode; // Yeni düðümü listenin sonuna ekle
            tail = newPolyNode; // Son iþaretçiyi güncelle
        }
    }

    return head; // Oluþturulan polinom baðlantýlý listesinin baþýný döndür
}


void DeletePoly(PolyNode* poly) {
    // Polinomun düðümlerini tek tek dolaþýrak bellekten silindi.
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

    // Liste boþsa veya yeni düðümün daha büyük bir üssü varsa baþa eklendi.
    if (head == nullptr || head->exp < exp) {
        newNode->next = head;
        return newNode;
    }

    // Ayný üsse sahip bir düðüm varsa katsayý güncellendi.
    PolyNode* current = head, * prev = nullptr;
    while (current != nullptr && current->exp > exp) {
        prev = current;
        current = current->next;
    }

    if (current != nullptr && current->exp == exp) {
        current->coef += coef;
        if (current->coef == 0) { // Eðer katsayý sýfýr olursa düðüm silindi
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

    // Yeni düðüm uygun yere eklendi.
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

    // poly2'nin her terimi için poly1'i dolaþarak iþle
    for (PolyNode* p2 = poly2; p2 != nullptr; p2 = p2->next) {
        PolyNode* temp = poly1;
        bool found = false;

        // poly1 içinde poly2'nin üsse sahip terimini bul
        while (temp != nullptr) {
            if (p2->exp == temp->exp) {
                // Ayný üsse sahip terim bulundu, katsayýlarý topla
                double newCoef = temp->coef + p2->coef;
                poly3 = AddNode(poly3, newCoef, temp->exp);  // Yeni terimi poly3'e ekle
                found = true;
                break;
            }
            temp = temp->next;
        }

        // Eðer poly1 içinde ayný üsse sahip terim yoksa, poly2'nin terimini ekle
        if (!found) {
            poly3 = AddNode(poly3, p2->coef, p2->exp);
        }
    }

    // Geriye kalan poly1'deki terimleri poly3'e ekle
    for (PolyNode* p1 = poly1; p1 != nullptr; p1 = p1->next) {
        bool found = false;

        // poly3 içinde bu üsse sahip bir terim olup olmadýðýný kontrol et
        for (PolyNode* p3 = poly3; p3 != nullptr; p3 = p3->next) {
            if (p1->exp == p3->exp) {
                found = true;
                break;
            }
        }

        // Eðer poly3 içinde ayný üsse sahip terim yoksa poly1'in terimini ekle
        if (!found) {
            poly3 = AddNode(poly3, p1->coef, p1->exp);
        }
    }

    return poly3;
}

PolyNode* Subtract(PolyNode* poly1, PolyNode* poly2) {
    PolyNode* poly3 = nullptr;

    // poly2'nin her terimi için poly1'i dolaþarak iþle
    for (PolyNode* p2 = poly2; p2 != nullptr; p2 = p2->next) {
        PolyNode* temp = poly1;
        bool found = false;

        // poly1 içinde poly2'nin üsse sahip terimini bul
        while (temp != nullptr) {
            if (p2->exp == temp->exp) {
                // Ayný üsse sahip terim bulundu, katsayýlarý çýkar
                double newCoef = temp->coef - p2->coef;
                poly3 = AddNode(poly3, newCoef, temp->exp);
                found = true;
                break;
            }
            temp = temp->next;
        }

        // Eðer poly1 içinde ayný üsse sahip terim yoksa, poly2'nin terimini - ile ekle
        if (!found) {
            poly3 = AddNode(poly3, -p2->coef, p2->exp);
        }
    }

    // Geriye kalan poly1'deki terimleri poly3'e ekle (çünkü poly2 ile karþýlaþtýrýlmamýþlar)
    for (PolyNode* p1 = poly1; p1 != nullptr; p1 = p1->next) {
        bool found = false;

        // poly3 içinde bu üsse sahip bir terim olup olmadýðýný kontrol et
        for (PolyNode* p3 = poly3; p3 != nullptr; p3 = p3->next) {
            if (p1->exp == p3->exp) {
                found = true;
                break;
            }
        }

        // Eðer poly3 içinde ayný üsse sahip terim yoksa poly1'in terimini ekle
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


double Evaluate(PolyNode* poly, double x) { //belirli bir deðer için polinomun deðerini hesaplayan fonksiyon.
    double result = 0.0;
    PolyNode* curr = poly;

    while (curr != nullptr) { //polinomun tüm düðümleri iþlenir.
        result += curr->coef * pow(x, curr->exp);
        curr = curr->next; // döngünün bir sonraki iterasyonunda bir sonraki terim iþlenir.
    }

    return result;
}

PolyNode* Derivative(PolyNode* poly) {
    PolyNode* curr = poly; // Mevcut polinomun baþýný iþaret eden pointer
    PolyNode* derivedPoly = NULL; // Türevi tutacak polinomun baþý

    // Mevcut polinomun her terimi için döngü
    while (curr != NULL) {
        if (curr->exp != 0) { // Eðer üstel kýsým sýfýrdan farklýysa
            PolyNode* newPolyNode = new PolyNode; // Yeni bir polinom düðümü oluþtur
            newPolyNode->coef = curr->coef * curr->exp; // Yeni katsayý, mevcut katsayý * mevcut üs olarak hesapla
            newPolyNode->exp = curr->exp - 1; // Yeni üssü, mevcut üsten 1 çýkararak hesapla
            newPolyNode->next = NULL; // Sonraki iþaretçiyi null olarak ayarla

            // Eðer türev polinom yoksa, onu baþ olarak ayarla
            if (derivedPoly == NULL) {
                derivedPoly = newPolyNode; // Türevi baþa atar
            }
            else {
                // Türev polinomun sonuna yeni düðümü ekle
                PolyNode* temp = derivedPoly; // Türev polinomunu geçici gösterici ile baþla
                while (temp->next != NULL) { // Son düðümü bulana kadar ilerle
                    temp = temp->next;
                }
                temp->next = newPolyNode; // Yeni düðümü sona ekle
            }
        }
        curr = curr->next; // Mevcut polinomun bir sonraki terimine geç
    }

    return derivedPoly; // Hesaplanan türev polinomunu döndür
}




void Plot(PolyNode* poly, int x1, int x2) {
    const int SCREEN_HEIGHT = 30; // ekranýn yüksekliði (satýr sayýsý)
    const int SCREEN_WIDTH = 80;  // ekranýn geniþliði (sabit sütun sayýsý)
    const int X_CENTER = SCREEN_WIDTH / 2; //orta deðerler
    const int Y_CENTER = SCREEN_HEIGHT / 2;


    char screen[SCREEN_HEIGHT][SCREEN_WIDTH]; //2D karakter dizisi
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            screen[i][j] = ' '; // ekraný boþ karakterlerle doldurur.
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
    screen[Y_CENTER][X_CENTER] = '+'; //X ve Y eksenlerinin kesiþimlerine ekler.

    // polinomun x deðeri için y'yi hesaplayýp ekrana çizer.
    for (int x = x1; x <= x2; x++) {
        double y = Evaluate(poly, x); //Evaluate fonksiyonu ile o noktadaki deðer hesaplanýr.
        int screenX = X_CENTER + x;   // X konumu
        int screenY = Y_CENTER - (int)(y + 0.5); // Y konumu

        // eðer y ekran sýnýrlarý içindeyse çizer.
        if (screenY >= 0 && screenY < SCREEN_HEIGHT && screenX >= 0 && screenX < SCREEN_WIDTH) {
            screen[screenY][screenX] = '*';
        }
    }

    // ekraný yazdýrýr.
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            cout << screen[i][j]; // her satýrý ekrana yazdýrýr
        }
        cout << endl;
    }
}