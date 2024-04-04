#include "doctest.h"

#include "../include/logfmtxx.hpp"


struct dummy_clock {
  static std::chrono::system_clock::time_point now() {
    return std::chrono::system_clock::from_time_t(978307200);
  }
};

TEST_CASE("it should format log records with logfmt format") {
  auto logger = logfmtxx::logger<dummy_clock>{
    [](const std::string& message) {
      CHECK(message == "time=\"Mon Jan 01 01:00:00 2001\" level=info message=\"hello\" key1=\"value1\" key2=\"value2\"");
    },
    logfmtxx::field{"key1", "value1"}
  };

  logger.log(
    logfmtxx::level::info,
    "hello",
    logfmtxx::field{"key2", "value2"}
  );
}
