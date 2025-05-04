#include "Problem.hpp"

Problem::Problem(const std::vector<int>& dane, int liczbaZadan, int liczbaMaszyn)
    : n(liczbaZadan), m(liczbaMaszyn), instancjaZadan(dane) 
{
    maszyny.resize(m);  // tylko puste wektory, bez danych
}

void Problem::clearMachines() {
    for (auto& maszyna : maszyny) {
        maszyna.clear();
    }
}

void Problem::lsa() {
    clearMachines();

    for (int i = 0; i < n; ++i) {
        int indeksMaszyny = i % m;
        maszyny[indeksMaszyny].push_back(instancjaZadan[i]);
    }
}

void Problem::lpt() {
    clearMachines();

    // Stwórz lokalną kopię i posortuj malejąco
    std::vector<int> zadania = instancjaZadan;
    std::sort(zadania.begin(), zadania.end(), std::greater<int>());

    std::vector<int> sumaMaszyn(m, 0);
    //petla idaca po kazdym elemencie wektora zadania
    for (int p : zadania) {
        //roznica pomiedzy iteratorami powoduje ze zwracany jest indeks a nie iterator
        int indeks = std::min_element(sumaMaszyn.begin(), sumaMaszyn.end()) - sumaMaszyn.begin();
        //wrzucamy do maszyny zadanie o czasie p
        maszyny[indeks].push_back(p);
        //dodajemy p do sumy czasu na maszynie do ktorej dodano p
        sumaMaszyn[indeks] += p;
    }
}

int Problem::maxSum() const {
    int maxVal = 0;
    for (const auto& maszyna : maszyny) {
        int suma = std::accumulate(maszyna.begin(), maszyna.end(), 0);
        if (suma > maxVal) maxVal = suma;
    }
    return maxVal;
}

void Problem::display() const {
    for (int i = 0; i < maszyny.size(); ++i) {
        std::cout << "Maszyna " << i + 1 << ": ";
        for (int val : maszyny[i]) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Maksymalna suma " << maxSum() << std::endl;
}