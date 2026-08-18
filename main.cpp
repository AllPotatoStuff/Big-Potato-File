#include "argumentparser.h"
#include <config.h>
#include <iostream>

int main(int argc, char *argv[]) {
  ArgumentParser parser;

  AppOptions options = parser.parse(argc, argv);

  switch (options.tool) {
  case Tools::Scan:
    std::cerr << "Scan Not Implemented" << std::endl;
    break;
  default:
    std::cerr << "No default" << std::endl;
    break;
  }

  return 0;
}