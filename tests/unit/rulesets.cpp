#include "doctest/doctest.h"
#include "landlockpp/bitmasks.hpp"
#include "landlockpp/landlockpp.hpp"
#include "landlockpp/ruleset.hpp"
#include <system_error>

using namespace landlockpp;

TEST_CASE("ruleset creation") {
  SUBCASE("with no flags returns ENOMSG") {
    auto ruleset_e { Ruleset::create({}) };
    REQUIRE( ! ruleset_e.has_value());
    INFO(ruleset_e.error().message());
    CHECK(ruleset_e.error() == std::errc::no_message);
  }
  SUBCASE("with unsupported bit returns errc not_supported") {
    auto ruleset_e { Ruleset::create({.fs = static_cast<FsAccess>(1L << 63)}) };
    REQUIRE( ! ruleset_e.has_value());
    INFO(ruleset_e.error().message());
    CHECK(ruleset_e.error() == std::errc::not_supported);
  }
}

TEST_CASE("adding rules") {
  SUBCASE("adding unhandled access returns EINVAL") {
    auto ruleset_e { Ruleset::create({.fs = FsAccess::ReadFile}) };
    if(! ruleset_e.has_value()) {
      INFO("Ruleset::create: ", ruleset_e.error().message());
    }
    REQUIRE(ruleset_e.has_value());
    auto ruleset { std::move(ruleset_e.value()) };
    auto addp_e {ruleset.add_path("/etc/passwd", FsAccess::Write)};
    INFO("add_path: ", addp_e.error().message());
    REQUIRE(addp_e.error());
    CHECK(addp_e.error() == std::errc::invalid_argument);
  }
  SUBCASE("adding non-existent path returns ENOENT") {
    auto ruleset_e { Ruleset::create({.fs = FsAccess::ReadFile}) };
    if(! ruleset_e.has_value()) {
      INFO("Ruleset::create: ", ruleset_e.error().message());
    }
    REQUIRE(ruleset_e.has_value());
    auto ruleset { std::move(ruleset_e.value()) };
    auto addp_e {ruleset.add_path("/foobar/baz", FsAccess::ReadFile)};
    INFO("add_path: ", addp_e.error().message());
    REQUIRE(addp_e.error());
    CHECK(addp_e.error() == std::errc::no_such_file_or_directory);

  }
}
