#include <iostream>
#include <iomanip>
#include "include/filesystem/scanner.h"

int main() {
    Scanner scanner;

    std::string dir = "f:\\";

    std::cout << "Scan en cours du dossier : " << dir << " ...\n";

    ResultEndScan resultat = scanner.PathScanner(dir);

    std::cout << resultat.fileCount << "\n";

    std::cout << std::fixed << std::setprecision(2);
    std::cout << resultat.totalMb << " Mo\n";

    return 0;
}
