#pragma once
#include "landlockpp/bitmasks.hpp"
#include "landlockpp/landlockpp.hpp"

using namespace landlockpp;

template <typename T>
concept CanCreate = requires { T::create(); };

static_assert(
  CanCreate<Ruleset<7, {.fs = FsAccess::ReadFile}>>,
  "A ruleset can be created with some flags and version > 0"
); 

static_assert(
  ! CanCreate<Ruleset<8, {}>>,
  "A ruleset cannot be created with no flags"
); 

static_assert(
  ! CanCreate<Ruleset<0, {.fs = FsAccess::ReadFile}>>,
  "A ruleset cannot be created with version <= 0"
);

