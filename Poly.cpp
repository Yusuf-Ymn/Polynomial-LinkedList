#include <iostream>
#include "Poly.h"
using namespace std;

PolyNode* CreatePoly(char* expr) {
    PolyNode* head = nullptr;
    PolyNode* tail = nullptr;
    bool isnegative = false; // Katsay�n�n negatif olup olmad���n� kontrol eden boolean
    double coef = 0; // Polinom teriminin katsay�s�n�n tutuldu�u de�i�ken
    int expo = 0; // Polinom teriminin �stel k�sm�n�n tutuldu�u de�i�ken

    for (int i = 0; expr[i] != '\0';) { // Girdinin son karakteri olmad��� s�rece d�ng�y� devam ettir

        while (expr[i] == ' ') i++; // Bo�luk varsa, bo�luklar bitene kadar ge�

        if (expr[i] == '-') { // Negatif i�areti g�r�nce negatif boolean'� true yap ve bir ilerle
            isnegative = true;
            i++;
        }
        else { // Negatif i�areti yoksa pozitif say�l�r ve devam eder
            isnegative = false;
            if (expr[i] == '+') i++; // E�er + i�areti varsa, onu atla
        }

        while (expr[i] == ' ') i++; // Potansiyel bo�luk varsa, bo�luklar bitene kadar ge�

        coef = 0; // Katsay�y� s�f�rla
        bool isdouble = false; // Ondal�k bir say� ise, bu de�er . karakterine gelince true olacak
        double decimalPlace = 0.1; // Ondal�k yerleri y�netmek i�in de�i�ken
        while ((expr[i] >= '0' && expr[i] <= '9') || expr[i] == '.') { // 0 ile 9 veya . karakterini g�r�nceye kadar devam et
            if (expr[i] >= '0' && expr[i] <= '9') { // Nokta g�r�nceye kadar tam say� k�sm�n� hesaplar
                if (isdouble) {
                    coef += (expr[i] - '0') * decimalPlace;
                    decimalPlace /= 10; // Ondal�k k�sm�n bir sonraki basamakta kaymas�n� ayarlar
                }
                else {
                    coef = coef * 10 + (expr[i] - '0'); // Tam say� k�sm�n� hesaplar
                }
            }
            else if (expr[i] == '.') { // . karakterini g�r�nce float oldu�unu anlar ve isdouble'� true yapar
                isdouble = true;
            }
            i++; // bir sonraki karakteri kontrol etmek i�in ilerler
        }
        if (coef == 0) coef = 1; // Yukar�daki d�ng�de katsay� yoksa, 0 d�ner. Ancak katsay� 1 olmal�, bu y�zden d�zeltir
        if (isnegative) coef = -coef; // Negatif bayra�� true ise katsay�y� -1 ile �arpar

        while (expr[i] == ' ') i++; // Potansiyel bo�luklar� ge�mek i�in

        expo = 0; // �stel ifadenin de�erini s�f�rlar
        if (expr[i] == 'x') {
            i++; // x karakterini g�r�nce i de�erini 1 artt�r�r
            if (expr[i] == '^') { // x'in ard�ndan ^ karakterini g�r�nce bir ilerle
                i++;
                while (expr[i] >= '0' && expr[i] <= '9') { // E�er ^ karakterinden sonraki karakter 0 ile 9 aras�ndaysa d�ng�ye girer ve �ss� hesaplar
                    expo = expo * 10 + (expr[i] - '0');
                    i++;
                }
            }
            else { // E�er ^ sonras� bir say� yoksa otomatik olarak 1 olarak hesaplar
                expo = 1;
            }
        }

        while (expr[i] == ' ') i++; // Potansiyel bo�luklar� ge�er ve bir sonraki terim i�in d�ng�n�n daha sa�l�kl� �al��mas�n� sa�lar

        // Bulunan coef ve expo de�erlerini, bu de�erleri tutacak ba�lant�l� liste d���mlerine atar ve olu�turur
        PolyNode* newPolyNode = new PolyNode;
        newPolyNode->coef = coef; // Katsay�y� ayarla
        newPolyNode->exp = expo;  // �ss� ayarla
        newPolyNode->next = nullptr; // Sonraki i�aret�iyi null olarak ba�lat
        if (head == nullptr) {
            head = tail = newPolyNode; // E�er liste bo�sa, ba� ve son olarak yeni d���m� ayarla
        }
        else {
            tail->next = newPolyNode; // Yeni d���m� listenin sonuna ekle
            tail = newPolyNode; // Son i�aret�iyi g�ncelle
        }
    }

    return head; // Olu�turulan polinom ba�lant�l� listesinin ba��n� d�nd�r
}


void DeletePoly(PolyNode* poly) {
    // Polinomun d���mlerini tek tek dola��rak bellekten silindi.
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

    // Liste bo�sa veya yeni d���m�n daha b�y�k bir �ss� varsa ba�a eklendi.
    if (head == nullptr || head->exp < exp) {
        newNode->next = head;
        return newNode;
    }

    // Ayn� �sse sahip bir d���m varsa katsay� g�ncellendi.
    PolyNode* current = head, * prev = nullptr;
    while (current != nullptr && current->exp > exp) {
        prev = current;
        current = current->next;
    }

    if (current != nullptr && current->exp == exp) {
        current->coef += coef;
        if (current->coef == 0) { // E�er katsay� s�f�r olursa d���m silindi
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

    // Yeni d���m uygun yere eklendi.
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

    // poly2'nin her terimi i�in poly1'i dola�arak i�le
    for (PolyNode* p2 = poly2; p2 != nullptr; p2 = p2->next) {
        PolyNode* temp = poly1;
        bool found = false;

        // poly1 i�inde poly2'nin �sse sahip terimini bul
        while (temp != nullptr) {
            if (p2->exp == temp->exp) {
                // Ayn� �sse sahip terim bulundu, katsay�lar� topla
                double newCoef = temp->coef + p2->coef;
                poly3 = AddNode(poly3, newCoef, temp->exp);  // Yeni terimi poly3'e ekle
                found = true;
                break;
            }
            temp = temp->next;
        }

        // E�er poly1 i�inde ayn� �sse sahip terim yoksa, poly2'nin terimini ekle
        if (!found) {
            poly3 = AddNode(poly3, p2->coef, p2->exp);
        }
    }

    // Geriye kalan poly1'deki terimleri poly3'e ekle
    for (PolyNode* p1 = poly1; p1 != nullptr; p1 = p1->next) {
        bool found = false;

        // poly3 i�inde bu �sse sahip bir terim olup olmad���n� kontrol et
        for (PolyNode* p3 = poly3; p3 != nullptr; p3 = p3->next) {
            if (p1->exp == p3->exp) {
                found = true;
                break;
            }
        }

        // E�er poly3 i�inde ayn� �sse sahip terim yoksa poly1'in terimini ekle
        if (!found) {
            poly3 = AddNode(poly3, p1->coef, p1->exp);
        }
    }

    return poly3;
}

PolyNode* Subtract(PolyNode* poly1, PolyNode* poly2) {
    PolyNode* poly3 = nullptr;

    // poly2'nin her terimi i�in poly1'i dola�arak i�le
    for (PolyNode* p2 = poly2; p2 != nullptr; p2 = p2->next) {
        PolyNode* temp = poly1;
        bool found = false;

        // poly1 i�inde poly2'nin �sse sahip terimini bul
        while (temp != nullptr) {
            if (p2->exp == temp->exp) {
                // Ayn� �sse sahip terim bulundu, katsay�lar� ��kar
                double newCoef = temp->coef - p2->coef;
                poly3 = AddNode(poly3, newCoef, temp->exp);
                found = true;
                break;
            }
            temp = temp->next;
        }

        // E�er poly1 i�inde ayn� �sse sahip terim yoksa, poly2'nin terimini - ile ekle
        if (!found) {
            poly3 = AddNode(poly3, -p2->coef, p2->exp);
        }
    }

    // Geriye kalan poly1'deki terimleri poly3'e ekle (��nk� poly2 ile kar��la�t�r�lmam��lar)
    for (PolyNode* p1 = poly1; p1 != nullptr; p1 = p1->next) {
        bool found = false;

        // poly3 i�inde bu �sse sahip bir terim olup olmad���n� kontrol et
        for (PolyNode* p3 = poly3; p3 != nullptr; p3 = p3->next) {
            if (p1->exp == p3->exp) {
                found = true;
                break;
            }
        }

        // E�er poly3 i�inde ayn� �sse sahip terim yoksa poly1'in terimini ekle
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

        // poly1'in her terimi ile p2'nin terimini �arp
        while (temp != nullptr) {
            int newExp = temp->exp + p2->exp;
            double newCoef = temp->coef * p2->coef;

            // Ge�ici polinoma yeni terimi ekle
            tempPoly = AddNode(tempPoly, newCoef, newExp);
            temp = temp->next; // temp'i ilerlet
        }

        // tempPoly'yi sonu� polinomuna ekle
        poly3 = Add(poly3, tempPoly);


        DeletePoly(tempPoly);
    }

    return poly3;
}


double Evaluate(PolyNode* poly, double x) { //belirli bir de�er i�in polinomun de�erini hesaplayan fonksiyon.
    double result = 0.0;
    PolyNode* curr = poly;

    while (curr != nullptr) { //polinomun t�m d���mleri i�lenir.
        result += curr->coef * pow(x, curr->exp);
        curr = curr->next; // d�ng�n�n bir sonraki iterasyonunda bir sonraki terim i�lenir.
    }

    return result;
}

PolyNode* Derivative(PolyNode* poly) {
    PolyNode* curr = poly; // Mevcut polinomun ba��n� i�aret eden pointer
    PolyNode* derivedPoly = NULL; // T�revi tutacak polinomun ba��

    // Mevcut polinomun her terimi i�in d�ng�
    while (curr != NULL) {
        if (curr->exp != 0) { // E�er �stel k�s�m s�f�rdan farkl�ysa
            PolyNode* newPolyNode = new PolyNode; // Yeni bir polinom d���m� olu�tur
            newPolyNode->coef = curr->coef * curr->exp; // Yeni katsay�, mevcut katsay� * mevcut �s olarak hesapla
            newPolyNode->exp = curr->exp - 1; // Yeni �ss�, mevcut �sten 1 ��kararak hesapla
            newPolyNode->next = NULL; // Sonraki i�aret�iyi null olarak ayarla

            // E�er t�rev polinom yoksa, onu ba� olarak ayarla
            if (derivedPoly == NULL) {
                derivedPoly = newPolyNode; // T�revi ba�a atar
            }
            else {
                // T�rev polinomun sonuna yeni d���m� ekle
                PolyNode* temp = derivedPoly; // T�rev polinomunu ge�ici g�sterici ile ba�la
                while (temp->next != NULL) { // Son d���m� bulana kadar ilerle
                    temp = temp->next;
                }
                temp->next = newPolyNode; // Yeni d���m� sona ekle
            }
        }
        curr = curr->next; // Mevcut polinomun bir sonraki terimine ge�
    }

    return derivedPoly; // Hesaplanan t�rev polinomunu d�nd�r
}




void Plot(PolyNode* poly, int x1, int x2) {
    const int SCREEN_HEIGHT = 30; // ekran�n y�ksekli�i (sat�r say�s�)
    const int SCREEN_WIDTH = 80;  // ekran�n geni�li�i (sabit s�tun say�s�)
    const int X_CENTER = SCREEN_WIDTH / 2; //orta de�erler
    const int Y_CENTER = SCREEN_HEIGHT / 2;


    char screen[SCREEN_HEIGHT][SCREEN_WIDTH]; //2D karakter dizisi
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            screen[i][j] = ' '; // ekran� bo� karakterlerle doldurur.
        }
    }

    // X eksenini �izer.
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        screen[i][X_CENTER] = '|';
    }
    // Y eksenini �izer
    for (int j = 0; j < SCREEN_WIDTH; j++) {
        screen[Y_CENTER][j] = '-';
    }
    screen[Y_CENTER][X_CENTER] = '+'; //X ve Y eksenlerinin kesi�imlerine ekler.

    // polinomun x de�eri i�in y'yi hesaplay�p ekrana �izer.
    for (int x = x1; x <= x2; x++) {
        double y = Evaluate(poly, x); //Evaluate fonksiyonu ile o noktadaki de�er hesaplan�r.
        int screenX = X_CENTER + x;   // X konumu
        int screenY = Y_CENTER - (int)(y + 0.5); // Y konumu

        // e�er y ekran s�n�rlar� i�indeyse �izer.
        if (screenY >= 0 && screenY < SCREEN_HEIGHT && screenX >= 0 && screenX < SCREEN_WIDTH) {
            screen[screenY][screenX] = '*';
        }
    }

    // ekran� yazd�r�r.
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            cout << screen[i][j]; // her sat�r� ekrana yazd�r�r
        }
        cout << endl;
    }
}