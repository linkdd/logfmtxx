#include "doctest.h"

#include "../include/logfmtxx.hpp"


TEST_CASE("it should serialize strings") {
  std::string value = "hello";
  auto result = logfmtxx::details::serialize(value);
  CHECK(result == "\"hello\"");
}

TEST_CASE("it should escape quotes") {
  std::string value = "hello \"world\"";
  auto result = logfmtxx::details::serialize(value);
  CHECK(result == "\"hello \\\"world\\\"\"");
}

TEST_CASE("it should serialize types with an as_string() method") {
  struct serializable {
    std::string as_string() const {
      return "serializable";
    }
  };

  auto value = serializable{};
  auto result = logfmtxx::details::serialize(value);
  CHECK(result == "\"serializable\"");
}

TEST_CASE("it should serialize arithmetic types") {
  auto i = 42;
  auto result = logfmtxx::details::serialize(i);
  CHECK(result == "42");

  auto d = 3.14;
  result = logfmtxx::details::serialize(d);
  CHECK(result == "3.140000");
}

TEST_CASE("it should serialize time points") {
  auto now = std::chrono::system_clock::from_time_t(978307200);
  auto result = logfmtxx::details::serialize(now);
  CHECK(result == "\"Mon Jan 01 01:00:00 2001\"");
}

TEST_CASE("it should serialize log levels") {
  auto level = logfmtxx::level::info;
  auto result = logfmtxx::details::serialize(level);
  CHECK(result == "info");
}
