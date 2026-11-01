#pragma once
#include "landlockpp/bitmask_helpers.hpp"
#include "landlockpp/bitmasks.hpp"
#include "landlockpp/detail.hpp"
namespace landlockpp {
  struct RulesetFlags {
    FsAccess fs{FsAccess::None};
    NetAccess net{NetAccess::None};
    Scope scope{Scope::None};
  };

  namespace detail {
    template <RulesetFlags f>
    concept RulesetNotEmpty = requires() {
      requires f.fs != FsAccess::None
               || f.net != NetAccess::None
               || f.scope != Scope::None;
    };

    template <RulesetFlags f, int version>
    concept RulesetSupportedByVersion = requires() {
      requires   (f.fs & ~supported_fs_access(version)) == FsAccess::None
              && (f.net & ~supported_net_access(version)) == NetAccess::None
              && (f.scope & ~supported_scope(version)) == Scope::None;
    };

    template <RulesetFlags flags, FsAccess fs>
    concept RulesetContainsFs = requires() {
      requires (flags.fs & fs) == fs && fs != FsAccess::None;
    };

    template <RulesetFlags flags, NetAccess net>
    concept RulesetContainsNet = requires() {
      requires (flags.net & net) == net && net != NetAccess::None;
    };

  }
}
