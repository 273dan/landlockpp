#pragma once
#include "landlockpp/bitmasks.hpp"
#include "landlockpp/fd.hpp"
#include "landlockpp/landlockpp.hpp"
#include <expected>
#include <fcntl.h>
#include <filesystem>
#include <linux/landlock.h>
#include <sys/prctl.h>
#include <system_error>
namespace landlockpp {
  struct RulesetFlags {
    FsAccess fs{FsAccess::None};
    NetAccess net{NetAccess::None};
    Scope scope{Scope::None};
  };
class Ruleset {
public:
  [[nodiscard]] static std::expected<Ruleset, std::error_code> create(const RulesetFlags flags) noexcept {
    auto version_e { abi_version() };
    if( ! version_e.has_value()) return std::unexpected{std::move(version_e.error())};
    int version { version_e.value() };
    FsAccess sup_fs { supported_fs_access(version) };
    NetAccess sup_net { supported_net_access(version) };
    Scope sup_scope { supported_scope(version) };

    if(
         (flags.fs & ~sup_fs) != FsAccess::None
      || (flags.net & ~sup_net) != NetAccess::None
      || (flags.scope & ~sup_scope) != Scope::None
    ) return std::unexpected{std::make_error_code(std::errc::not_supported)};

    landlock_ruleset_attr rules_attr{
      .handled_access_fs = std::to_underlying(flags.fs),
      .handled_access_net = std::to_underlying(flags.net),
      .scoped = std::to_underlying(flags.scope)
    };

    int fd { detail::create_ruleset(&rules_attr, 0) };
    if(fd < 0) return std::unexpected{std::error_code{errno, std::system_category()}};

    return Ruleset{fd, flags};

  }

  /**
   *
   */
  [[nodiscard]] std::expected<void, std::error_code> add_path(const std::filesystem::path& path, FsAccess access) noexcept {
    if((access & ~fs_) != FsAccess::None) return std::unexpected{std::make_error_code(std::errc::invalid_argument)};
    detail::UniqueFd fd{ ::open(path.c_str(), O_PATH | O_CLOEXEC) };
    if( ! fd.valid()) return std::unexpected{std::error_code{errno, std::system_category()}};

    landlock_path_beneath_attr attrs{
      .allowed_access = std::to_underlying(access),
      .parent_fd = fd.get()
    };

    int ec { detail::add_rule(fd_.get(), LANDLOCK_RULE_PATH_BENEATH, &attrs, 0) };
    if(ec < 0) return std::unexpected{std::error_code{errno, std::system_category()}};

    return {};
  }

  [[nodiscard]] std::expected<void, std::error_code> restrict_self(bool no_new_privs = true) const noexcept {
    if(no_new_privs) {
      if(::prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0) {
        return std::unexpected{std::error_code{errno, std::system_category()}};
      }

    }
    int ec { detail::restrict_self(fd_.get(), 0) };
    if( ec  < 0) return std::unexpected{std::error_code{errno, std::system_category()}};
    return {};
  }

private:
  Ruleset(int fd, const RulesetFlags flags) : fd_{fd}, fs_{flags.fs}, net_{flags.net}, scope_{flags.scope} {}
  detail::UniqueFd fd_;
  FsAccess fs_;
  NetAccess net_;
  Scope scope_;
};

}
