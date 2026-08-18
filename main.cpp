#include "argumentparser.h"
#include <iostream>

int main(int argc, char *argv[]) {
  ArgumentParser parser;

  AppOptions options = parser.parse(argc, argv);

  std::cout << "Success " << options.tool << " exists" << std::endl;
  return 0;
}