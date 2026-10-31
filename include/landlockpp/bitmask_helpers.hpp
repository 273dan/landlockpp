#pragma once
#include "landlockpp/bitmasks.hpp"
#include "landlockpp/detail.hpp"
namespace landlockpp {
  namespace detail {




  /* abi helpers
     sources: https://man7.org/linux/man-pages/man7/landlock.7.html 
              https://docs.kernel.org/userspace-api/landlock.html 
   */

  constexpr FsAccess supported_fs_access(int abi) {
    if(abi <= 0) return FsAccess::None;
    auto supported = FsAccess::All;
    switch(abi) {
      case 1: supported &= ~FsAccess::Refer;
              [[fallthrough]];

      case 2: supported &= ~FsAccess::Truncate;
              [[fallthrough]];

      case 3: [[fallthrough]];

      case 4: supported &= ~FsAccess::IoctlDev;
              
      // TODO: add more when upgraded to abi >8
    }

    return supported;
  }

  constexpr NetAccess supported_net_access(int abi) {
    return abi < 4 ? NetAccess::None : NetAccess::All;
  }

  constexpr Scope supported_scope(int abi) {
    return abi < 6 ? Scope::None : Scope::All;
  }

  constexpr RestrictSelf supported_restrict_self(int abi) {
    return abi < 7 ? RestrictSelf::None : RestrictSelf::All;
  }
  }
 
}
