#pragma once
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <sys/syscall.h>
#include <system_error>
#include <unistd.h>

namespace landlockpp {
namespace detail {

  inline constexpr uint32_t create_ruleset_version { 1U << 0 };
  inline constexpr uint32_t create_ruleset_errata  { 1U << 1 };

  enum class RuleType : int {
    PathBeneath = 1,
    NetPort     = 2,
  };

  struct ruleset_attr {
    uint64_t handled_access_fs{};
    uint64_t handled_access_net{};
    uint64_t scoped{};
  };

  struct __attribute__((packed)) path_beneath_attr {
    uint64_t allowed_access{};
    int32_t parent_fd{};
  };

  struct net_port_attr {
    uint64_t allowed_access{};
    uint64_t port{};
  };

  [[nodiscard]]
  inline int create_ruleset(const ruleset_attr* attr, uint32_t flags)
  noexcept {
    return ::syscall(SYS_landlock_create_ruleset, attr, sizeof(ruleset_attr), flags);
  }

  [[nodiscard]]
  inline int create_ruleset(const ruleset_attr* attr, size_t size, uint32_t flags)
  noexcept {
    return ::syscall(SYS_landlock_create_ruleset, attr, size, flags);
  }

  [[nodiscard]]
  inline int add_rule(int ruleset_fd, RuleType rule_type,
                      const void* rule_attr, uint32_t flags)
  noexcept {
    return ::syscall(SYS_landlock_add_rule, ruleset_fd, static_cast<int>(rule_type), rule_attr, flags);
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
    int version { detail::create_ruleset(nullptr, 0, detail::create_ruleset_version) };
    if (version < 0) return std::unexpected{std::error_code{errno, std::system_category()}};
    return version;
  }

  [[nodiscard]]
  inline std::expected<int, std::error_code> errata()
  noexcept {
    int errata { detail::create_ruleset(nullptr, 0, detail::create_ruleset_errata) };
    if (errata < 0) return std::unexpected{std::error_code{errno, std::system_category()}};
    return errata;
  }

}
