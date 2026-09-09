#ifndef ARGUMENTPARSER_H__
#define ARGUMENTPARSER_H__

#include "toolconfig.h"
#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>

struct ScanOptions {
  std::string path;

  std::optional<std::string> validate() const;
};

struct IndexOptions {
  std::string path;

  std::optional<std::string> validate() const;
};

using ToolOptions = std::variant<ScanOptions, IndexOptions>;

struct AppOptions {
  Tools tool;
  ToolOptions options;
};

class ArgumentParser {
private:
  std::string m_programmeName;

  void printUsage() const;

  void invalidArgument(std::string errorReason) const;
  ScanOptions parseScanOptions(int argc, char *argv[]);
  IndexOptions parseIndexOptions(int argc, char *argv[]);

public:
  AppOptions parse(int argc, char *argv[]);
};

#endif // ARGUMENTPARSER_H__