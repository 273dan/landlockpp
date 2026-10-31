#include "doctest/doctest.h"
#include "landlockpp/bitmasks.hpp"
#include "test_utils.hpp"
#include "landlockpp/landlockpp.hpp"
#include "landlockpp/ruleset.hpp"
#include <fcntl.h>

using namespace landlockpp;
TEST_CASE("fs access") {
  test::Stage stage{};
  auto ruleset_e { Ruleset::create({.fs = FsAccess::ReadFile | FsAccess::ReadDir }) };
  if( ! ruleset_e.has_value()) INFO("Ruleset::create: ", ruleset_e.error().message());
  REQUIRE(ruleset_e.has_value());
  auto ruleset { std::move(ruleset_e.value()) };
  auto adp_e { ruleset.add_path(test::stage_dir / "a", FsAccess::ReadFile) };


  auto runner { test::Runner(std::move(ruleset), []() -> int {
      std::filesystem::path file_allowed { test::stage_dir / "a" / "1.txt"};
      int fd_allowed { ::open(file_allowed.c_str(), O_RDONLY)};

      std::filesystem::path file_denied { test::stage_dir / "b" / "1.txt"};
      errno = 0;
      int fd_denied { ::open(file_denied.c_str(), O_RDONLY)};
      int denied_errno = errno;

      uint8_t rv = 1;
      rv |= (fd_allowed >= 0)        << 1; // assertion 1: opening allowed file succeeds
      rv |= (fd_denied < 0)          << 2; // assertion 2: opening denied file fails
      rv |= (denied_errno == EACCES) << 3; // assertion 3: ^ with EACCES
      
      return rv;
  }) };

  uint32_t e { runner.run() };
  CHECK_MESSAGE((e & 1),        "restrict_self was successful");
  CHECK_MESSAGE(((e >> 1) & 1), "opening allowed file succeeds");
  CHECK_MESSAGE(((e >> 2) & 1), "opening denied file fails");
  CHECK_MESSAGE(((e >> 3) & 1), "opening denied file fails with EACCES");
  

}
