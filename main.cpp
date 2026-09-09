#include "argumentparser.h"
#include "filesystem/scanner.h"
#include <iomanip>
#include <iostream>

void runScan(const ScanOptions &options) {
  Scanner scanner;
  std::string path = options.path;

  std::cout << "Scaning folder : " << path << " ...\n";

  ResultEndScan resultat = scanner.PathScannerSingleThread(path);

  std::cout << "Files: " << resultat.fileCount << "\n";
  std::cout << "Directories: " << resultat.folderCount << "\n";

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Size: " << resultat.totalMb << " Mo\n";
}

void runIndex(const IndexOptions &options) {
  std::cout << "INDEX" << std::endl;
  ThreadPool pool(std::thread::hardware_concurrency());
  ConcurrentQueue<FileInfo> queue;
  Indexer indexer(queue, "index.db");

  Scanner scanner;

  indexer.start();

  ResultEndScan res = scanner.PathScannerMultiThread(options.path, pool, queue);

  indexer.join();

  std::cout << res.fileCount << " files, " << res.folderCount
            << " folder, " << res.totalMb << " Mo, " << indexer.indexedCount()
            << " indexed entry\n";
}

int main(int argc, char *argv[]) {
  ArgumentParser parser;

  AppOptions options = parser.parse(argc, argv);

  switch (options.tool) {
  case Tools::Scan:
    runScan(std::get<ScanOptions>(options.options));
    break;
  case Tools::Index:
    runIndex(std::get<IndexOptions>(options.options));
    break;
  default:
    std::cerr << "No default" << std::endl;
    break;
  }

  return 0;
}
