#ifndef ARGUMENTPARSER_H__
#define ARGUMENTPARSER_H__

#include <algorithm>
#include <config.h>
#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <filesystem>

struct ScanOptions {
  std::string path;

  std::optional<std::string> validate() const;
};

using ToolOptions = std::variant<ScanOptions>;

struct AppOptions {
  Tools tool;
  ToolOptions options;
};

class ArgumentParser {
private:
  std::string programmeName;

  void printUsage() const {
    std::cerr << "Options:\n"
              << "  -t, --tools Tool that you want to use (needed)\n"
              << "    existing tools: scan\n"
              << "\n\nUsage: " << programmeName << " -t <tool>\n\n"
              << "  Scan: \n  needs a target folder -p, --path"
              << "  -h, --help  Help\n";
  }

  void invalidArgument(std::string errorReason) const;
  ScanOptions parseScanOptions(int argc, char *argv[]);

public:
  AppOptions parse(int argc, char *argv[]);
};

#endif // ARGUMENTPARSER_H__