#include <iostream>
#include <thread>
#include <chrono>

#include "doctest.h"
#include "nanobench.h"

#include "../include/logfmtxx.hpp"

using std::chrono::operator ""ms;


TEST_SUITE("benchmarks") {
  TEST_CASE("fast") {
    auto logger_noctx = logfmtxx::logger{
      [](const std::string& message) {},
    };

    auto logger_ctx = logfmtxx::logger{
      [](const std::string& message) {},
      logfmtxx::field{"key1", "value1"},
    };

    ankerl::nanobench::Bench()
      .minEpochIterations(1024)
      .run("logger.info with context (fast)", [&] {
        logger_ctx.info("hello", logfmtxx::field{"key2", "value2"});
      });

    ankerl::nanobench::Bench()
      .minEpochIterations(1024)
      .run("logger.info without context (fast)", [&] {
        logger_noctx.info("hello");
      });
  }

  TEST_CASE("slow") {
    auto logger_noctx = logfmtxx::logger{
      [](const std::string& message) {
        std::cerr << message << std::endl;
      },
    };

    auto logger_ctx = logfmtxx::logger{
      [](const std::string& message) {
        std::cerr << message << std::endl;
      },
      logfmtxx::field{"key1", "value1"},
    };

    ankerl::nanobench::Bench()
      .minEpochIterations(256)
      .run("logger.info with context (slow)", [&] {
        logger_ctx.info("hello", logfmtxx::field{"key2", "value2"});
      });

    ankerl::nanobench::Bench()
      .minEpochIterations(256)
      .run("logger.info without context (slow)", [&] {
        logger_noctx.info("hello");
      });
  }
}
