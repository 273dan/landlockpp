#include "doctest/doctest.h"
#include "landlockpp/landlockpp.hpp"

TEST_CASE("abi version") {
  auto version { landlockpp::abi_version() };
  if( ! version.has_value()) { INFO(version.error()); };
  CHECK(version.value() >= 1);
}

TEST_CASE("errata") {
  auto errata { landlockpp::abi_version() };
  if( ! errata.has_value()) { INFO(errata.error()); };
  CHECK(errata.has_value());
}
