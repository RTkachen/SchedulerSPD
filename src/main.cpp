#include "Zadanie.hpp"
#include "Problem.hpp"

// Funkcja pomocnicza do mierzenia czasu działania
void measureExecutionTime(const std::string& label, const std::function<void()>& func) {
    auto start = std::chrono::high_resolution_clock::now();
    
    func(); // Wykonanie przekazanej funkcji

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    
    std::cout << "Czas wykonania (" << label << "): "
              << std::fixed << std::setprecision(7) << duration.count() << " s" << std::endl;
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

    problem.lsa();
    problem.display();
    problem.lpt();
    problem.display();
    

    return 0;
}