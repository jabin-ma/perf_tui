//
// Created by Jabin Ma on 2022/11/30.
//
#include "strings_utils.h"
#include <fstream>
#include <optional>
#include <string>
#include <vector>

using namespace std;
namespace sysmonitor {
  string StringPrintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int size_s = vsnprintf(nullptr, 0, format, args);
    va_end(args);
    if (size_s <= 0) {
      throw runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s) + 1;// Extra space for '\0'.
    auto buf = make_unique<char[]>(size);
    va_start(args, format);
    vsnprintf(buf.get(), size, format, args);
    va_end(args);
    return {buf.get(), buf.get() + size - 1};// Without the '\0'.
  }

  bool StartsWith(std::string_view s, std::string_view prefix) {
    return s.substr(0, prefix.size()) == prefix;
  }

  bool StartsWith(std::string_view s, char prefix) {
    return !s.empty() && s.front() == prefix;
  }

  bool StartsWithIgnoreCase(std::string_view s, std::string_view prefix) {
    return s.size() >= prefix.size() && strncasecmp(s.data(), prefix.data(), prefix.size()) == 0;
  }

  bool EndsWith(std::string_view s, std::string_view suffix) {
    return s.size() >= suffix.size() && s.substr(s.size() - suffix.size(), suffix.size()) == suffix;
  }

  bool EndsWith(std::string_view s, char suffix) {
    return !s.empty() && s.back() == suffix;
  }

  bool EndsWithIgnoreCase(std::string_view s, std::string_view suffix) {
    return s.size() >= suffix.size() &&
           strncasecmp(s.data() + (s.size() - suffix.size()), suffix.data(), suffix.size()) == 0;
  }

  bool ReadFileToString(const std::string &path, std::string *stringOut)
  {
    std::ifstream inFile(path.c_str());
    if (inFile.fail())
    {
      return false;
    }

    inFile.seekg(0, std::ios::end);
    stringOut->reserve(static_cast<std::string::size_type>(inFile.tellg()));
    inFile.seekg(0, std::ios::beg);

    stringOut->assign(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>());
    return !inFile.fail();
  }

  std::vector<std::string> Split(const std::string& s,
                                 const std::string& delimiters) {
    std::vector<std::string> result;

    size_t base = 0;
    size_t found;
    while (true) {
      found = s.find_first_of(delimiters, base);
      result.push_back(s.substr(base, found - base));
      if (found == s.npos) break;
      base = found + 1;
    }

    return result;
  }

  std::optional<std::vector<uint32_t>> readNumbersFromFile(const std::string &path) {
      std::string data;

      if (!ReadFileToString(path, &data)) return {};

      auto strings = Split(data, " \n");
      std::vector<uint32_t> ret;
      for (const auto &s : strings) {
        if (s.empty()) continue;
        uint32_t n;
        if (!ParseUint(s, &n)) return {};
        ret.emplace_back(n);
      }
      return ret;
    }
}// namespace sysmonitor