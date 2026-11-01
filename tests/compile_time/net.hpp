#pragma once
#include "landlockpp/bitmasks.hpp"
#include "landlockpp/landlockpp.hpp"
#include "util.hpp"

using namespace landlockpp;

template <typename T, NetAccess access>
concept CanAddNetPort = requires(T t) {
  t.template add_net_port<access>(80);
};

static_assert(
  CanAddNetPort<Rules8<{.net = NetAccess::All}>, NetAccess::BindTCP>,
  "A port can be added when its rule exists in the ruleset"
);

static_assert(
  ! CanAddNetPort<Rules8<{.net = NetAccess::ConnectTCP}>, NetAccess::BindTCP>,
  "A port cannot be added when its rule does not exist in the ruleset"
);

static_assert(
  CanAddNetPort<Rules8<{.net = NetAccess::All}>, NetAccess::BindTCP | NetAccess::ConnectTCP>,
  "A port can be added when all of its ORed rules exist in the ruleset"
);

static_assert(
  ! CanAddNetPort<Rules8<{.net = NetAccess::All}>, NetAccess::None>,
  "A port cannot be added when its rule is empty"
);




