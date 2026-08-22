#include "argumentparser.h"
#include "filesystem/scanner.h"
#include <iomanip>
#include <iostream>

void runScan(const ScanOptions &options) {
  Scanner scanner;
  std::string path = options.path;

  std::cout << "Scaning folder : " << path << " ...\n";

  ResultEndScan resultat = scanner.PathScanner(path);

  std::cout << resultat.fileCount << "\n";

  std::cout << std::fixed << std::setprecision(2);
  std::cout << resultat.totalMb << " Mo\n";
}

int main(int argc, char *argv[]) {
  ArgumentParser parser;

  AppOptions options = parser.parse(argc, argv);

  switch (options.tool) {
  case Tools::Scan:
    runScan(std::get<ScanOptions>(options.options));
    break;
  default:
    std::cerr << "No default" << std::endl;
    break;
  }

  return 0;
}
