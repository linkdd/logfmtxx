#pragma once

#include <type_traits>
#include <concepts>

#include <functional>
#include <utility>
#include <chrono>
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>

namespace logfmtxx {
  enum class level {
    debug,
    info,
    warn,
    error
  };

  namespace details {
    template <typename T>
    concept as_string_trait = requires(T t) {
      { t.as_string() } -> std::same_as<std::string>;
    };

    template <typename T>
    concept serializable_trait = (
      std::is_convertible_v<T, std::string>
      || as_string_trait<T>
      || std::is_arithmetic_v<T>
      || std::is_same_v<T, std::chrono::system_clock::time_point>
      || std::is_same_v<T, level>
    );

    template <serializable_trait T>
    std::string serialize(const T& value) {
      if constexpr (std::is_convertible_v<T, std::string>) {
        auto stream = std::ostringstream{};
        stream << std::quoted(value);
        return stream.str();
      }
      else if constexpr (as_string_trait<T>) {
        auto stream = std::ostringstream{};
        stream << std::quoted(value.as_string());
        return stream.str();
      }
      else if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(value);
      }
      else if constexpr (std::is_same_v<T, std::chrono::system_clock::time_point>) {
        auto time = std::chrono::system_clock::to_time_t(value);
        auto stream = std::ostringstream{};
        stream << std::put_time(std::gmtime(&time), "%Y-%m-%dT%H:%M:%SZ");
        return stream.str();
      }
      else if constexpr (std::is_same_v<T, level>) {
        switch (value) {
          case level::debug: return "debug";
          case level::info:  return "info";
          case level::warn:  return "warn";
          case level::error: return "error";
        }
      }

      return "";
    }

    struct record {
      level lvl;
      std::chrono::system_clock::time_point ts;
      std::string msg;
      std::vector<std::pair<std::string, std::string>> extras;
    };
  }

  template <details::serializable_trait T>
  struct field {
    std::string key;
    T value;
  };

  template <typename clock_type = std::chrono::system_clock>
  class logger {
    public:
      using printer_type = std::function<void(const std::string&)>;

    public:
      template <typename... Args>
      logger(
        printer_type printer,
        field<Args>... fields
      ) : m_printer(printer) {
        (m_extras.push_back({fields.key, details::serialize(fields.value)}), ...);
      }

      template <typename... Args>
      void log(level level, const std::string& message, field<Args>... fields) {
        auto record = details::record{
          .lvl = level,
          .ts = clock_type::now(),
          .msg = message
        };

        for (const auto& [key, value] : m_extras) {
          record.extras.push_back({key, value});
        }

        (record.extras.push_back({fields.key, details::serialize(fields.value)}), ...);

        m_printer(format(record));
      }

    private:
      std::string format(const details::record& record) {
        auto stream = std::ostringstream{};

        stream << "time=" << details::serialize(record.ts) << " ";
        stream << "level=" << details::serialize(record.lvl) << " ";
        stream << "message=" << std::quoted(record.msg);

        for (const auto& [key, value] : record.extras) {
          stream << " " << key << "=" << value;
        }

        return stream.str();
      }

    private:
      printer_type m_printer;
      std::vector<std::pair<std::string, std::string>> m_extras;
  };
}
