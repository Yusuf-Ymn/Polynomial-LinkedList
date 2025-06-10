# 📐 Polinom İşlemleyici (C++)

Bu proje, C++ dilinde bağlantılı liste (linked list) veri yapısını kullanarak polinom işlemleri yapan bir uygulamadır. Proje ödev kapsamında geliştirilmiştir.

## 🧱 Kullanılan Veri Yapısı

Polinomlar, her terimi (katsayı ve üs) içeren düğümlerden oluşan **tek yönlü bağlı liste** (linked list) ile temsil edilmiştir. Bu yapı sayesinde polinomlar dinamik olarak oluşturulabilir, işlenebilir ve güncellenebilir.

## ✨ Özellikler

- Polinom oluşturma (string ifadeden)
- Polinomları toplama, çıkarma ve çarpma
- Belirli bir `x` değeri için değerlendirme
- Türev alma
- ASCII karakterlerle terminalde grafik çizimi

## 🗃️ Dosyalar

- `PolyNode.h` – Polinom düğüm tanımı (coef, exp, next)
- `Poly.h` – Fonksiyon prototipleri
- `poly.cpp` – Tüm işlevlerin implementasyonu
- `main.cpp` – Test fonksiyonları ve çıktılar

Program çalıştığında, sırasıyla 8 test otomatik olarak yürütülür.

### 📌 Not
Bu proje, veri yapıları dersi kapsamında ödev olarak hazırlanmıştır. Linked list kullanımı özellikle istenmiştir.
