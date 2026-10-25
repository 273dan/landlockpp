#pragma once
#include <cstdint>
#include <linux/landlock.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace landlockpp {
  namespace detail {
    [[nodiscard]] inline int create_ruleset(
        const landlock_ruleset_attr* attr,
        uint32_t flags) noexcept {
      return ::syscall(SYS_landlock_create_ruleset, attr, sizeof(landlock_ruleset_attr), flags);
    }

    [[nodiscard]] inline int create_ruleset(
        const landlock_ruleset_attr* attr,
        size_t size,
        uint32_t flags) noexcept {
      return ::syscall(SYS_landlock_create_ruleset, attr, size, flags);
    }

    [[nodiscard]] inline int add_rule(
        int ruleset_fd,
        landlock_rule_type rule_type,
        const void* rule_attr,
        uint32_t flags) noexcept {
      return ::syscall(SYS_landlock_add_rule, rule_type, rule_attr, flags);
    }
  }

  [[nodiscard]] static int abi_version() {
    return detail::create_ruleset(nullptr, 0, LANDLOCK_CREATE_RULESET_VERSION);
  }
}
