#pragma once
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <linux/landlock.h>
#include <sys/syscall.h>
#include <system_error>
#include <unistd.h>

namespace landlockpp {
namespace detail {

  [[nodiscard]]
  inline int create_ruleset(const landlock_ruleset_attr* attr, uint32_t flags)
  noexcept {
    return ::syscall(SYS_landlock_create_ruleset, attr, sizeof(landlock_ruleset_attr), flags);
  }

  [[nodiscard]]
  inline int create_ruleset(const landlock_ruleset_attr* attr, size_t size, uint32_t flags)
  noexcept {
    return ::syscall(SYS_landlock_create_ruleset, attr, size, flags);
  }

  [[nodiscard]]
  inline int add_rule(int ruleset_fd, landlock_rule_type rule_type,
                      const void* rule_attr, uint32_t flags)
  noexcept {
    return ::syscall(SYS_landlock_add_rule, ruleset_fd, rule_type, rule_attr, flags);
  }

  [[nodiscard]]
  inline int restrict_self(int ruleset_fd, uint32_t flags)
  noexcept {
    return ::syscall(SYS_landlock_restrict_self, ruleset_fd, flags);
  }

} // namespace detail

  [[nodiscard]]
  inline std::expected<int, std::error_code> abi_version()
  noexcept {
    int version { detail::create_ruleset(nullptr, 0, LANDLOCK_CREATE_RULESET_VERSION) };
    if (version < 0) return std::unexpected{std::error_code{errno, std::system_category()}};
    return version;
  }

  [[nodiscard]]
  inline std::expected<int, std::error_code> errata()
  noexcept {
    int errata { detail::create_ruleset(nullptr, 0, LANDLOCK_CREATE_RULESET_ERRATA) };
    if (errata < 0) return std::unexpected{std::error_code{errno, std::system_category()}};
    return errata;
  }

}
