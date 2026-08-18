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

      auto it = std::find(TOOLS.begin(), TOOLS.end(), argv[i + 1]);

      if (it != TOOLS.end()) {
        options.tool = argv[i + 1];
      } else {
        invalidArgument(
            std::format("The tool {} does not exist.", argv[i + 1]));
      }
    } else if (arg == "-h" || arg == "-help") {
      printUsage();
      std::exit(0);
    }
  }

  return options;
}
