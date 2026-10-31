#pragma once
#include "landlockpp/bitmasks.hpp"
#include "landlockpp/fd.hpp"
#include "landlockpp/ruleset_flags.hpp"
#include "landlockpp/util.hpp"
#include <expected>
#include <fcntl.h>
#include <filesystem>
#include <linux/landlock.h>
#include <sys/prctl.h>
#include <system_error>
namespace landlockpp {
  template <typename T>
  using exp_error = std::expected<T, std::error_code>;

struct RulesetBase {
protected:
  detail::UniqueFd fd_;
  FsAccess fs_;
  NetAccess net_;
  Scope scope_;

  template <bool AssumeValidRule>
  [[nodiscard]]
  exp_error<void> add_path_impl(const std::filesystem::path& path, FsAccess access)
  const noexcept {

    if constexpr( ! AssumeValidRule) {
      if((access & ~fs_) != FsAccess::None)
        return std::unexpected{std::make_error_code(std::errc::invalid_argument)};
    }

    detail::UniqueFd fd{ ::open(path.c_str(), O_PATH | O_CLOEXEC) };
    if( ! fd.valid())
      return std::unexpected{std::error_code{errno, std::system_category()}};

    landlock_path_beneath_attr attrs{
      .allowed_access = std::to_underlying(access),
      .parent_fd = fd.get()
    };

    int ec { detail::add_rule(fd_.get(), LANDLOCK_RULE_PATH_BENEATH, &attrs, 0) };

    if(ec < 0)
      return std::unexpected{std::error_code{errno, std::system_category()}};

    return {};
  }

  RulesetBase(int fd, RulesetFlags flags) :
    fd_{fd},
    fs_{flags.fs},
    net_{flags.net},
    scope_{flags.scope} {}

  [[nodiscard]] std::expected<void, std::error_code> restrict_self_impl(bool no_new_privs = true) const noexcept {

    if(no_new_privs) {
      if(::prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0) {
        return std::unexpected{std::error_code{errno, std::system_category()}};
      }
    }

    int ec { detail::restrict_self(fd_.get(), 0) };
    if( ec  < 0) return std::unexpected{std::error_code{errno, std::system_category()}};
    return {};
  }
};


template <int version, RulesetFlags flags = {}>
class Ruleset : public RulesetBase {
public:


  [[nodiscard]]
  static exp_error<Ruleset> create()
  noexcept
  requires(detail::RulesetNotEmpty<flags>
           && detail::RulesetSupportedByVersion<flags, version>
  ) {
    landlock_ruleset_attr rules_attr{
      .handled_access_fs = std::to_underlying(flags.fs),
      .handled_access_net = std::to_underlying(flags.net),
      .scoped = std::to_underlying(flags.scope)
    };

    int fd { detail::create_ruleset(&rules_attr, 0) };
    if(fd < 0) return std::unexpected{std::error_code{errno, std::system_category()}};

    return Ruleset{fd};
  }

  /**
   *
   */
  template <FsAccess access>
  [[nodiscard]]
  exp_error<void> add_path(const std::filesystem::path& path)
  noexcept
  requires(detail::RulesetContainsFs<flags, access>) {
    return add_path_impl<true>(path, access);
  }

  [[nodiscard]]
  exp_error<void> restrict_self(bool no_new_privs = true)
  const noexcept {
    return restrict_self_impl(no_new_privs);
  }


private:
  Ruleset(int fd) : RulesetBase{fd, flags} {}
};

}
