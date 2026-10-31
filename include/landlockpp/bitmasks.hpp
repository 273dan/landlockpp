#pragma once
#include <cstdint>
#include <linux/landlock.h>
namespace landlockpp {



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


  /* net */

  enum class NetAccess : uint64_t {
    None       = 0,
    BindTCP    = LANDLOCK_ACCESS_NET_BIND_TCP,
    ConnectTCP = LANDLOCK_ACCESS_NET_CONNECT_TCP,
    _LAST,
    All        = ((_LAST - 1) << 1) - 1
  };


  /* scope */

  enum class Scope : uint64_t {
    None               = 0,
    AbstractUnixSocket = LANDLOCK_SCOPE_ABSTRACT_UNIX_SOCKET,
    Signal             = LANDLOCK_SCOPE_SIGNAL,
    _LAST,
    All                = ((_LAST - 1) << 1) - 1
  };


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




    

}
