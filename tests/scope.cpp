#include "landlockpp/landlockpp.hpp"
#include "doctest/doctest.h"
#include "test_utils.hpp"
using namespace landlockpp;

TEST_SUITE("scope") {
  TEST_CASE("scope signal") {
    auto ruleset_e { Ruleset<8, {.scope = Scope::Signal}>::create() };
    test::require_info("Ruleset::create", ruleset_e);

    auto ruleset{std::move(ruleset_e.value())};

    pid_t parent_pid { ::getpid() };

    auto runner { test::Runner{std::move(ruleset), [parent_pid]() -> uint8_t {
      int sig_self { ::kill(::getpid(), 0) };
      int sig_parent { ::kill(parent_pid, 0) };

      uint8_t rv = 1;

      rv |= (sig_self == 0)    << 1; // assertion 1: signalling self succeeds
      rv |= (sig_parent == -1) << 2; // assertion 2: signalling parent fails
      rv |= (errno == EPERM)   << 3; // assertion 3: ^ with EPERM

      return rv;

    }}};

    uint8_t e { runner.run() };
    CHECK_MESSAGE((e & 1), "restrict_self was successful");
    CHECK_MESSAGE(((e >> 1) & 1), "signalling self succeeds");
    CHECK_MESSAGE(((e >> 2) & 1), "signalling parent fails");
    CHECK_MESSAGE(((e >> 3) & 1), "signalling parent fails with EPERM");
  }

}
