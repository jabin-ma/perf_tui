//
// Created by Jabin Ma on 2022/11/30.
//

#ifndef PERF_TUI_STRINGS_UTILS_H
#define PERF_TUI_STRINGS_UTILS_H
#include <string>
#include <optional>
namespace sysmonitor {
  std::string StringPrintf(const char *format, ...) __attribute__((format(printf, 1, 2)));

  // Tests whether 's' starts with 'prefix'.
  bool StartsWith(std::string_view s, std::string_view prefix);
  bool StartsWith(std::string_view s, char prefix);
  bool StartsWithIgnoreCase(std::string_view s, std::string_view prefix);

  // Tests whether 's' ends with 'suffix'.
  bool EndsWith(std::string_view s, std::string_view suffix);
  bool EndsWith(std::string_view s, char suffix);
  bool EndsWithIgnoreCase(std::string_view s, std::string_view suffix);
  bool ReadFileToString(const std::string &path, std::string *stringOut);
  std::vector<std::string> Split(const std::string& s,
                                 const std::string& delimiters);
  std::optional<std::vector<uint32_t>> readNumbersFromFile(const std::string &path);

  template <typename T>
  bool ParseUint(const char* s, T* out, T max = std::numeric_limits<T>::max(),
                 bool allow_suffixes = false) {
    static_assert(std::is_unsigned<T>::value, "ParseUint can only be used with unsigned types");
    while (isspace(*s)) {
      s++;
    }

    if (s[0] == '-') {
      errno = EINVAL;
      return false;
    }

    // This is never out of bounds. If string is zero-sized, s[0] == '\0'
    // so the second condition is not checked. If string is "0",
    // s[1] will compare against the '\0'.
    int base = (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) ? 16 : 10;
    errno = 0;
    char* end;
    unsigned long long int result = strtoull(s, &end, base);
    if (errno != 0) return false;
    if (end == s) {
      errno = EINVAL;
      return false;
    }
    if (*end != '\0') {
      const char* suffixes = "bkmgtpe";
      const char* suffix;
      if ((!allow_suffixes || (suffix = strchr(suffixes, tolower(*end))) == nullptr) ||
          __builtin_mul_overflow(result, 1ULL << (10 * (suffix - suffixes)), &result)) {
        errno = EINVAL;
        return false;
      }
    }
    if (max < result) {
      errno = ERANGE;
      return false;
    }
    if (out != nullptr) {
      *out = static_cast<T>(result);
    }
    return true;
  }

  template <typename T>
  bool ParseUint(const std::string& s, T* out, T max = std::numeric_limits<T>::max(),
                 bool allow_suffixes = false) {
    return ParseUint(s.c_str(), out, max, allow_suffixes);
  }

}// namespace sysmonitor
#endif//PERF_TUI_STRINGS_UTILS_H
