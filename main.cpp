//öğrenci:Mavlyuda Almazova
//ogr no:5220505043
//  main.cpp
//  Softmax Action Selection
//

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>

using namespace std;

// --- SUNUCU YAPISI ---
class Server {
public:
    double true_latency;
    double q_value; // Sadece Softmax için kullanılır

    Server(double initial_latency) : true_latency(initial_latency), q_value(0.0) {}

    // Sunucu gecikmesini zamanla değiştir (Non-stationary)
    void drift(mt19937& gen, normal_distribution<double>& dist) {
        true_latency += dist(gen);
        if (true_latency < 5.0) true_latency = 5.0;   // Alt limit
        if (true_latency > 100.0) true_latency = 100.0; // Üst limit
    }
};

// --- LOAD BALANCER (AJAN) ---
class LoadBalancer {
public:
    // 1. Softmax Seçimi
    int select_softmax(const vector<Server>& servers, double tau) {
        vector<double> probs;
        double sum_exp = 0.0;
        for (const auto& s : servers) {
            double e = exp(s.q_value / tau);
            probs.push_back(e);
            sum_exp += e;
        }
        static mt19937 gen(time(0));
        uniform_real_distribution<double> dis(0.0, 1.0);
        double pick = dis(gen);
        double current = 0.0;
        for (int i = 0; i < servers.size(); ++i) {
            current += probs[i] / sum_exp;
            if (pick <= current) return i;
        }
        return (int)servers.size() - 1;
    }

    // 2. Random Seçim
    int select_random(int k) {
        return rand() % k;
    }

    // 3. Round-Robin Seçim
    int select_round_robin(int k, int step) {
        return step % k;
    }
};

int main() {
    srand(time(0));
    const int K = 5;               // Sunucu sayısı
    const int STEPS = 2000;        // Analiz süresi
    const double TAU = 0.5;        // Sıcaklık
    const double ALPHA = 0.1;      // Öğrenme hızı

    // Her yöntem için ayrı sunucu kümeleri (Adil kıyaslama için)
    vector<Server> s_softmax, s_random, s_rr;
    for(int i=0; i<K; i++) {
        double start_lat = 20.0 + (rand() % 50);
        s_softmax.emplace_back(start_lat);
        s_random.emplace_back(start_lat);
        s_rr.emplace_back(start_lat);
    }

    LoadBalancer lb;
    mt19937 gen(time(0));
    normal_distribution<double> noise(0.0, 2.0); // Performans dalgalanması

    double total_lat_softmax = 0, total_lat_random = 0, total_lat_rr = 0;

    for(int t=0; t<STEPS; t++) {
        // Ortamdaki değişim (Her adımda sunucular değişir)
        for(int i=0; i<K; i++) {
            s_softmax[i].drift(gen, noise);
            s_random[i].drift(gen, noise);
            s_rr[i].drift(gen, noise);
        }

        // --- SOFTMAX SÜRECİ ---
        int idx_sm = lb.select_softmax(s_softmax, TAU);
        double lat_sm = s_softmax[idx_sm].true_latency;
        total_lat_softmax += lat_sm;
        // Öğrenme/Güncelleme (Agentic kısım burası)
        double reward = 100.0 / lat_sm;
        s_softmax[idx_sm].q_value += ALPHA * (reward - s_softmax[idx_sm].q_value);

        // --- RANDOM SÜRECİ ---
        total_lat_random += s_random[lb.select_random(K)].true_latency;

        // --- ROUND-ROBIN SÜRECİ ---
        total_lat_rr += s_rr[lb.select_round_robin(K, t)].true_latency;
    }

    // --- ANALİZ RAPORU ---
    cout << fixed << setprecision(2);
    cout << "==========================================" << endl;
    cout << "   LOAD BALANCER PERFORMANS ANALIZI       " << endl;
    cout << "==========================================" << endl;
    cout << "Softmax (Ajan) Ortalama Latency  : " << total_lat_softmax / STEPS << " ms" << endl;
    cout << "Round-Robin Ortalama Latency     : " << total_lat_rr / STEPS << " ms" << endl;
    cout << "Random Ortalama Latency          : " << total_lat_random / STEPS << " ms" << endl;
    cout << "------------------------------------------" << endl;
    cout << "Softmax Verimlilik Artisi: %" << ((total_lat_rr - total_lat_softmax) / total_lat_rr) * 100 << endl;
    cout << "==========================================" << endl;

    return 0;
}
