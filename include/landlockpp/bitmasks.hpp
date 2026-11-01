#pragma once
#include <cstdint>
namespace landlockpp {



  /* fs */

  enum class FsAccess : uint64_t {
    None       = 0,
    Execute    = (1ULL << 0),
    Write      = (1ULL << 1),
    ReadFile   = (1ULL << 2),
    ReadDir    = (1ULL << 3),
    RemoveDir  = (1ULL << 4),
    RemoveFile = (1ULL << 5),
    MakeChar   = (1ULL << 6),
    MakeDir    = (1ULL << 7),
    MakeReg    = (1ULL << 8),
    MakeSock   = (1ULL << 9),
    MakeFifo   = (1ULL << 10),
    MakeBlock  = (1ULL << 11),
    MakeSym    = (1ULL << 12),
    Refer      = (1ULL << 13),
    Truncate   = (1ULL << 14),
    IoctlDev   = (1ULL << 15),
    LAST_,
    All        = ((LAST_ - 1) << 1) - 1
  };


  /* net */

  enum class NetAccess : uint64_t {
    None       = 0,
    BindTCP    = (1ULL << 0),
    ConnectTCP = (1ULL << 1),
    LAST_,
    All        = ((LAST_ - 1) << 1) - 1
  };


  /* scope */

  enum class Scope : uint64_t {
    None               = 0,
    AbstractUnixSocket = (1ULL << 0),
    Signal             = (1ULL << 1),
    LAST_,
    All                = ((LAST_ - 1) << 1) - 1
  };


  /* restrict self log */
  
  enum class RestrictSelf : uint32_t {
    None             = 0,
    LogSameExecOff   = (1U << 0),
    LogNewExecOn     = (1U << 1),
    LogSubdomainsOff = (1U << 2),
    LAST_,
    All              = ((LAST_ - 1) << 1) - 1
    // TODO: add tsync when upgraded to abi 8
  };




    

}
