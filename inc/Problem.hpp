#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <iostream>
#include <algorithm>  // do next_permutation
#include <limits>     // dla std::numeric_limits
#include <numeric>
#include <bits/stdc++.h>
#include "Struct.hpp"
class Problem {
    private:
        int n;  // liczba zadań
        int m;  // liczba maszyn
        std::vector<std::vector<DaneZad>> maszyny;  // m wektorów dla maszyn
        std::vector<DaneZad> instancjaZadan;
    
    public:
        Problem(const std::vector<DaneZad>& dane, int liczbaZadan, int liczbaMaszyn);
        void clearMachines();
        void lsa();  // rozdziela dane do maszyn
        void lpt();
        int maxSum() const;  // maksymalna suma wartości na maszynie
        void display() const;  // wyświetlanie danych maszyn
        void bruteSearch();
        void dynamicSearch();
        void ptas(double eps);
        void fptas(double eps);
    };

#endif // PROBLEM_H