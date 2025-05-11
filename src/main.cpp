#include "Zadanie.hpp"
#include "Problem.hpp"

// Funkcja pomocnicza do mierzenia czasu działania
void measureExecutionTime(Problem& problem, const std::function<void()>& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end   = std::chrono::high_resolution_clock::now();

    auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "="<< us << "*0,000001"<< '\n' ;
    std::cout << problem.maxSum() << '\n';
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Użycie: " << argv[0] << " <ścieżka do pliku>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    Zadanie zadanie(filename);

    if (zadanie.getM() < 2 || zadanie.getM() > 3) {
        return 1;  // błędna liczba maszyn
    }

    Problem problem(zadanie.getInstantion(), zadanie.getN(), zadanie.getM());

     // kolejne algorytmy, każdy czyści maszyny przed startem
     measureExecutionTime(problem, [&]{ problem.clearMachines(); problem.lsa();        });
     measureExecutionTime(problem, [&]{ problem.clearMachines(); problem.lpt();        });
     measureExecutionTime(problem, [&]{ problem.clearMachines(); problem.bruteSearch(); });
     measureExecutionTime(problem, [&]{ problem.clearMachines(); problem.dynamicSearch(); });
     measureExecutionTime(problem, [&]{ problem.clearMachines(); problem.ptas(0.05);    });
     measureExecutionTime(problem, [&]{ problem.clearMachines(); problem.fptas(0.05);  });
 
    return 0;
}