#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

enum class Tools { Scan };

inline const std::unordered_map<std::string, Tools> TOOL_NAMES = {
    {"scan", Tools::Scan},
};

inline std::optional<Tools> toolsFromString(const std::string &name) {
  auto it = TOOL_NAMES.find(name);
  if (it != TOOL_NAMES.end()) {
    return it->second;
  }
  return std::nullopt;
}

inline std::string toolsToString(Tools tool) {
  switch (tool) {
  case Tools::Scan:
    return "scan";
  }
  return "unknown";
}