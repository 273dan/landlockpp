#pragma once
#include "landlockpp/bitmasks.hpp"
#include "landlockpp/landlockpp.hpp"
#include "util.hpp"

using namespace landlockpp;

template <typename T, FsAccess access>
concept CanAddPath = requires(T t) { t.template add_path<access>("foo"); };



static_assert(
  CanAddPath<Rules8<{.fs = FsAccess::All}>, FsAccess::ReadFile>,
  "A path can be added when its rule exists in the ruleset"
);

static_assert(
  CanAddPath<Rules8<{.fs = FsAccess::All}>, FsAccess::ReadFile | FsAccess::Write>,
  "A path can be added when all of its ORed rules exist in the ruleset"
);

static_assert(
  ! CanAddPath<Rules8<{.fs = FsAccess::All}>, FsAccess::None>,
  "A path cannot be added when its rule is empty"
);

static_assert(
  ! CanAddPath<
      Rules8<{.fs = FsAccess::ReadFile | FsAccess::ReadDir}>,
      FsAccess::ReadDir | FsAccess::Write>,
  "A path cannot be added when any of its ORed rules do not exist in the ruleset"
    );
