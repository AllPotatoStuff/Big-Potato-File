#include "argumentparser.h"

void ArgumentParser::printUsage() const {
  std::cerr << "Usage: " << m_programmeName << " -t <tool> [options]\n"
            << "       " << m_programmeName << " <tool> [options]\n\n";

  std::cerr << "Global options:\n"
            << "  -t, --tool <tool>  Tool to use (required)\n"
            << "  -h, --help         Show this help\n\n";

  std::cerr << "Available tools:\n";
  for (const auto &t : TOOLS) {
    std::cerr << "  " << t.name << " - " << t.description << "\n"
              << "      " << t.args << "\n";
  }
}

void ArgumentParser::invalidArgument(std::string errorReason) const {
  std::cerr << "Error : " << errorReason << "\n";
  printUsage();
  std::exit(1);
}

ScanOptions ArgumentParser::parseScanOptions(int argc, char *argv[]) {
  ScanOptions scanOptions;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "-p" || arg == "--path") {
      if (i + 1 >= argc) {
        invalidArgument("Scan tool needs a path");
      }
      std::string stringPath = argv[++i];
      std::filesystem::path p(stringPath);

      if (!std::filesystem::exists(p)) {
        invalidArgument(
            std::format("Need an existing folder path : {}", p.string()));
      }

      if (std::filesystem::is_directory(p)) {
        scanOptions.path = p.string();
      } else {
        invalidArgument(std::format("Need a folder path: {}", p.string()));
      }
    }
  }

  return scanOptions;
}
AppOptions ArgumentParser::parse(int argc, char *argv[]) {
  m_programmeName = argv[0];
  AppOptions options;
  bool toolSet = false;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "-h" || arg == "--help") {
      printUsage();
      std::exit(0);
    }

    bool isToolFlag = (arg == "-t" || arg == "--tool");
    bool isBareToolName = toolsFromString(arg).has_value();

    if (isToolFlag || isBareToolName) {
      std::string toolArg;

      if (isToolFlag) {
        if (i + 1 >= argc) {
          invalidArgument("Tool option is a necessary argument.");
        }
        toolArg = argv[++i];
      } else {
        toolArg = arg;
      }

      auto tool = toolsFromString(toolArg);
      if (!tool.has_value()) {
        invalidArgument(std::format("The tool {} does not exist.", toolArg));
      }

      options.tool = *tool;
      toolSet = true;

      switch (options.tool) {
      case Tools::Scan: {
        ScanOptions scanOptions = parseScanOptions(argc, argv);
        if (auto missing = scanOptions.validate()) {
          invalidArgument(std::format(
              "The argument \"{}\" is missing to use the tool \"{}\".",
              *missing, toolArg));
        }
        options.options = scanOptions;
      } break;
      default:
        invalidArgument("There is no default tool");
        break;
      }
    }
  }

  if (!toolSet) {
    invalidArgument("Tool option is a necessary argument.");
  }

  return options;
}

std::optional<std::string> ScanOptions::validate() const {
  if (path.empty()) {
    return "path";
  }
  return std::nullopt;
}
