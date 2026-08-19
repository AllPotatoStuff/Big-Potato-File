#include "scanner.h"
#include <filesystem>

namespace fs = std::filesystem;

Scanner::Scanner() : root(nullptr) {}

Scanner::~Scanner()
{
    freeTree(root);
}

void Scanner::insertNode(Node*& node, ResultEndScan& res)
{
    if (node == nullptr) return;

    try {
        fs::path p(node->dir);
        std::cout << "Scanning directory: " << p << "\n";
        if (!fs::exists(p) || !fs::is_directory(p)) return;

        Node* currentTail = nullptr;  // Pointer to keep track of the last child node added for sibling linking

        // Iterate through the directory entries
        for (const auto& entry : fs::directory_iterator(p)) {
            if (fs::is_regular_file(entry.path())) {
                // if is a regular file, update the current node's file count and size
                node->fileCount++;
                node->sizeMb += static_cast<double>(fs::file_size(entry.path())) / (1024.0 * 1024.0); // Convert bytes to megabytes
            }
            else if (fs::is_directory(entry.path())) {
                // if is a directory, create a new child node and link it as a sibling
                Node* childNode = new Node();
                childNode->dir = entry.path().string(); // Set the directory path for the child node

                // Link the new child node to the current node's left child or as a sibling to the last added child
                if (node->left == nullptr)
                    node->left = childNode; // first child
                else
                    currentTail->right = childNode; // link as a sibling to the last added child
                currentTail = childNode;
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        // Handle filesystem errors (e.g., permission denied, path not found)
        std::cerr << "Access denied or error: " << e.what() << "\n";
    }

    // Update the end result with the current node's size and file count
    res.totalMb += node->sizeMb;
    res.fileCount += node->fileCount;


    insertNode(node->left, res);
    insertNode(node->right, res);

}

ResultEndScan Scanner::PathScanner(std::string dir)
{
    // Free the existing directory tree if it exists
    if (root != nullptr) {
        freeTree(root);
        root = nullptr;
    }

    dir = dir.empty() ? "C:\\" : dir; // Default to "C:\\\\" if no directory is provided.

    ResultEndScan res;

    root = new Node(); // Create a new node for the root of the directory tree.
    root->dir = dir; // Set the directory path for the root node.

    insertNode(root, res);

    return res;
}

void Scanner::freeTree(Node* node)
{
    if (node == nullptr) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}
