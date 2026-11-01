#pragma once
#include "landlockpp/bitmasks.hpp"
#include "landlockpp/landlockpp.hpp"
using namespace landlockpp;

template <typename T, RestrictSelf rflags>
concept CanRestrictSelf = requires(T t) {
  t.template restrict_self<rflags>();
};

static_assert(
    CanRestrictSelf<Ruleset<7, {.fs = FsAccess::All}>, RestrictSelf::All>,
    "Restrict self enabled with all flags when version >= 7"
);

static_assert(
    ! CanRestrictSelf<Ruleset<6, {.fs = FsAccess::All}>, RestrictSelf::All>,
    "Restrict disabled with any flags when version < 7"
);

static_assert(
    CanRestrictSelf<Ruleset<6, {.fs = FsAccess::All}>, RestrictSelf::None>,
    "Restrict enabled when version < 7 if using no flags"
);

static_assert(
    CanRestrictSelf<RulesetBuilder<7, {.fs = FsAccess::All}>, RestrictSelf::All>,
    "(builder) Restrict self enabled with all flags when version >= 7"
);

static_assert(
    ! CanRestrictSelf<RulesetBuilder<6, {.fs = FsAccess::All}>, RestrictSelf::All>,
    "(builder) Restrict disabled with any flags when version < 7"
);

static_assert(
    CanRestrictSelf<RulesetBuilder<6, {.fs = FsAccess::All}>, RestrictSelf::None>,
    "(builder) Restrict enabled when version < 7 if using no flags"
);
