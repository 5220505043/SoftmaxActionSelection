Softmax Based Client-Side Load Balancer

Bu proje, dağıtık sistemlerde sunucu performanslarının zamanla değiştiği (**non-stationary**) ve gürültülü olduğu senaryolarda, toplam bekleme süresini (**latency**) minimize etmek için geliştirilmiş **Softmax Action Selection** tabanlı bir yük dengeleyici simülasyonudur.

##  Proje Özeti

Geleneksel yük dengeleme algoritmaları (Round-Robin, Random vb.) sunucu performansındaki anlık değişimleri dikkate almaz. Bu projede kullanılan **Agentic Kodlama** yaklaşımı ile yük dengeleyici, bir ajan gibi davranarak sunucuların geçmiş performans verilerinden öğrenir ve en verimli sunucuya öncelik verir.

## Algoritma Detayları

Projede kullanılan **Softmax Action Selection** yöntemi, her sunucu için bir başarı puanı (-value) tutar ve seçim olasılığını şu formülle hesaplar:

* **Exploitation:** Bilinen en iyi sunucuya odaklanma.
* **Exploration:** Diğer sunucuların performansını periyodik olarak kontrol etme.
* **Nümerik Stabilite:** Hesaplamalarda üstel değerlerin taşmasını önlemek için uygun normalizasyon ve veri tipleri kullanılmıştır.

##Performans Analizi

Simülasyon sonuçlarına göre Softmax ajanı, geleneksel yöntemlere kıyasla belirgin bir avantaj sağlamaktadır:

| Algoritma | Ortalama Gecikme (Latency) | Durum |
| --- | --- | --- |
| **Softmax (Agent)** | **~33 ms** | **En Verimli** |
| Random | ~49 ms | Verimsiz |
| Round-Robin | ~60 ms | En Verimsiz |

**Verimlilik Artışı:** Softmax kullanımı, Round-Robin yöntemine göre yaklaşık **%44** daha düşük gecikme sağlamıştır.

## Kurulum ve Çalıştırma

Proje C++ dili kullanılarak geliştirilmiştir.

1. Repoyu klonlayın: `git clone https://github.com/kullaniciadi/Softmax-Load-Balancer.git`
2. Derleyin: `g++ main.cpp -o LoadBalancer`
3. Çalıştırın: `./LoadBalancer`

## Kullanılan Teknolojiler

* **Dil:** C++
* **IDE:** Xcode / Clion
* **Yöntem:** Reinforcement Learning (Action Selection), Agentic Coding
* **AI Desteği:** Gemini (Kod mimarisi ve algoritma optimizasyonu için)


