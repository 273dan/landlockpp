#pragma once
#include <cstdint>
#include <linux/landlock.h>
#include <type_traits>
#include <utility>
namespace landlockpp {

  /* bitmask templates and operations */

  template <typename E> 
  inline constexpr bool is_enum_bitmask_v { false };

  template <typename E>
  concept Bitmask = std::is_enum_v<E> && is_enum_bitmask_v<E>;

  template <Bitmask E>
  constexpr E operator|(E l, E r) { return static_cast<E>(std::to_underlying(l) | std::to_underlying(r)); }

  template <Bitmask E>
  constexpr E operator&(E l, E r) { return static_cast<E>(std::to_underlying(l) & std::to_underlying(r)); }

  template <Bitmask E>
  constexpr E operator^(E l, E r) { return static_cast<E>(std::to_underlying(l) ^ std::to_underlying(r)); }

  template <Bitmask E>
  constexpr E& operator|=(E& l, E r) { return  (l = l | r); }

  template <Bitmask E>
  constexpr E& operator^=(E& l, E r) { return (l = l ^ r); }

  template <Bitmask E>
  constexpr E& operator&=(E& l, E r) { return (l = l & r) ; }

  template <Bitmask E>
  constexpr E operator~(E e) { return static_cast<E>(~std::to_underlying(e)); }

  /* fs */

  enum class FsAccess : uint64_t {
    None       = 0,
    Execute    = LANDLOCK_ACCESS_FS_EXECUTE,
    Write      = LANDLOCK_ACCESS_FS_WRITE_FILE,
    ReadFile   = LANDLOCK_ACCESS_FS_READ_FILE,
    ReadDir    = LANDLOCK_ACCESS_FS_READ_DIR,
    RemoveDir  = LANDLOCK_ACCESS_FS_REMOVE_DIR,
    RemoveFile = LANDLOCK_ACCESS_FS_REMOVE_FILE,
    MakeChar   = LANDLOCK_ACCESS_FS_MAKE_CHAR,
    MakeDir    = LANDLOCK_ACCESS_FS_MAKE_DIR,
    MakeReg    = LANDLOCK_ACCESS_FS_MAKE_REG,
    MakeSock   = LANDLOCK_ACCESS_FS_MAKE_SOCK,
    MakeFifo   = LANDLOCK_ACCESS_FS_MAKE_FIFO,
    MakeBlock  = LANDLOCK_ACCESS_FS_MAKE_BLOCK,
    MakeSym    = LANDLOCK_ACCESS_FS_MAKE_SYM,
    Refer      = LANDLOCK_ACCESS_FS_REFER,
    Truncate   = LANDLOCK_ACCESS_FS_TRUNCATE,
    IoctlDev   = LANDLOCK_ACCESS_FS_IOCTL_DEV,
    _LAST,
    All        = ((_LAST - 1) << 1) - 1
  };

  template <>
  inline constexpr bool is_enum_bitmask_v<FsAccess> { true };

  /* net */

  enum class NetAccess : uint64_t {
    None       = 0,
    BindTCP    = LANDLOCK_ACCESS_NET_BIND_TCP,
    ConnectTCP = LANDLOCK_ACCESS_NET_CONNECT_TCP,
    _LAST,
    All        = ((_LAST - 1) << 1) - 1
  };

  template <>
  inline constexpr bool is_enum_bitmask_v<NetAccess> { true };

  /* scope */

  enum class Scope : uint64_t {
    None               = 0,
    AbstractUnixSocket = LANDLOCK_SCOPE_ABSTRACT_UNIX_SOCKET,
    Signal             = LANDLOCK_SCOPE_SIGNAL,
    _LAST,
    All                = ((_LAST - 1) << 1) - 1
  };

  template <>
  inline constexpr bool is_enum_bitmask_v<Scope> { true };

  /* restrict self log */
  
  enum class RestrictSelf : uint32_t {
    None             = 0,
    LogSameExecOff   = LANDLOCK_RESTRICT_SELF_LOG_SAME_EXEC_OFF,
    LogNewExecOn     = LANDLOCK_RESTRICT_SELF_LOG_NEW_EXEC_ON,
    LogSubdomainsOff = LANDLOCK_RESTRICT_SELF_LOG_SUBDOMAINS_OFF,
    _LAST,
    All              = ((_LAST - 1) << 1) - 1
    // TODO: add tsync when upgraded to abi 8
  };

  template <>
  inline constexpr bool is_enum_bitmask_v<RestrictSelf> { true };

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
