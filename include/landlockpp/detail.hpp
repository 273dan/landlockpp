#pragma once
#include <type_traits>
#include <utility>
#include "bitmasks.hpp"
namespace landlockpp {


  /* bitmask templates and operations */

  template <typename E> 
  inline constexpr bool is_enum_bitmask_v { false };

  template <typename E>
  concept Bitmask = requires() {
    requires std::is_enum_v<E> && is_enum_bitmask_v<E>;
    // E::None;
  };

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

  template <>
  inline constexpr bool is_enum_bitmask_v<FsAccess> { true };

  template <>
  inline constexpr bool is_enum_bitmask_v<NetAccess> { true };

  template <>
  inline constexpr bool is_enum_bitmask_v<Scope> { true };

  template <>
  inline constexpr bool is_enum_bitmask_v<RestrictSelf> { true };
}
