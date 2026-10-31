#include "doctest/doctest.h"
#include "landlockpp/bitmasks.hpp"
#include "landlockpp/landlockpp.hpp"
#include "landlockpp/ruleset.hpp"
#include <system_error>

using namespace landlockpp;

TEST_CASE("adding rules") {
  SUBCASE("adding non-existent path returns ENOENT") {
    auto ruleset_e { Ruleset<8, {.fs = FsAccess::ReadFile}>::create() };
    if(! ruleset_e.has_value()) {
      INFO("Ruleset::create: ", ruleset_e.error().message());
    }
    REQUIRE(ruleset_e.has_value());
    auto ruleset { std::move(ruleset_e.value()) };
    auto addp_e {ruleset.add_path<FsAccess::ReadFile>("/foobar/baz")};
    INFO("add_path: ", addp_e.error().message());
    REQUIRE(addp_e.error());
    CHECK(addp_e.error() == std::errc::no_such_file_or_directory);

  }
}
