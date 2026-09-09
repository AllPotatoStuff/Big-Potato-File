#ifndef FILEINFO_H__
#define FILEINFO_H__

#include <cstdint>
#include <optional>
#include <string>

/**
 * @struct FileInfo
 * @brief Represent an entry (file or folder) ready to be indexed.
 */
struct FileInfo {
  std::string path;
  std::string name;
  uint64_t size = 0;      // byte
  int64_t modifiedAt = 0; // timestamp Unix
  int64_t createdAt = 0;
  bool isDirectory = false;
  std::optional<std::string> hash; // only after explicit hashing
  uint8_t flags = 0; // bitfield: hidden, system, readonly, symlink...
};

#endif // FILEINFO_H__