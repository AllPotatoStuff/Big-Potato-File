#ifndef ARGUMENTPARSER_H__
#define ARGUMENTPARSER_H__

#include <algorithm>
#include <config.h>
#include <format>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

struct AppOptions {
  Tools tool;
};

class ArgumentParser {
private:
  std::string programmeName;

  void printUsage() const {
    std::cerr << "\n\nUsage: " << programmeName << " -t <tool>\n\n"
              << "Options:\n"
              << "  -t, --tools Tool that you want to use (needed)\n"
              << "    existing tools: scan\n"
              << "  -h, --help  Help\n";
  }

  void invalidArgument(std::string errorReason) const;

public:
  AppOptions parse(int argc, char *argv[]);
};

#endif // ARGUMENTPARSER_H__