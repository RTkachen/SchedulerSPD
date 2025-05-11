#ifndef ZADANIE_HPP
#define ZADANIE_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Struct.hpp"
class Zadanie {
private:
    int n;  // liczba zadań
    int m;  // liczba maszyn
    std::vector<DaneZad> instantion;  // dane zadań
    void readFromFile(const std::string& filename);  // wczytywanie danych z pliku

public:
    Zadanie(const std::string& filename);  // konstruktor

    // Gettery
    int getN() const;
    int getM() const;
    const std::vector<DaneZad>& getInstantion() const;

    void displayFile();  // wyświetlanie wczytanych danych
};

#endif // ZADANIE_HPP