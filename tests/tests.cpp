#include "doctest/doctest.h"
#include "landlockpp/landlockpp.hpp"

TEST_CASE("abi version") {
  int version { landlockpp::abi_version() };
  INFO(version);
  CHECK(version >= 1);
}
