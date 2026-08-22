#pragma once
#include <algorithm>
#include <optional>
#include <string>
#include <vector>

enum class Tools { Scan };

struct ToolInfo {
  Tools tool;
  std::string name;
  std::string description;
  std::string args;
};

inline const std::vector<ToolInfo> TOOLS = {
    {Tools::Scan, "scan",
     "Scan a target folder and give back generic informations",
     "-p, --path <folder>  Target folder (required)"},
};

inline std::optional<Tools> toolsFromString(const std::string &name) {
  auto it = std::find_if(TOOLS.begin(), TOOLS.end(),
                         [&](const ToolInfo &t) { return t.name == name; });
  if (it != TOOLS.end()) {
    return it->tool;
  }
  return std::nullopt;
}

inline std::string toolsToString(Tools tool) {
  auto it = std::find_if(TOOLS.begin(), TOOLS.end(),
                         [&](const ToolInfo &t) { return t.tool == tool; });
  return it != TOOLS.end() ? it->name : "unknown";
}

inline const ToolInfo *toolInfoFor(Tools tool) {
  auto it = std::find_if(TOOLS.begin(), TOOLS.end(),
                         [&](const ToolInfo &t) { return t.tool == tool; });
  return it != TOOLS.end() ? &(*it) : nullptr;
}