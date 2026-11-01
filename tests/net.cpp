#include "doctest/doctest.h"
#include "landlockpp/bitmasks.hpp"
#include "landlockpp/fd.hpp"
#include "landlockpp/landlockpp.hpp"
#include "test_utils.hpp"
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace landlockpp;
TEST_SUITE("net") {

TEST_CASE("net access" * doctest::skip(abi_version().value_or(0) < 4)) {
  constexpr int test_ver { 8 };
  constexpr RulesetFlags test_flags { .net = NetAccess::BindTCP };
  uint8_t result { 0 };

  test::Runner<test_ver, test_flags> runner {[]() -> uint8_t {
    detail::UniqueFd sock1_fd { ::socket(AF_INET, SOCK_STREAM, 0) };
    detail::UniqueFd sock2_fd { ::socket(AF_INET, SOCK_STREAM, 0) };

    sockaddr_in addr1{};
    addr1.sin_port = htons(0);
    addr1.sin_addr = {.s_addr = htonl(INADDR_LOOPBACK)};
    addr1.sin_family = AF_INET;

    sockaddr_in addr2{};
    addr2.sin_port = ::htons(45678);
    addr2.sin_addr = {.s_addr = htonl(INADDR_LOOPBACK)};
    addr2.sin_family = AF_INET;

    int bind1 { ::bind(sock1_fd.get(), reinterpret_cast<sockaddr*>(&addr1), sizeof(addr1)) };

    errno = 0;
    int bind2 { ::bind(sock2_fd.get(), reinterpret_cast<sockaddr*>(&addr2), sizeof(addr2)) };
    int errno2 = errno;

    uint8_t rv = 1;
    rv |= (sock1_fd.get() >= 0) << 1; // assertion 1: creating socket 1 succeeds
    rv |= (sock2_fd.get() >= 0) << 2; // assertion 2: creating socket 2 succeeds
    rv |= (bind1 == 0)          << 3; // assertion 3: bind socket on allowed port suceeds
    rv |= (bind2 == -1)         << 4; // assertion 4: bind socket on denied port fails
    rv |= (errno2 == EACCES)    << 5; // assertion 5: ^ with EACCES

    return rv;
  }};


  SUBCASE("ruleset api") {
    auto ruleset_e { Ruleset<test_ver, test_flags>::create() };
    test::require_info("Ruleset::create", ruleset_e);

    auto ruleset { std::move(ruleset_e.value()) };
    auto adnp_e { ruleset.add_net_port<NetAccess::BindTCP>(0) };
    test::require_info("Ruleset::add_net_port", adnp_e);

    result = runner.run(ruleset);

  }

  SUBCASE("builder api") {
    auto ruleset_e {
      RulesetBuilder<test_ver, test_flags>{}
      .add_net_port<NetAccess::BindTCP>(0)
      .build()
    };
    test::require_info("RulesetBuilder::build", ruleset_e);
    auto ruleset { std::move(ruleset_e.value()) };
    result = runner.run(ruleset);
  }

  CHECK_MESSAGE((result & 1),        "restrict_self was successful");
  CHECK_MESSAGE(((result >> 1) & 1), "creating socket 1 succeeds");
  CHECK_MESSAGE(((result >> 2) & 1), "creating socket 2 succeeds");
  CHECK_MESSAGE(((result >> 3) & 1), "bind socket on allowed port suceeds");
  CHECK_MESSAGE(((result >> 4) & 1), "bind socket on denied port fails");
  CHECK_MESSAGE(((result >> 5) & 1), "bind socket on denied port fails with EACCES");
}
}
