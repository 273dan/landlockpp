#include "landlockpp/landlockpp.hpp"
#include "doctest/doctest.h"
#include "test_utils.hpp"
using namespace landlockpp;

TEST_SUITE("scope") {
  TEST_CASE("scope signal" * doctest::skip(abi_version().value_or(0) < 6)) {
    constexpr int test_ver { 8 };
    constexpr RulesetFlags test_flags { .scope = Scope::Signal };
    pid_t parent_pid { ::getpid() };
    uint8_t result { 0 };

    test::Runner<test_ver, test_flags> runner { [parent_pid]() -> uint8_t {
      int sig_self { ::kill(::getpid(), 0) };
      int sig_parent { ::kill(parent_pid, 0) };

      uint8_t rv = 1;

      rv |= (sig_self == 0)    << 1; // assertion 1: signalling self succeeds
      rv |= (sig_parent == -1) << 2; // assertion 2: signalling parent fails
      rv |= (errno == EPERM)   << 3; // assertion 3: ^ with EPERM

      return rv;

    }};

    SUBCASE("ruleset api") {
      auto ruleset_e { Ruleset<8, {.scope = Scope::Signal}>::create() };
      test::require_info("Ruleset::create", ruleset_e);

      auto ruleset{std::move(ruleset_e.value())};
      result = runner.run(ruleset);
    }

    SUBCASE("builder api") {
      auto ruleset_e {
        RulesetBuilder<test_ver, test_flags>{}
        .build()
      };
      test::require_info("RulesetBuilder::build", ruleset_e);
      auto ruleset{std::move(ruleset_e.value())};
      result = runner.run(ruleset);
    }




    CHECK_MESSAGE((result & 1), "restrict_self was successful");
    CHECK_MESSAGE(((result >> 1) & 1), "signalling self succeeds");
    CHECK_MESSAGE(((result >> 2) & 1), "signalling parent fails");
    CHECK_MESSAGE(((result >> 3) & 1), "signalling parent fails with EPERM");
  }

}
