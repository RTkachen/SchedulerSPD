#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <iostream>
#include <algorithm>  // do next_permutation
#include <limits>     // dla std::numeric_limits
#include <numeric>
#include <bits/stdc++.h>
class Problem {
    private:
        int n;  // liczba zadań
        int m;  // liczba maszyn
        std::vector<std::vector<int>> maszyny;  // m wektorów dla maszyn
        std::vector<int> instancjaZadan;
        void clearMachines();
    
    public:
        Problem(const std::vector<int>& dane, int liczbaZadan, int liczbaMaszyn);
    
        void lsa();  // rozdziela dane do maszyn
        void lpt();
        int maxSum() const;  // maksymalna suma wartości na maszynie
        void display() const;  // wyświetlanie danych maszyn
    };

#endif // PROBLEM_H