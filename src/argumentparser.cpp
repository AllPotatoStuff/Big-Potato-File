#include "argumentparser.h"

void ArgumentParser::invalidArgument(std::string errorReason) const {
  std::cerr << "Error : " << errorReason << "\n";
  printUsage();
  std::exit(1);
}

AppOptions ArgumentParser::parse(int argc, char *argv[]) {
  programmeName = argv[0];
  AppOptions options;
  std::stringstream ss;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "-t" || arg == "--tool") {
      if (i + 1 >= argc) {
        invalidArgument("Tool option is a necessery argument.");
      }

      std::string toolArg = argv[i + 1];
      auto tool = toolsFromString(toolArg);

      if (tool.has_value()) {
        options.tool = tool.value();
      } else {
        invalidArgument(std::format("The tool {} does not exist.", toolArg));
      }
    } else if (arg == "-h" || arg == "-help") {
      printUsage();
      std::exit(0);
    }
  }

  return options;
}
