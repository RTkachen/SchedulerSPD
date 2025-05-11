#include "Problem.hpp"

Problem::Problem(const std::vector<DaneZad>& dane, int liczbaZadan, int liczbaMaszyn)
    : n(liczbaZadan), m(liczbaMaszyn), instancjaZadan(dane) 
{
    maszyny.assign(m, std::vector<DaneZad>{});
}

void Problem::clearMachines() {
    for (auto& maszyna : maszyny) {
        maszyna.clear();
    }
}

void Problem::lsa() {

    for (int i = 0; i < n; ++i) {
        int indeksMaszyny = i % m;
        maszyny[indeksMaszyny].push_back(instancjaZadan[i]);
    }
}

void Problem::lpt() {

    // Stwórz lokalną kopię i posortuj malejąco
    std::vector<DaneZad> zadania = instancjaZadan;
    std::sort(zadania.begin(), zadania.end(), [](const DaneZad& a, const DaneZad& b){return a.czas > b.czas;});

    std::vector<int> sumaMaszyn(m, 0);
    //petla idaca po kazdym elemencie wektora zadania
    for (const auto& zad : zadania) {
        //roznica pomiedzy iteratorami powoduje ze zwracany jest indeks a nie iterator
        int indeks = std::min_element(sumaMaszyn.begin(), sumaMaszyn.end()) - sumaMaszyn.begin();
        //wrzucamy do maszyny zadanie o czasie p
        maszyny[indeks].push_back(zad);
        //dodajemy p do sumy czasu na maszynie do ktorej dodano p
        sumaMaszyn[indeks] += zad.czas;
    }
}
/****************************** BRUTE SEARCH ********************************* */
void Problem::bruteSearch() {
    if (m == 2) {

        // Używamy 64-bitów
        long long bestMask = 0;
        int bestMaxLoad = INT_MAX;

        // limit = 2^(n-1)
        long long limit = 1LL << (n - 1);

        for (long long mask = 0; mask < limit; ++mask) {
            int load0 = 0;
            int load1 = 0;

            // Obliczamy obciążenie obu maszyn
            for (int i = 0; i < n; ++i) {
                if (mask & (1LL << i)) {
                    load1 += instancjaZadan[i].czas;
                } else {
                    load0 += instancjaZadan[i].czas;
                }
            }

            int currentMax = std::max(load0, load1);
            if (currentMax < bestMaxLoad) {
                bestMaxLoad = currentMax;
                bestMask = mask;
            }
        }

        // Przydzielamy zadania według bestMask
        for (int i = 0; i < n; ++i) {
            if (bestMask & (1LL << i)) {
                maszyny[1].push_back(instancjaZadan[i]);
            } else {
                maszyny[0].push_back(instancjaZadan[i]);
            }
        }
    }
    else if (m == 3) {

        long long bestCode = 0;
        int bestMaxLoad = INT_MAX;

        // Obliczamy 3^n
        long long limit = 1;
        for (int i = 0; i < n; ++i) {
            limit *= 3;
        }

        for (long long code = 0; code < limit; ++code) {
            std::array<int,3> load = {0, 0, 0};
            long long x = code;

            // Dekodujemy code w systemie trójkowym:
            // cyfra trójkowa = x % 3 → numer maszyny
            for (int i = 0; i < n; ++i) {
                int trit = x % 3;
                x /= 3;
                load[trit] += instancjaZadan[i].czas;
            }

            int currentMax = std::max({load[0], load[1], load[2]});
            if (currentMax < bestMaxLoad) {
                bestMaxLoad = currentMax;
                bestCode = code;
            }
        }

        // Odtwarzamy najlepszy podział
        {
            long long x = bestCode;
            for (int i = 0; i < n; ++i) {
                int trit = x % 3;
                x /= 3;
                maszyny[trit].push_back(instancjaZadan[i]);
            }
        }
    }
}
/****************************** PD ********************************* */
void Problem::dynamicSearch() {
    if (m == 2) {
        // 1) Obliczamy sumę czasów i połowę tej sumy.
        int total = std::accumulate(
            instancjaZadan.begin(), instancjaZadan.end(),
            0, [](int acc, const DaneZad& z){ return acc + z.czas; }
        );
        int half = total / 2;

        // 2) Tworzymy DP jako wektor (n+1) x (half+1), przy czym używamy char.
        // 0 oznacza false, 1 oznacza true.
        std::vector<std::vector<char>> DP(n + 1, std::vector<char>(half + 1, 0));
        DP[0][0] = 1;

        // 3) Wypełniamy DP.
        for (int i = 1; i <= n; ++i) {
            int t = instancjaZadan[i - 1].czas;
            for (int s = 0; s <= half; ++s) {
                // Jeśli z poprzednich zadań mogliśmy osiągnąć sumę s,
                // lub jeśli przyjmując obecne zadanie (o ile s>=t) osiągamy sumę s,
                // ustawiamy DP[i][s] na 1.
                DP[i][s] = (DP[i - 1][s] ||
                           (s >= t && DP[i - 1][s - t])) ? 1 : 0;
            }
        }

        // 4) Znajdujemy największe s ≤ half, dla którego DP[n][s] jest prawdziwe.
        int bestS = 0;
        for (int s = half; s >= 0; --s) {
            if (DP[n][s]) { bestS = s; break; }
        }

        // 5) Backtracking – odtwarzamy przydział zadań.
        // Na potrzeby backtrackingu zakładamy, że zadania które „wybraliśmy” podczas DP trafią na maszynę 0,
        // a pozostałe – na maszynę 1. Przed backtrackingiem można opcjonalnie wyczyścić maszyny.
        maszyny[0].clear();
        maszyny[1].clear();

        int s = bestS;
        for (int i = n; i > 0; --i) {
            int t = instancjaZadan[i - 1].czas;
            if (DP[i - 1][s]) {
                // Gdy stan DP bez uwzględnienia i-tego zadania osiągalny był bez niego,
                // przypisujemy zadanie do maszyny 1.
                maszyny[1].push_back(instancjaZadan[i - 1]);
            } else {
                // W przeciwnym przypadku zadanie musiało być wybrane (przypisane do maszyny 0),
                // odejmujemy jego czas od s.
                maszyny[0].push_back(instancjaZadan[i - 1]);
                s -= t;
            }
        }
        // Odwracamy kolejność zadań, aby zachować oryginalną kolejność.
        std::reverse(maszyny[0].begin(), maszyny[0].end());
        std::reverse(maszyny[1].begin(), maszyny[1].end());
    }
    else if (m == 3) {
        // Dla m==3 – zadanie jest znacznie bardziej złożone.
        // 1) Obliczamy sumę czasów zadań.
        int total = std::accumulate(
            instancjaZadan.begin(), instancjaZadan.end(),
            0, [](int acc, const DaneZad& z){ return acc + z.czas; }
        );

        int N = n, T = total;
        // 2) Tworzymy 3-wymiarowe DP: [0..N] x [0..T] x [0..T]
        // Używamy typu char – 0 oznacza false, 1 oznacza true.
        std::vector<std::vector<std::vector<char>>> DP(
            N + 1, std::vector<std::vector<char>>(T + 1, std::vector<char>(T + 1, 0))
        );
        DP[0][0][0] = 1;

        // 3) Wypełnienie DP: DP[i][s1][s2] – czy da się osiągnąć sumy s1 i s2 dla maszyn 0 i 1,
        // przy czym zadania, których nie przydzielimy do maszyn 0 lub 1, trafią do maszyny 2.
        for (int i = 1; i <= N; ++i) {
            int t = instancjaZadan[i - 1].czas;
            for (int s1 = 0; s1 <= T; ++s1) {
                for (int s2 = 0; s2 <= T; ++s2) {
                    if (DP[i - 1][s1][s2]) {
                        // Jeśli stan osiągalny był już bez i-tego zadania,
                        // to on nadal pozostaje.
                        DP[i][s1][s2] = 1;
                    } else {
                        char ok = 0;
                        // Opcja: i-te zadanie przypisane do maszyny 0.
                        if (s1 >= t && DP[i - 1][s1 - t][s2])
                            ok = 1;
                        // Opcja: i-te zadanie przypisane do maszyny 1.
                        if (!ok && s2 >= t && DP[i - 1][s1][s2 - t])
                            ok = 1;
                        // Jeśli któraś z opcji jest możliwa, ustawiamy DP[i][s1][s2]=1.
                        DP[i][s1][s2] = ok;
                    }
                }
            }
        }

        // 4) Wybór optymalnych sum s1 i s2 tak, by minimalizować maksymalne obciążenie:
        // minimalizujemy max(s1, s2, total - s1 - s2).
        int bestS1 = 0, bestS2 = 0;
        int bestLoad = std::numeric_limits<int>::max();
        for (int s1 = 0; s1 <= T; ++s1) {
            for (int s2 = 0; s2 + s1 <= T; ++s2) {
                if (DP[N][s1][s2]) {
                    int load = std::max({ s1, s2, total - s1 - s2 });
                    if (load < bestLoad) {
                        bestLoad = load;
                        bestS1 = s1;
                        bestS2 = s2;
                    }
                }
            }
        }

        // 5) Backtracking: odtwarzamy przydział zadań do poszczególnych maszyn.
        // Zadanie ustawiamy na:
        // - maszynę 0, jeśli stan DP wskazuje, że zostało przydzielone do maszyny 0,
        // - maszynę 1, jeśli przydzielone do maszyny 1,
        // - w przeciwnym wypadku (jeśli stan bez zmian był osiągalny) na maszynę 2.
        maszyny[0].clear();
        maszyny[1].clear();
        maszyny[2].clear();

        int s1 = bestS1, s2 = bestS2;
        for (int i = N; i > 0; --i) {
            int t = instancjaZadan[i - 1].czas;
            if (DP[i - 1][s1][s2]) {
                // Jeśli możliwe było osiągnięcie stanu bez i-tego zadania,
                // przypisujemy je do maszyny 2.
                maszyny[2].push_back(instancjaZadan[i - 1]);
            }
            else if (s1 >= t && DP[i - 1][s1 - t][s2]) {
                // Jeśli i-te zadanie musiało pójść do maszyny 0.
                maszyny[0].push_back(instancjaZadan[i - 1]);
                s1 -= t;
            } else {
                // W przeciwnym razie musi trafić na maszynę 1.
                maszyny[1].push_back(instancjaZadan[i - 1]);
                s2 -= t;
            }
        }
        // Odwracamy kolejność zadań dla każdej maszyny, aby zachować oryginalną sekwencję.
        for (int k = 0; k < 3; ++k) {
            std::reverse(maszyny[k].begin(), maszyny[k].end());
        }
    }
}
/****************************** PTAS ********************************* */

void Problem::ptas(double eps) {
    if(m!=2){return;}
    // Skopiuj i posortuj zadania malejąco wg. czasu
    std::vector<DaneZad> sorted = instancjaZadan;
    std::sort(sorted.begin(), sorted.end(), [](const DaneZad& a, const DaneZad& b) {
        return a.czas > b.czas;
    });

    // Ustal liczbę "dużych" zadań do rozpatrzenia – k = ceil(1/eps)
    int k = std::min((int)sorted.size(), (int)std::ceil(1.0 / eps));
    int bestMakespan = INT_MAX;
    std::vector<std::vector<DaneZad>> bestAssignment(2); // bestAssignment[0] dla maszyny 0, bestAssignment[1] dla maszyny 1

    // Przeglądamy wszystkie 2^k możliwości przydziału k największych zadań.
    long long limit = 1LL << k;
    for (long long mask = 0; mask < limit; ++mask) {
        int load0 = 0, load1 = 0;
        std::vector<std::vector<DaneZad>> candidate(2);

        // Przydziel duże zadania zgodnie z maską
        for (int i = 0; i < k; ++i) {
            if (mask & (1LL << i)) {
                candidate[1].push_back(sorted[i]);
                load1 += sorted[i].czas;
            } else {
                candidate[0].push_back(sorted[i]);
                load0 += sorted[i].czas;
            }
        }
        // Pozostałe zadania przydzielamy metodą LPT – czyli do maszyny, która aktualnie ma mniejsze obciążenie.
        for (size_t i = k; i < sorted.size(); ++i) {
            if (load0 <= load1) {
                candidate[0].push_back(sorted[i]);
                load0 += sorted[i].czas;
            } else {
                candidate[1].push_back(sorted[i]);
                load1 += sorted[i].czas;
            }
        }
        int currentMakespan = std::max(load0, load1);
        if (currentMakespan < bestMakespan) {
            bestMakespan = currentMakespan;
            bestAssignment = candidate;
        }
    }
    // Zapisujemy najlepszy przydział zadań.
    maszyny[0] = bestAssignment[0];
    maszyny[1] = bestAssignment[1];
}

/****************************** FPTAS ********************************* */
// Idea: zamiast budować DP na pełnych wartościach czasów (które mogą być duże),
// skalujemy czasy zadań. Niech K = (eps * p_max) / n, gdzie p_max to największy czas zadania,
// Następnie rozwiązujemy problem podziału (subset sum) przy użyciu DP, analogicznie do funkcji dynamicSearch.
void Problem::fptas(double eps) {
    if(m!=2){return;}
    n = instancjaZadan.size(); // liczba zadań
    int total = std::accumulate(instancjaZadan.begin(), instancjaZadan.end(), 0,
                                [](int acc, const DaneZad& zad) { return acc + zad.czas; });
    int p_max = 0;
    for (const auto& zad : instancjaZadan)
        p_max = std::max(p_max, zad.czas);

    // Obliczamy współczynnik skalowania
    double K = (eps * p_max) / static_cast<double>(n);

    // Obliczamy skalowane czasy zadań
    std::vector<int> scaled(n);
    for (int i = 0; i < n; ++i) {
        scaled[i] = static_cast<int>(instancjaZadan[i].czas / K);
    }

    // Wyznaczamy skalowaną "połowę" łącznego czasu – celem jest znalezienie podzbioru o sumie
    // nie przekraczającej (total/2)/K, ale jak najbliższej tej wartości.
    int halfScaled = static_cast<int>((total / 2) / K);

    // Budujemy tablicę DP o wymiarach (n+1) x (halfScaled+1)
    std::vector<std::vector<bool>> DP(n + 1, std::vector<bool>(halfScaled + 1, false));
    DP[0][0] = true;
    for (int i = 1; i <= n; ++i) {
        int t = scaled[i - 1];
        for (int s = 0; s <= halfScaled; ++s) {
            if (DP[i - 1][s])
                DP[i][s] = true;
            else if (s >= t && DP[i - 1][s - t])
                DP[i][s] = true;
            else
                DP[i][s] = false;
        }
    }

    int bestS = 0;
    for (int s = halfScaled; s >= 0; --s) {
        if (DP[n][s]) {
            bestS = s;
            break;
        }
    }

    maszyny[0].clear();
    maszyny[1].clear();
    int s = bestS;
    for (int i = n; i > 0; --i) {
        int t = scaled[i - 1];
        // Jeśli nie musieliśmy brać zadania, przypisujemy je do maszyny 1
        if (DP[i - 1][s]) {
            maszyny[1].push_back(instancjaZadan[i - 1]);
        } else {
            // W przeciwnym przypadku – zadanie wybrane, przypisujemy do maszyny 0
            maszyny[0].push_back(instancjaZadan[i - 1]);
            s -= t;
        }
    }
    std::reverse(maszyny[0].begin(), maszyny[0].end());
    std::reverse(maszyny[1].begin(), maszyny[1].end());
}

int Problem::maxSum() const {
    int maxVal = 0;
    for (const auto& maszyna : maszyny) {
        int suma = std::accumulate(maszyna.begin(), maszyna.end(), 0, [](int acc, const DaneZad& zad){ return acc + zad.czas;});
        if (suma > maxVal) maxVal = suma;
    }
    return maxVal;
}

void Problem::display() const {
    for (int i = 0; i < maszyny.size(); ++i) {
        std::cout << "Maszyna " << i + 1 << ": ";
        for (const auto& zad : maszyny[i]) {
            std::cout << zad.numer << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Maksymalna suma " << maxSum() << std::endl;
}