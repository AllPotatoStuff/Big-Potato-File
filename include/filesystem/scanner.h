#ifndef SCANNER_H__
#define SCANNER_H__

#include <iostream>
#include <string>
#include <vector>

/**
 * @struct ResultEndScan
 * @brief Structure to hold the result of scanning a directory.
 * @param totalMb The total size of files in the directory in megabytes.
 * @param fileCount The number of files in the directory.
 */
struct ResultEndScan {
  double totalMb;
  int fileCount;
  int folderCount;

  ResultEndScan() : totalMb(0.0), fileCount(0) {}
};

/**
 * @struct Node
 * @brief Structure to represent a node in the directory tree.
 * @param dir The directory path.
 * @param fileCount The number of files in the directory.
 * @param sizeMb The total size of files in the directory in megabytes.
 * @param left Pointer to the left child node.
 * @param right Pointer to the right child node.
 */
struct Node {
  std::string dir;
  int fileCount;
  double sizeMb;
  std::vector<Node *>
      children; // Vector to hold child nodes for subdirectories.

  Node() : dir(""), fileCount(0), sizeMb(0.0) {}
};

/**
 * @class Scanner
 * @brief Class to scan directories and build a directory tree.
 */
class Scanner {
private:
  Node *root; // Pointer to the root of the directory tree.
  // Private member functions to manage the directory tree.
  void insertNode(Node *&node, ResultEndScan &res);
  // Private member function to free the memory allocated for the directory
  // tree.
  void freeTree(Node *node);

public:
  Scanner();
  ~Scanner();
  /**
   * @brief Scans the specified directory and returns the result.
   * @param dir The directory path to scan. Defaults to "C:\\" if not specified
   * on Windows.
   * @return A ResultEndScan structure containing the total size and file count.
   */
  ResultEndScan PathScanner(std::string dir = "C:\\");
};

#endif // SCANNER_H__