#include "Zadanie.hpp"


void Zadanie::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Nie można otworzyć pliku!" << std::endl;
        return;
    }

    file >> n >> m;

    if (m < 2 || m > 3) {
        std::cerr << "Liczba maszyn musi być 2 lub 3. Podano: " << m << std::endl;
        file.close();
        return;
    }

    int value;
    for (int i = 0; i < n; ++i) {
        if (file >> value) {
            DaneZad zadanie;
            zadanie.czas = value;
            zadanie.numer = i + 1;  // numeracja od 1
            instantion.push_back(zadanie);
        } else {
            std::cerr << "Błąd podczas wczytywania danych dla zadania nr " << i + 1 << std::endl;
            file.close();
            return;
        }
    }

    file.close();
}

Zadanie::Zadanie(const std::string& filename) {
    readFromFile(filename);
}

int Zadanie::getN() const { return n; }
int Zadanie::getM() const { return m; }

const std::vector<DaneZad>& Zadanie::getInstantion() const {
    return instantion;
}

void Zadanie::displayFile() {
    std::cout << "Liczba zadań (n): " << n << ", liczba maszyn (m): " << m << std::endl;
    std::cout << "Wczytane dane zadań:" << std::endl;
    for (const auto& zad : instantion) {
        std::cout << "Zadanie " << zad.numer << ": " << zad.czas << std::endl;
    }
}